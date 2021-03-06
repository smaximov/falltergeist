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
#include <sstream>

// Falltergeist includes
#include "../Engine/Font.h"
#include "../Engine/Game.h"
#include "../Engine/Graphics/Renderer.h"
#include "../Engine/ResourceManager.h"
#include "../Game/GameDudeObject.h"
#include "../States/LocationState.h"
#include "../States/PlayerEditAgeState.h"
#include "../States/PlayerEditAlertState.h"
#include "../States/PlayerEditGenderState.h"
#include "../States/PlayerEditNameState.h"
#include "../States/PlayerCreateState.h"
#include "../UI/Image.h"

// Third party includes
#include <libfalltergeist.h>

namespace Falltergeist
{

PlayerCreateState::PlayerCreateState() : State()
{
    // background
    auto background = new Image("art/intrface/edtrcrte.frm");
    auto backgroundX = (Game::getInstance()->renderer()->width() - background->width())*0.5;
    auto backgroundY = (Game::getInstance()->renderer()->height() - background->height())*0.5;
    background->setX(backgroundX);
    background->setY(backgroundY);
    addUI(background);

    // STATS
    auto msgStats = ResourceManager::msgFileType("text/english/game/stat.msg");
    std::string imagesStats[] = { "strength", "perceptn", "endur", "charisma", "intel", "agility", "luck"};
    for (unsigned int i = 0; i != 7; ++i)
    {
        std::stringstream ss;
        ss << "stats_" << (i+1);

        _addTitle(ss.str(), msgStats->message(100 + i)->text());       // stat title
        _addDescription(ss.str(), msgStats->message(200 + i)->text()); // stat description
        _addImage(ss.str(), new Image("art/skilldex/" + imagesStats[i] + ".frm")); // stat image
        _addLabel(ss.str(), new TextArea(backgroundX+102, backgroundY+46+33*i));          // stat value label
        _addCounter(ss.str(), new BigCounter(backgroundX+59, backgroundY+37+33*i));       // stat value counter
        _addMask(ss.str(), new HiddenMask(133, 29, backgroundX+14, backgroundY+36+33*i)); // stat click mask
        _addButton(ss.str() + "_increase", new ImageButton(ImageButton::TYPE_PLUS,  backgroundX+149, backgroundY+38+33*i)); // stat increase button
        _addButton(ss.str() + "_decrease", new ImageButton(ImageButton::TYPE_MINUS, backgroundX+149, backgroundY+49+33*i)); // stat decrease button
    }

    _addCounter("statsPoints", new BigCounter(backgroundX+126, backgroundY+282)); // Free stats points counter

    // TRAITS
    auto msgTraits = ResourceManager::msgFileType("text/english/game/trait.msg");
    std::string imagesTraits[] = { "fastmeta", "bruiser", "smlframe", "onehand", "finesse", "kamikaze", "heavyhnd", "fastshot",
                              "bldmess", "jinxed", "goodnatr", "addict", "drugrest", "empathy", "skilled", "gifted"};
    for (unsigned int i = 0; i != 16; ++i)
    {
        std::stringstream ss;
        ss << "traits_" << (i+1);
        _addTitle(ss.str(), msgTraits->message(100 + i)->text()); // trait title
        _addDescription(ss.str(), msgTraits->message(200 + i)->text()); // trait description
        _addImage(ss.str(), new Image("art/skilldex/" + imagesTraits[i] + ".frm")); // trait image
        // left column
        if (i <= 7)
        {
            _addLabel(ss.str(),  new TextArea(msgTraits->message(100 + i), backgroundX+48, backgroundY+353+13*i)); // trate label
            _addButton(ss.str(), new ImageButton(ImageButton::TYPE_SKILL_TOGGLE, backgroundX+23, backgroundY+352+13*i)); // trate toggle button
        }
        //right column
        else
        {
            _addLabel(ss.str(),  new TextArea(msgTraits->message(100 + i), backgroundX+169, backgroundY+353+13*(i-8)))->setWidth(122)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_RIGHT); // trate label
            _addButton(ss.str(), new ImageButton(ImageButton::TYPE_SKILL_TOGGLE, backgroundX+299, backgroundY+352+13*(i-8))); // trate toggle button
        }
    }

