//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple Team        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <https://www.gnu.org/licenses/>.   //
//////////////////////////////////////////////////////////////////////////////
#include "AttackHandlers.h"

#include "../../Character/SkillId.h"
#include "../../Gameplay/Stage.h"

#include <unordered_map>
#include <vector>

namespace jrc
{
AttackHandler::AttackHandler(Attack::Type t) noexcept : type{t}
{
}

void AttackHandler::handle(InPacket& recv) const
{
    std::int32_t cid = recv.read_int();
    std::uint8_t count = recv.read_byte();

    recv.skip(1);

    AttackResult attack;
    attack.type = type;
    attack.attacker = cid;

    attack.level = recv.read_byte();
    attack.skill = attack.level > 0 ? recv.read_int() : 0;

    attack.display = recv.read_byte();
    attack.to_left = recv.read_bool();
    attack.stance = recv.read_byte();
    attack.speed = recv.read_byte();

    recv.skip(1);

    attack.bullet = recv.read_int();

    attack.mob_count = (count >> 4) & 0x0F;
    attack.hit_count = count & 0x0F;
    for (std::uint8_t i = 0; i < attack.mob_count; ++i) {
        std::int32_t oid = recv.read_int();

        recv.skip(1);

        std::uint8_t length = attack.skill == SkillId::MESO_EXPLOSION
                                  ? recv.read_byte()
                                  : attack.hit_count;
        for (std::uint8_t j = 0; j < length; ++j) {
            std::int32_t damage = recv.read_int();
            bool critical = false; // TODO
            attack.damage_lines[oid].emplace_back(damage, critical);
        }
    }

    Stage::get().get_combat().push_attack(attack);
}
} // namespace jrc
