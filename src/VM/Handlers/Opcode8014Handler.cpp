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
#include "../../Engine/Exception.h"
#include "../../Engine/Game.h"
#include "../../States/LocationState.h"
#include "../../Engine/Logger.h"
#include "../../VM/Handlers/Opcode8014Handler.h"
#include "../../VM/VM.h"
#include "../../VM/VMStackValue.h"

// Third party includes

namespace Falltergeist
{

Opcode8014Handler::Opcode8014Handler(Falltergeist::VM *vm) : OpcodeHandler(vm)
{
}

void Opcode8014Handler::_run()
{
    Logger::debug("SCRIPT") << "[8014] [+] getExported(name)" << std::endl;
    auto game = Game::getInstance();
    auto EVARS = game->locationState()->EVARS();
    switch (_vm->dataStack()->top()->type())
    {
        case VMStackValue::TYPE_INTEGER:
            _vm->dataStack()->push(EVARS->at(_vm->script()->identificators()->at(_vm->popDataInteger())));
            break;
        case VMStackValue::TYPE_POINTER:
        {
            auto string = static_cast<std::string*>(_vm->popDataPointer());
            _vm->dataStack()->push(EVARS->at(*string));
            break;
        }
        default:
            throw Exception("VM::opcode8014 error");
    }
}
}