    // SKILLS
    auto msgSkills = ResourceManager::msgFileType("text/english/game/skill.msg");
    std::string imagesSkills[] = { "gunsml", "gunbig", "energywp", "unarmed", "melee", "throwing", "firstaid", "doctor", "sneak",
                                 "lockpick", "steal", "traps", "science", "repair", "speech", "barter", "gambling", "outdoors"};
    for (unsigned int i = 0; i != 18; ++i)
    {
        std::stringstream ss;
        ss << "skills_" << (i+1);
        _addTitle(ss.str(), msgSkills->message(100 + i)->text());
        _addDescription(ss.str(), msgSkills->message(200 + i)->text());
        _addImage(ss.str(), new Image("art/skilldex/" + imagesSkills[i] + ".frm"));
        _addButton(ss.str(), new ImageButton(ImageButton::TYPE_SKILL_TOGGLE, backgroundX+347, backgroundY+26+11*i));
        _addLabel(ss.str(),  new TextArea(msgSkills->message(100 + i), backgroundX+377, backgroundY+27+11*i))->setWidth(240);
        _addLabel(ss.str() + "_value",  new TextArea("", backgroundX+577, backgroundY+27+11*i));
    }
    // Free skill points counts
    _addCounter("skillsPoints", new BigCounter(backgroundX+522, backgroundY+228));

    // HEALTH CONDITION
    auto msgHealth = ResourceManager::msgFileType("text/english/game/editor.msg");
    std::string imagesHealth[] = { "hitpoint", "poisoned", "radiated", "eyedamag", "armright", "armleft", "legright", "legleft"};
    _addTitle("health_1", msgHealth->message(300)->text());
    _addLabel("health_1",  new TextArea(msgHealth->message(300), backgroundX+194, backgroundY+46)); //health
    _addDescription("health_1", ResourceManager::msgFileType("text/english/game/stat.msg")->message(207)->text());
    _addImage("health_1", new Image("art/skilldex/" + imagesHealth[0] + ".frm"));

    auto font1_0x183018ff = ResourceManager::font("font1.aaf", 0x183018ff);

    for (unsigned int i = 0; i != 7; ++i)
    {
        std::stringstream ss;
        ss << "health_" << (i+2);
        _addTitle(ss.str(), msgHealth->message(312 + i)->text());
        _addDescription(ss.str(), msgHealth->message(400 + i)->text());
        _addLabel(ss.str(), new TextArea(msgHealth->message(312+i), backgroundX+194, backgroundY+46+13*(i+1)))->setFont(font1_0x183018ff);
        _addImage(ss.str(), new Image("art/skilldex/" + imagesHealth[i+1] + ".frm"));
    }

    // PLAYER PARAMS
    auto msgStat = ResourceManager::msgFileType("text/english/game/stat.msg");
    auto msgEditor = ResourceManager::msgFileType("text/english/game/editor.msg");
    std::string imagesParams[] = {"armorcls", "actionpt", "carryamt", "meleedam", "damresis", "poisnres", "radresis", "sequence", "healrate", "critchnc"};
    unsigned int labels[] = {302, 301, 311, 304, 305, 306, 307, 308, 309, 310};
    const int params[] = {109, 108, 112, 111, 124, 132, 131, 113, 114, 115};
    for (unsigned int i = 0; i != 10; ++i)
    {
        std::stringstream ss;
        ss << "params_" << (i+1);
        _addTitle(ss.str(), msgStat->message(params[i])->text());
        _addDescription(ss.str(), msgStat->message(params[i] + 100)->text());
        _addImage(ss.str(), new Image("art/skilldex/" + imagesParams[i] + ".frm"));
        _addLabel(ss.str(), new TextArea(msgEditor->message(labels[i]), backgroundX+194, backgroundY+182+13*i));
        _addLabel(ss.str() + "_value", new TextArea("", backgroundX+288, backgroundY+182+13*i));
    }

    _addButton("options", new ImageButton(ImageButton::TYPE_SMALL_RED_CIRCLE, backgroundX+345, backgroundY+454));
    _addButton("done",    new ImageButton(ImageButton::TYPE_SMALL_RED_CIRCLE, backgroundX+455, backgroundY+454));
    _addButton("cancel",  new ImageButton(ImageButton::TYPE_SMALL_RED_CIRCLE, backgroundX+554, backgroundY+454));

    auto font3_b89c28ff = ResourceManager::font("font3.aaf", 0xb89c28ff);

