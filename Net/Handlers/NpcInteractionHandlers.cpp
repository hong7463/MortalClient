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
#include "NpcInteractionHandlers.h"

#include "../../IO/UI.h"
#include "../../IO/UITypes/UINpcTalk.h"
#include "../../IO/UITypes/UIShop.h"

namespace jrc
{
void NpcDialogueHandler::handle(InPacket& recv) const
{
    recv.skip(1);

    std::int32_t npcid = recv.read_int();

    // 0  : Text only.
    // 1  : Yes/no.
    // 4  : Selection.
    // 12 : Accept/decline.
    std::int8_t msgtype = recv.read_byte();

    std::int8_t speaker = recv.read_byte();
    std::string text = recv.read_string();

    std::int16_t style = 0;
    if (msgtype == 0 && recv.length() > 0) {
        style = recv.read_short();
    }

    UI::get().emplace<UINpcTalk>();
    UI::get().enable();

    if (auto npctalk = UI::get().get_element<UINpcTalk>()) {
        npctalk->change_text(npcid, msgtype, style, speaker, std::move(text));
    }
}

void OpenNpcShopHandler::handle(InPacket& recv) const
{
    std::int32_t npcid = recv.read_int();
    auto oshop = UI::get().get_element<UIShop>();

    if (!oshop)
        return;

    UIShop& shop = *oshop;

    shop.reset(npcid);

    std::int16_t size = recv.read_short();
    for (std::int16_t i = 0; i < size; ++i) {
        std::int32_t itemid = recv.read_int();
        std::int32_t price = recv.read_int();
        std::int32_t pitch = recv.read_int();
        std::int32_t time = recv.read_int();

        recv.skip(4);

        bool norecharge = recv.read_short() == 1;
        if (norecharge) {
            std::int16_t buyable = recv.read_short();

            shop.add_item(itemid, price, pitch, time, buyable);
        } else {
            recv.skip(4);

            std::int16_t rechargeprice = recv.read_short();
            std::int16_t slotmax = recv.read_short();

            shop.add_rechargable(
                itemid, price, pitch, time, rechargeprice, slotmax);
        }
    }
}
} // namespace jrc
