/*
 * Copyright 2012-2014 Falltergeist Developers.
 *
 * This file is part of Falltergeist.
 *
 * Falltergeist is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Falltergeist is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Falltergeist.  If not, see <http://www.gnu.org/licenses/>.
 */

// C++ standard includes

// Falltergeist includes
#include "../UI/MonthCounter.h"
#include "../UI/Image.h"
#include "../Engine/Graphics/Texture.h"

// Third party includes

namespace Falltergeist
{

enum
{
    MONTH_TEXTURE_WIDTH  = 28,
    MONTH_TEXTURE_HEIGHT = 14,
    MONTH_TEXTURE_HORIZONTAL_OFFSET = 15
};

MonthCounter::MonthCounter(Month month, int x, int y) : ImageList(x, y), _month(month)
{
    auto months = std::shared_ptr<Image>(new Image("art/intrface/months.frm"));

    for (auto i = 0, hOffset = 0; i < 12; ++i, hOffset += MONTH_TEXTURE_HORIZONTAL_OFFSET)
    {
        auto monthImage = new Image(MONTH_TEXTURE_WIDTH, MONTH_TEXTURE_HEIGHT);
        months->texture()->copyTo(monthImage->texture(), 0, 0, 0, hOffset, MONTH_TEXTURE_WIDTH, MONTH_TEXTURE_HEIGHT);
        addImage(monthImage);
    }
    setCurrentImage(month);
}

MonthCounter::MonthCounter(int x, int y) : MonthCounter(JANUARY, x, y)
{
}

MonthCounter::Month MonthCounter::month() const
{
    return _month;
}

void MonthCounter::setMonth(Month month)
{
    _month = month;
    setCurrentImage(month);
}

}