    _addLabel("options", new TextArea(msgEditor->message(101), backgroundX+365, backgroundY+453))->setFont(font3_b89c28ff);
    _addLabel("next",    new TextArea(msgEditor->message(100), backgroundX+473, backgroundY+453))->setFont(font3_b89c28ff);
    _addLabel("cancel",  new TextArea(msgEditor->message(102), backgroundX+571, backgroundY+453))->setFont(font3_b89c28ff);
    _addLabel("name",    new TextArea(Game::getInstance()->player()->name(), backgroundX+17, backgroundY+7))->setWidth(150)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_CENTER)->setFont(font3_b89c28ff);
    _addLabel("age",     new TextArea("AGE", backgroundX+163, backgroundY+7))->setFont(font3_b89c28ff);
    _addLabel("gender",  new TextArea(msgEditor->message(Game::getInstance()->player()->gender() == 0 ? 107 : 108), backgroundX+250, backgroundY+7))->setFont(font3_b89c28ff); // 0 -male 1 - female
    _addLabel("label_1", new TextArea(msgEditor->message(116), backgroundX+14, backgroundY+286))->setFont(font3_b89c28ff);  // char points
    _addLabel("label_2", new TextArea(msgEditor->message(139), backgroundX+50, backgroundY+326))->setFont(font3_b89c28ff);  // optinal traits
    _addLabel("label_3", new TextArea(msgEditor->message(117), backgroundX+383, backgroundY+5))->setFont(font3_b89c28ff);   // skills
    _addLabel("label_4", new TextArea(msgEditor->message(138), backgroundX+428, backgroundY+233))->setFont(font3_b89c28ff); // tag skills
    _addTitle("label_1", msgEditor->message(120)->text());
    _addTitle("label_2", msgEditor->message(146)->text());
    _addTitle("label_3", msgEditor->message(150)->text());
    _addTitle("label_4", msgEditor->message(144)->text());
    _addDescription("label_1", msgEditor->message(121)->text());
    _addDescription("label_2", msgEditor->message(147)->text());
    _addDescription("label_3", msgEditor->message(151)->text());
    _addDescription("label_4", msgEditor->message(145)->text());
    _addImage("label_1", new Image("art/skilldex/generic.frm"));
    _addImage("label_2", new Image("art/skilldex/traits.frm"));
    _addImage("label_3", new Image("art/skilldex/skills.frm"));
    _addImage("label_4", new Image("art/skilldex/skills.frm"));

    // Name change button
    _addButton("name",  new ImageButton(ImageButton::TYPE_PLAYER_NAME, backgroundX+13, backgroundY+0));

    // Age change button
    _addButton("age", new ImageButton(ImageButton::TYPE_PLAYER_AGE, backgroundX+156, backgroundY+0));

    //Gender change button
    _addButton("gender", new ImageButton(ImageButton::TYPE_PLAYER_GENDER, backgroundX+236, backgroundY+0));

    // add buttons to the state
    for(auto it = _buttons.begin(); it != _buttons.end(); ++it)
    {
        it->second->addEventHandler("mouseleftclick", this, (EventRecieverMethod) &PlayerCreateState::onButtonClick);
        addUI(it->second);
    }

    // add labels to the state
    // reverse iterator to change drawing order
    for(auto it = _labels.rbegin(); it != _labels.rend(); ++it)
    {
        it->second->setBackgroundColor(0x00000001);
        it->second->addEventHandler("mouseleftclick", this, (EventRecieverMethod) &PlayerCreateState::onLabelClick);
        addUI(it->second);
    }

    // add counters to the state
    for(auto it = _counters.begin(); it != _counters.end(); ++it)
    {
        addUI(it->second);
    }

    // add hidden masks
    for(auto it = _masks.begin(); it != _masks.end(); ++it)
    {
        it->second->addEventHandler("mouseleftclick", this, (EventRecieverMethod) &PlayerCreateState::onMaskClick);
        addUI(it->second);
    }

    // Fix to handle click on empty spaces
    for(auto it = _labels.begin(); it != _labels.end(); ++it)
    {
        it->second->setBackgroundColor(0x00000001);
    }

    _selectedImage = _images.at("stats_1");
    _selectedLabel = _labels.at("stats_1");
    _selectedImage->setX(backgroundX+480);
    _selectedImage->setY(backgroundY+310);
    addUI(_selectedImage);

    auto font1_000000ff = ResourceManager::font("font1.aaf", 0x000000FF);
    auto font2_000000ff = ResourceManager::font("font2.aaf", 0x000000FF);

    _title = new TextArea("", backgroundX+350, backgroundY+275);
    _title->setFont(font2_000000ff);
    addUI(_title);

    auto line = new Image(270, 2);
    line->setX(backgroundX+350);
    line->setY(backgroundY+300);
    line->texture()->fill(0x000000ff);
    addUI(line);

    _description = new TextArea("", backgroundX+350, backgroundY+315);
    _description->setFont(font1_000000ff)->setWidth(140)->setHeight(120)->setWordWrap(true);
    addUI(_description);

}

