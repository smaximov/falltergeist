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

#ifndef FALLTERGEIST_LOGGER_H
#define FALLTERGEIST_LOGGER_H

// C++ standard includes
#include <iosfwd>
#include <iostream>

// Falltergeist includes

// Third party includes

namespace Falltergeist
{

class Logger
{
public:
    enum class Level
    {
        DEBUG    = 0,
        INFO     = 1,
        WARNING  = 2,
        ERROR    = 3,
        CRITICAL = 4,
    };

    static Level level();
    static void setLevel(Level level);
    static void setLevel(const std::string &level);
    static const char *levelString(Level level);

    static const bool colorsSupported;
    static void useColors(bool useColors);

    static std::ostream &log(Level level, const std::string &subsystem = "");
    static std::ostream &debug(const std::string &subsystem = "");
    static std::ostream &info(const std::string &subsystem = "");
    static std::ostream &warning(const std::string &subsystem = "");
    static std::ostream &error(const std::string &subsystem = "");
    static std::ostream &critical(const std::string &subsystem = "");

private:
    static Level _level;
    static bool _useColors;
};

}

#endif // FALLTERGEIST_LOGGER_H
