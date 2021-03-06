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

#ifndef FALLTERGEIST_PLAYEREDITSTATE_H
#define	FALLTERGEIST_PLAYEREDITSTATE_H

// C++ standard includes
#include <map>
#include <vector>

// Falltergeist includes
#include "../Engine/State.h"
#include "../UI/BigCounter.h"
#include "../UI/HiddenMask.h"
#include "../UI/ImageButton.h"
#include "../UI/TextArea.h"

// Third party includes

namespace Falltergeist
{

class PlayerEditState : public State
{
protected:
    TextArea* _selectedLabel = 0;
    TextArea* _title = 0;
    TextArea* _description = 0;
    Image* _selectedImage = 0;
    std::map<std::string, TextArea*> _labels;
    std::map<std::string, BigCounter*> _counters;
    std::map<std::string, ImageButton*> _buttons;
    std::map<std::string, HiddenMask*> _masks;
    std::map<std::string, std::string> _titles;
    std::map<std::string, std::string> _descriptions;
    std::map<std::string, Image*> _images;

    TextArea* _addLabel(std::string name, TextArea* label);
    ImageButton* _addButton(std::string name, ImageButton* button);
    BigCounter* _addCounter(std::string name, BigCounter* counter);
    HiddenMask* _addMask(std::string name, HiddenMask* mask);
    void _addTitle(std::string name, std::string title);
    void _addDescription(std::string name, std::string description);
    void _addImage(std::string name, Image* image);

    bool _statIncrease(unsigned int num);
    bool _statDecrease(unsigned int num);
    bool _traitToggle(unsigned int num);
    bool _skillToggle(unsigned int num);

public:
    PlayerEditState();
    void init();
    void think();
    virtual ~PlayerEditState();

    void onMaskClick(std::shared_ptr<MouseEvent> event);
    void onButtonClick(std::shared_ptr<MouseEvent> event);
    void onAgeButtonClick(std::shared_ptr<MouseEvent> event);
    void onNameButtonClick(std::shared_ptr<MouseEvent> event);
    void onGenderButtonClick(std::shared_ptr<MouseEvent> event);
    void onLabelClick(std::shared_ptr<MouseEvent> event);
    void onBackButtonClick(std::shared_ptr<MouseEvent> event);
    void onDoneButtonClick(std::shared_ptr<MouseEvent> event);
};

}
#endif /* FALLTERGEIST_PLAYEREDITSTATE_H */