PlayerCreateState::~PlayerCreateState()
{
}

void PlayerCreateState::init()
{
}

TextArea* PlayerCreateState::_addLabel(std::string name, TextArea* label)
{
    _labels.insert(std::pair<std::string,TextArea*>(name, label));
    return label;
}

ImageButton* PlayerCreateState::_addButton(std::string name, ImageButton* button)
{
    _buttons.insert(std::pair<std::string,ImageButton*>(name, button));
    return button;
}

BigCounter* PlayerCreateState::_addCounter(std::string name, BigCounter* counter)
{
    _counters.insert(std::pair<std::string,BigCounter*>(name, counter));
    return counter;
}

HiddenMask* PlayerCreateState::_addMask(std::string name, HiddenMask* mask)
{
    _masks.insert(std::pair<std::string,HiddenMask*>(name, mask));
    return mask;
}

void PlayerCreateState::_addTitle(std::string name, std::string title)
{
    _titles.insert(std::pair<std::string,std::string>(name, title));
}

void PlayerCreateState::_addDescription(std::string name, std::string description)
{
    _descriptions.insert(std::pair<std::string,std::string>(name, description));
}

void PlayerCreateState::_addImage(std::string name, Image* image)
{
    _images.insert(std::pair<std::string, Image*>(name, image));
}


void PlayerCreateState::think()
{
    State::think();
    auto player = Game::getInstance()->player();
    auto msgEditor = ResourceManager::msgFileType("text/english/game/editor.msg");

    _labels.at("name")->setText(player->name());
    _labels.at("age")->setText(msgEditor->message(104))->appendText(" ")->appendText(std::to_string(player->age()));
    _labels.at("gender")->setText(msgEditor->message(player->gender() == 0 ? 107 : 108)); // 0 - male   1 - female

    _counters.at("statsPoints")->setNumber(player->statsPoints());
    _counters.at("skillsPoints")->setNumber(player->skillsPoints());

    _labels.at("health_1")->setText(msgEditor->message(300))->appendText("  ")->appendText(std::to_string(player->hitPointsMax()))->appendText("/")->appendText(std::to_string(player->hitPointsMax()));
    _labels.at("params_1_value")->setText(player->armorClass());
    _labels.at("params_2_value")->setText(player->actionPoints());
    _labels.at("params_3_value")->setText(player->carryWeightMax());
    _labels.at("params_4_value")->setText(player->meleeDamage());
    _labels.at("params_5_value")->setText(player->damageResistance())->appendText("%");
    _labels.at("params_6_value")->setText(player->poisonResistance())->appendText("%");
    _labels.at("params_7_value")->setText(player->radiationResistance())->appendText("%");
    _labels.at("params_8_value")->setText(player->sequence());
    _labels.at("params_9_value")->setText(player->healingRate());
    _labels.at("params_10_value")->setText(player->criticalChance())->appendText("%");

    // Stats counters and labels
    for (unsigned int i = 0; i < 7; i++)
    {
        std::stringstream ss;
        ss << "stats_" << (i+1);
        unsigned int val = player->statTotal(i);
        _counters.at(ss.str())->setNumber(val);
        _counters.at(ss.str())->setColor(BigCounter::COLOR_WHITE);
        if (val > 10)
        {
            val = 10;
            _counters.at(ss.str())->setColor(BigCounter::COLOR_RED);
        }
        _labels.at(ss.str())->setText(msgEditor->message(199 + (val < 1 ? 1 : val)));
    }

    // Skills values
    for (unsigned int i = 0; i != 18; ++i)
    {
        std::stringstream ss;
        ss << "skills_" << (i + 1) << "_value";
        _labels.at(ss.str())->setText(player->skillValue(i))->appendText("%");
    }

    // Default labels colors
    for(auto it = _labels.begin(); it != _labels.end(); ++it)
    {
        std::string name = it->first;

        auto font1_3ff800ff = ResourceManager::font("font1.aaf", 0x3ff800ff);
        auto font1_a0a0a0ff = ResourceManager::font("font1.aaf", 0xa0a0a0ff);
        auto font1_183018ff = ResourceManager::font("font1.aaf", 0x183018ff);

        if (name.find("stats_") == 0 || name.find("params_") == 0)
        {
            it->second->setFont(font1_3ff800ff);
        }

        if (name.find("traits_") == 0)
        {
            unsigned int number = atoi(name.substr(7).c_str());
            it->second->setFont(player->trait(number - 1) ? font1_a0a0a0ff : font1_3ff800ff);
        }

        if (name.find("skills_") == 0)
        {
            unsigned int number = atoi(name.substr(7).c_str());
            it->second->setFont(player->skill(number - 1) ? font1_a0a0a0ff : font1_3ff800ff);
        }

        if (name.find("health_") == 0)
        {
            it->second->setFont(name.compare("health_1") == 0 ? font1_3ff800ff : font1_183018ff);
        }
    }

    // Selected labels colors
    for(auto it = _labels.begin(); it != _labels.end(); ++it)
    {
        if (_selectedLabel != it->second) continue;

        std::string name = it->first;

        _title->setText(_titles.at(name));
        _description->setText(_descriptions.at(name));
        _selectedImage->setTexture(_images.at(name)->texture());

        auto font1_ffff7fff = ResourceManager::font("font1.aaf", 0xffff7fff);
        auto font1_ffffffff = ResourceManager::font("font1.aaf", 0xffffffff);
        auto font1_707820ff = ResourceManager::font("font1.aaf", 0x707820ff);

        if (name.find("stats_") == 0)
        {
            it->second->setFont(font1_ffff7fff);
        }

        if (name.find("params_") == 0)
        {
            it->second->setFont(font1_ffff7fff);
            _labels.at(name+"_value")->setFont(font1_ffff7fff);
        }

        if (name.find("traits_") == 0)
        {
            unsigned int number = atoi(name.substr(7).c_str());
            it->second->setFont(player->trait(number - 1) ? font1_ffffffff : font1_ffff7fff);
        }

        if (name.find("skills_") == 0)
        {
            unsigned int number = atoi(name.substr(7).c_str());
            it->second->setFont(player->skill(number - 1) ? font1_ffffffff : font1_ffff7fff);
            _labels.at(name+"_value")->setFont(player->skill(number - 1) ? font1_ffffffff : font1_ffff7fff);
        }

        if (name.find("health_") == 0)
        {
            it->second->setFont(name.compare("health_1") == 0 ? font1_ffff7fff : font1_707820ff);
        }
    }

}

