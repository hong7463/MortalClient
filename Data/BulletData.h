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
#pragma once
#include "../Graphics/Animation.h"
#include "ItemData.h"

namespace jrc
{
//! Information about a bullet type item.
class BulletData : public Cache<BulletData>
{
public:
    //! Returns wether the bullet was loaded correctly.
    [[nodiscard]] bool is_valid() const;
    //! Returns wether the bullet was loaded correctly.
    explicit operator bool() const;

    //! Returns the watk increase when using this bullet.
    [[nodiscard]] std::int16_t get_watk() const;
    //! Returns the bullet animation.
    [[nodiscard]] const Animation& get_animation() const;
    //! Returns the general item data.
    [[nodiscard]] const ItemData& get_item_data() const;

private:
    //! Allow the cache to use the constructor.
    friend Cache<BulletData>;
    //! Load a bullet from the game files.
    BulletData(std::int32_t item_id);

    const ItemData& item_data;

    Animation bullet;
    std::int16_t watk;
};
} // namespace jrc