bool PlayerCreateState::_statDecrease(unsigned int num)
{
    auto player = Game::getInstance()->player();
    if (player->stat(num) <= 1) return false;

    player->setStat(num, player->stat(num) - 1);
    player->setStatsPoints(player->statsPoints() + 1);
    return true;
}

bool PlayerCreateState::_statIncrease(unsigned int num)
{
    auto player = Game::getInstance()->player();
    if (player->statsPoints() <= 0) return false;

    if (player->stat(num) + player->statBonus(num) >= 10) return false;

    player->setStat(num, player->stat(num) + 1);
    player->setStatsPoints(player->statsPoints() - 1);
    return true;
}

bool PlayerCreateState::_traitToggle(unsigned int num)
{
    auto player = Game::getInstance()->player();
    if (player->trait(num))
    {
        player->setTrait(num, 0);
        return true;
    }
    else
    {
        unsigned int selectedTraits = 0;
        for (unsigned int i = 0; i != 16; ++i) if (player->trait(i)) selectedTraits++;
        if (selectedTraits < 2)
        {
            player->setTrait(num, 1);
            return true;
        }
    }
    return false;
}

bool PlayerCreateState::_skillToggle(unsigned int num)
{
    auto player = Game::getInstance()->player();
    if (player->skill(num))
    {
        player->setSkill(num, 0);
        player->setSkillsPoints(player->skillsPoints() + 1);
        return true;
    }
    else
    {
        if (player->skillsPoints() > 0)
        {
            player->setSkill(num, 1);
            player->setSkillsPoints(player->skillsPoints() - 1);
            return true;
        }
    }
    return false;
}

void PlayerCreateState::onButtonClick(std::shared_ptr<MouseEvent> event)
{
    auto sender = dynamic_cast<ImageButton*>(event->emitter());
    auto state = dynamic_cast<PlayerCreateState*>(event->reciever());

    for(auto it = state->_buttons.begin(); it != state->_buttons.end(); ++it)
    {
        if (it->second == sender)
        {
            std::string name = it->first;

            if (name == "name") return onNameButtonClick(event);
            if (name == "age")  return onAgeButtonClick(event);
            if (name == "gender") return onGenderButtonClick(event);
            if (name == "cancel") return onBackButtonClick(event);
            if (name == "done") return onDoneButtonClick(event);

            if (name.find("stats_") == 0)
            {
                state->_selectedLabel = state->_labels.at(name.substr(0,7));
                state->_selectedImage->setTexture(state->_images.at(name.substr(0,7))->texture());
                unsigned int number = atoi(name.substr(6,1).c_str());
                if (name.find("_increase") == 7)
                {
                    _statIncrease(number - 1);
                }
                else
                {
                    _statDecrease(number - 1);
                }
            }

            if (name.find("traits_") == 0)
            {
                unsigned int number = atoi(name.substr(7).c_str());
                state->_selectedLabel = state->_labels.at(name);
                state->_selectedImage->setTexture(state->_images.at(name)->texture());

                if (!_traitToggle(number - 1))
                {
                    auto state = new PlayerEditAlertState();
                    auto msg = ResourceManager::msgFileType("text/english/game/editor.msg");
                    std::string text = msg->message(148)->text() + "\n" + msg->message(149)->text();
                    state->setMessage(text);
                    Game::getInstance()->pushState(state);
                }
            }

            if (name.find("skills_") == 0)
            {
                unsigned int number = atoi(name.substr(7).c_str());
                state->_selectedLabel = state->_labels.at(name);
                state->_selectedImage->setTexture(state->_images.at(name)->texture());
                if (!_skillToggle(number - 1))
                {
                    auto state = new PlayerEditAlertState();
                    auto msg = ResourceManager::msgFileType("text/english/game/editor.msg");
                    std::string text = msg->message(140)->text() + "\n" + msg->message(141)->text();
                    state->setMessage(text);
                    Game::getInstance()->pushState(state);
                }
            }
        }
    }
}

void PlayerCreateState::onLabelClick(std::shared_ptr<MouseEvent> event)
{
    auto state = dynamic_cast<PlayerCreateState*>(event->reciever());

    for(auto it = state->_labels.begin(); it != state->_labels.end(); ++it)
    {
        std::string name = it->first;
        if (it->second == event->emitter())
        {
            if (name.find("stats_") == 0 || name.find("traits_") == 0 || name.find("skills_") == 0 || name.find("health_") == 0 || name.find("params_") == 0 || name.find("label_") == 0)
            {
                std::string label = name;
                if (name.find("_value") > 0)
                {
                    label = name.substr(0, name.find("_value"));
                }
                state->_selectedLabel = state->_labels.at(label.c_str());
                state->_selectedImage->setTexture(state->_images.at(label.c_str())->texture());
            }
        }
    }
}

void PlayerCreateState::onMaskClick(std::shared_ptr<MouseEvent> event)
{
    auto state = dynamic_cast<PlayerCreateState*>(event->reciever());

    for(auto it = state->_masks.begin(); it != state->_masks.end(); ++it)
    {
        if (it->second == event->emitter())
        {
            std::string name = it->first;
            if (name.find("stats_") == 0)
            {
                state->_selectedLabel = state->_labels.at(name);
                state->_selectedImage->setTexture(state->_images.at(name)->texture());
            }
        }
    }
}


void PlayerCreateState::onNameButtonClick(std::shared_ptr<MouseEvent> event)
{
    Game::getInstance()->pushState(new PlayerEditNameState());
}

void PlayerCreateState::onAgeButtonClick(std::shared_ptr<MouseEvent> event)
{
    Game::getInstance()->pushState(new PlayerEditAgeState());
}

void PlayerCreateState::onGenderButtonClick(std::shared_ptr<MouseEvent> event)
{
    Game::getInstance()->pushState(new PlayerEditGenderState());
}

void PlayerCreateState::onBackButtonClick(std::shared_ptr<MouseEvent> event)
{
    Game::getInstance()->popState();
}

void PlayerCreateState::onDoneButtonClick(std::shared_ptr<MouseEvent> event)
{
    auto player = Game::getInstance()->player();
    player->setHitPoints(player->hitPointsMax());
    Game::getInstance()->setState(new LocationState());
}

}
