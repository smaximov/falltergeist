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
 *
 */

// C++ standard includes
#include <sstream>

// Falltergeist includes
#include "../Engine/Game.h"
#include "../Engine/Graphics/Renderer.h"
#include "../Engine/Input/Mouse.h"
#include "../Engine/ResourceManager.h"
#include "../Engine/State.h"
#include "../Game/GameArmorItemObject.h"
#include "../Game/GameCritterObject.h"
#include "../Game/GameDudeObject.h"
#include "../Game/GameObject.h"
#include "../Game/GameObjectFactory.h"
#include "../Game/GameWeaponItemObject.h"
#include "../States/GameMenuState.h"
#include "../States/InventoryDragItemState.h"
#include "../States/InventoryState.h"
#include "../UI/Image.h"
#include "../UI/ImageButton.h"
#include "../UI/TextArea.h"
#include "../UI/ImageList.h"

// Third party includes

namespace Falltergeist
{

InventoryState::InventoryState() : State()
{
}

InventoryState::~InventoryState()
{
}

void InventoryState::init()
{
    if (_initialized) return;
    State::init();

    setModal(true);
    setFullscreen(false);

    auto game = Game::getInstance();

    setX((game->renderer()->width()  - 499)*0.5);
    setY((game->renderer()->height() - 427)*0.5);

    addUI("background", new Image("art/intrface/invbox.frm"));
    getActiveUI("background")->addEventHandler("mouserightclick", this, (EventRecieverMethod) &InventoryState::backgroundRightClick);

    addUI("button_up",   new ImageButton(ImageButton::TYPE_INVENTORY_UP_ARROW,   128, 40));
    addUI("button_down", new ImageButton(ImageButton::TYPE_INVENTORY_DOWN_ARROW, 128, 65));

    addUI("button_done", new ImageButton(ImageButton::TYPE_SMALL_RED_CIRCLE, 438, 328));
    getActiveUI("button_done")->addEventHandler("mouseleftclick", this, (EventRecieverMethod) &InventoryState::onDoneButtonClick);

    // screen
    auto screenX = 300;
    auto screenY = 47;

    auto player = Game::getInstance()->player();

    addUI("player_name", new TextArea(player->name(), screenX, screenY));

    auto line1 = new Image(142, 1);
    line1->setX(screenX);
    line1->setY(screenY+16);
    line1->texture()->fill(0x3ff800ff); // default green color

    auto msg = ResourceManager::msgFileType("text/english/game/inventry.msg");

    std::string statsLabels;
    for (unsigned int i = 0; i != 7; ++i) statsLabels += msg->message(i)->text() + "\n";
    addUI("label_stats", new TextArea(statsLabels, screenX, screenY + 10*2));

    std::string statsValues;
    for (unsigned int i = 0; i != 7; ++i) statsValues += std::to_string(player->stat(i)) + "\n";
    addUI("label_stats_values", new TextArea(statsValues, screenX + 22, screenY + 20));

    std::stringstream ss;
    for (unsigned int i=7; i<14; i++)
    {
        ss << msg->message(i)->text() << "\n";
    }
    auto textLabel = new TextArea(ss.str(), screenX+40, screenY+20);

    // label: hit points
    ss.str("");
    ss << player->hitPoints();
    ss << "/";
    ss << player->hitPointsMax();
    auto hitPointsLabel = new TextArea(ss.str(), screenX+94, screenY+20);
    hitPointsLabel->setWidth(46)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_RIGHT);
    // label: armor class
    ss.str("");
    ss << player->armorClass();
    auto armorClassLabel = new TextArea(ss.str(), screenX+94, screenY+30);
    armorClassLabel->setWidth(46)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_RIGHT);

    // armorSlot, leftHand, rightHand
    GameArmorItemObject* armorSlot = player->armorSlot();
    GameItemObject* leftHand = player->leftHandSlot();
    GameItemObject* rightHand = player->rightHandSlot();


    // label: damage treshold levels
    ss.str("");
    if (armorSlot)
    {
        ss << player->damageThreshold(GameCritterObject::DAMAGE_NORMAL) + armorSlot->damageThreshold(GameArmorItemObject::DAMAGE_NORMAL) <<"/\n";
        ss << player->damageThreshold(GameCritterObject::DAMAGE_LASER) + armorSlot->damageThreshold(GameArmorItemObject::DAMAGE_LASER) <<"/\n";
        ss << player->damageThreshold(GameCritterObject::DAMAGE_FIRE) + armorSlot->damageThreshold(GameArmorItemObject::DAMAGE_FIRE) <<"/\n";
        ss << player->damageThreshold(GameCritterObject::DAMAGE_PLASMA) + armorSlot->damageThreshold(GameArmorItemObject::DAMAGE_PLASMA) <<"/\n";
        ss << player->damageThreshold(GameCritterObject::DAMAGE_EXPLOSION) + armorSlot->damageThreshold(GameArmorItemObject::DAMAGE_NORMAL) <<"/";
    }
    else
    {
        ss << player->damageThreshold(GameCritterObject::DAMAGE_NORMAL) <<"/\n";
        ss << player->damageThreshold(GameCritterObject::DAMAGE_LASER) <<"/\n";
        ss << player->damageThreshold(GameCritterObject::DAMAGE_FIRE) <<"/\n";
        ss << player->damageThreshold(GameCritterObject::DAMAGE_PLASMA) <<"/\n";
        ss << player->damageThreshold(GameCritterObject::DAMAGE_EXPLOSION) <<"/";
    }
    auto damageThresholdLabel = new TextArea(ss.str(), screenX+94, screenY+40);
    damageThresholdLabel->setWidth(26)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_RIGHT);

    // label: damage resistance levels
    ss.str("");
    if (armorSlot)
    {
        ss << player->damageResist(GameCritterObject::DAMAGE_NORMAL) + armorSlot->damageResist(GameArmorItemObject::DAMAGE_NORMAL) <<"%\n";
        ss << player->damageResist(GameCritterObject::DAMAGE_LASER) + armorSlot->damageResist(GameArmorItemObject::DAMAGE_LASER) <<"%\n";
        ss << player->damageResist(GameCritterObject::DAMAGE_FIRE) + armorSlot->damageResist(GameArmorItemObject::DAMAGE_FIRE) <<"%\n";
        ss << player->damageResist(GameCritterObject::DAMAGE_PLASMA) + armorSlot->damageResist(GameArmorItemObject::DAMAGE_PLASMA) <<"%\n";
        ss << player->damageResist(GameCritterObject::DAMAGE_EXPLOSION) + armorSlot->damageResist(GameArmorItemObject::DAMAGE_NORMAL) <<"%";
    }
    else
    {
        ss << player->damageResist(GameCritterObject::DAMAGE_NORMAL) <<"%\n";
        ss << player->damageResist(GameCritterObject::DAMAGE_LASER) <<"%\n";
        ss << player->damageResist(GameCritterObject::DAMAGE_FIRE) <<"%\n";
        ss << player->damageResist(GameCritterObject::DAMAGE_PLASMA) <<"%\n";
        ss << player->damageResist(GameCritterObject::DAMAGE_EXPLOSION) <<"%";
    }
    auto damageResistanceLabel = new TextArea(ss.str(), screenX+120, screenY+40);

    auto line2 = new Image(142, 1);
    line2->setX(screenX);
    line2->setY(screenY+94);
    line2->texture()->fill(0x3ff800ff); // default green color

    auto line3 = new Image(142, 1);
    line3->setX(screenX);
    line3->setY(screenY+134);
    line3->texture()->fill(0x3ff800ff); // default green color

    // label: Total Wt: (20)
    auto weight = player->carryWeight();
    auto weightMax = player->carryWeightMax();

    ss.str("");
    ss << weight;
    auto totalWtLabel = new TextArea(msg->message(20), screenX+14, screenY+180);
    auto weightLabel = new TextArea(ss.str(), screenX+70, screenY+180);
    weightLabel->setWidth(24)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_RIGHT);
    ss.str("");
    ss << "/" << weightMax;
    auto weightMaxLabel = new TextArea(ss.str(), screenX+94, screenY+180);
    if (weight>weightMax)
    {
        weightLabel->setFont(ResourceManager::font("font1.aaf", 0xff0000ff));
    }

    // label: left hand
    ss.str("");
    if (leftHand)
    {
        ss << _handItemSummary(leftHand);
    }
    auto leftHandLabel = new TextArea(ss.str(), screenX, screenY+100);

    // label: right hand
    ss.str("");
    if (rightHand)
    {
        ss << _handItemSummary(rightHand);
    }
    auto rightHandLabel = new TextArea(ss.str(), screenX, screenY+140);

    // screen info
    auto screenLabel = new TextArea("", screenX, screenY+20);
    screenLabel->setWidth(140); //screen size
    screenLabel->setHeight(168);
    screenLabel->setVisible(false);
    screenLabel->setWordWrap(true);


    addUI(line1);
    addUI("textLabel", textLabel);
    addUI("hitPointsLabel", hitPointsLabel);
    addUI("armorClassLabel", armorClassLabel);
    addUI("damageThresholdLabel", damageThresholdLabel);
    addUI("damageResistanceLabel", damageResistanceLabel);
    addUI("line2", line2);
    addUI("line3", line3);
    addUI("totalWtLabel", totalWtLabel);
    addUI("weightLabel", weightLabel);
    addUI("weightMaxLabel", weightMaxLabel);
    addUI("leftHandLabel", leftHandLabel);
    addUI("rightHandLabel", rightHandLabel);
    addUI("screenLabel", screenLabel);


    // BIG ICONS
    // icon: armor
    if (armorSlot)
    {
        auto armorUi = new ImageList();
        armorUi->addImage(new Image(armorSlot->inventorySlotUi()));
        armorUi->addImage(new Image(armorSlot->inventoryDragUi()));
        armorUi->setX(200 - armorUi->width()*0.5);
        armorUi->setY(215 - armorUi->height()*0.5);
        addUI(armorUi);

        armorUi->addEventHandler("mouseleftdown", this, (EventRecieverMethod) &InventoryState::onArmorSlotMouseDown);
        //armorUi->addEventHandler("mouseleftup", this, (EventRecieverMethod) &InventoryState::onArmorSlotMouseUp);
        //armorUi->addEventHandler("mousedrag", armorSlot.get(), (EventRecieverMethod) &InventoryState::onSlotDrag);
        //armorUi->addEventHandler("mouseleftdown", armorSlot.get(), (EventRecieverMethod) &InventoryState::onSlotMouseDown);
        //armorUi->addEventHandler("mouseleftup", armorSlot.get(), (EventRecieverMethod) &InventoryState::onSlotMouseUp);
    }

    // icon: left hand
    if (leftHand)
    {
        auto leftHandUi = new ImageList();
        leftHandUi->addImage(new Image(leftHand->inventorySlotUi()));
        leftHandUi->addImage(new Image(leftHand->inventoryDragUi()));
        leftHandUi->setX(200 - leftHandUi->width()*0.5);
        leftHandUi->setY(317 - leftHandUi->height()*0.5);
        addUI(leftHandUi);

        leftHandUi->addEventHandler("mouseleftdown", this, (EventRecieverMethod) &InventoryState::onLeftHandSlotMouseDown);
        //leftHandUi->addEventHandler("mousedrag", leftHand.get(), (EventRecieverMethod) &InventoryState::onSlotDrag);
        //leftHandUi->addEventHandler("mouseleftdown", leftHand.get(), (EventRecieverMethod) &InventoryState::onSlotMouseDown);
        //leftHandUi->addEventHandler("mouseleftup", leftHand.get(), (EventRecieverMethod) &InventoryState::onSlotMouseUp);
    }

    // icon: right hand
    if (rightHand)
    {
        auto rightHandUi = new ImageList();
        rightHandUi->addImage(new Image(rightHand->inventorySlotUi()));
        rightHandUi->addImage(new Image(rightHand->inventoryDragUi()));
        rightHandUi->setX(290 - rightHandUi->width()*0.5);
        rightHandUi->setY(317 - rightHandUi->height()*0.5);
        addUI(rightHandUi);

        rightHandUi->addEventHandler("mouseleftdown", this, (EventRecieverMethod) &InventoryState::onRightHandSlotMouseDown);
        //rightHandUi->addEventHandler("mousedrag", rightHand.get(), (EventRecieverMethod) &InventoryState::onSlotDrag);
        //rightHandUi->addEventHandler("mouseleftdown", rightHand.get(), (EventRecieverMethod) &InventoryState::onSlotMouseDown);
        //rightHandUi->addEventHandler("mouseleftup", rightHand.get(), (EventRecieverMethod) &InventoryState::onSlotMouseUp);
    }

}

void InventoryState::onDoneButtonClick(std::shared_ptr<MouseEvent> event)
{
    Game::getInstance()->popState();
}

void InventoryState::onArmorSlotMouseDown(std::shared_ptr<MouseEvent> event)
{
    auto state = dynamic_cast<InventoryState*>(event->reciever());
    if (Game::getInstance()->mouse()->type() == Mouse::HAND)
    {
        auto itemUi = dynamic_cast<ImageList*>(event->emitter());
        Game::getInstance()->pushState(new InventoryDragItemState(itemUi));
    }
    else
    {
        auto itemPID = Game::getInstance()->player()->armorSlot()->PID();
        state->_screenShow(itemPID);
    }
}

void InventoryState::onLeftHandSlotMouseDown(std::shared_ptr<MouseEvent> event)
{
    auto state = dynamic_cast<InventoryState*>(event->reciever());
    if (Game::getInstance()->mouse()->type() == Mouse::HAND)
    {
        auto itemUi = dynamic_cast<ImageList*>(event->emitter());
        Game::getInstance()->pushState(new InventoryDragItemState(itemUi));
    }
    else
    {
        auto itemPID = Game::getInstance()->player()->leftHandSlot()->PID();
        state->_screenShow(itemPID);
    }
}

void InventoryState::onRightHandSlotMouseDown(std::shared_ptr<MouseEvent> event)
{
    auto state = dynamic_cast<InventoryState*>(event->reciever());
    if (Game::getInstance()->mouse()->type() == Mouse::HAND)
    {
        auto itemUi = dynamic_cast<ImageList*>(event->emitter());
        Game::getInstance()->pushState(new InventoryDragItemState(itemUi));
    }
    else
    {
        auto itemPID = Game::getInstance()->player()->rightHandSlot()->PID();
        state->_screenShow(itemPID);
    }
}

//void InventoryState::onSlotMouseDown(std::shared_ptr<MouseEvent> event)
//{
//    auto state = dynamic_cast<InventoryState*>(event->reciever());
//    auto itemUi = dynamic_cast<ImageList*>(event->emitter());
//    itemUi->setCurrentImage(1);
//    itemUi->setX(event->x() - itemUi->width()*0.5);
//    itemUi->setY(event->y() - itemUi->height()*0.5);
//}

//void InventoryState::onSlotMouseUp(std::shared_ptr<MouseEvent> event)
//{
//    auto itemUi = dynamic_cast<ImageList*>(event->emitter());
//    itemUi->setCurrentImage(0);
//    itemUi->setX(event->x() - itemUi->width()*0.5);
//    itemUi->setY(event->y() - itemUi->height()*0.5);
//}

//void InventoryState::onSlotDrag(std::shared_ptr<MouseEvent> event)
//{
//    //auto item = dynamic_cast<GameItemObject*>(event->reciever());
//    auto itemUi = dynamic_cast<ImageList*>(event->emitter());
//    //auto dragUi = item->inventoryDragUi();
//    itemUi->setX(itemUi->x() + event->xOffset());
//    itemUi->setY(itemUi->y() + event->yOffset());
//    //Game::getInstance()->states()->back()->ui()->push_back(dragUi);
//}

std::string InventoryState::_handItemSummary (GameItemObject* hand)
{
    std::stringstream ss;
    if (hand)
    {
        ss << hand->name() << "\n";
        // is it weapon
        if (hand->subtype() == GameObject::TYPE_ITEM_WEAPON)
        {
            auto weapon = dynamic_cast<GameWeaponItemObject*>(hand);
            ss << "Dmg: " << weapon->damageMin() << "-" << weapon->damageMax() << " ";
            ss << "Rng: " << weapon->rangePrimary();
            // has it ammo?
            if (weapon->ammoType() != 0)
            {
                ss << "\nAmmo: /" << weapon->ammoCapacity() << " ";
                auto ammo = GameObjectFactory::createObject(weapon->ammoPID());
                ss << ammo->name();
            }
        }
    }
    else
    {
        ss << "No item\nUnarmed dmg:";
    }
    return ss.str();
}

void InventoryState::backgroundRightClick(std::shared_ptr<MouseEvent> event)
{
    auto state = dynamic_cast<InventoryState*>(event->reciever());
    auto mouse = Game::getInstance()->mouse();
    if (mouse->type() == Mouse::ACTION)
    {
        mouse->setType(Mouse::HAND);
    }
    else
    {
        mouse->setType(Mouse::ACTION);
        //state->_screenShow(1);
    }
    state->_screenShow(0);
}

void InventoryState::_screenShow (unsigned int PID)
{
    auto player = Game::getInstance()->player();
    auto playerNameLabel = getTextArea("player_name");
    auto statsLabel = getTextArea("label_stats");
    auto statsValuesLabel = getTextArea("label_stats_values");
    auto textLabel = getTextArea("textLabel");
    auto hitPointsLabel = getTextArea("hitPointsLabel");
    auto armorClassLabel = getTextArea("armorClassLabel");
    auto damageThresholdLabel = getTextArea("damageThresholdLabel");
    auto damageResistanceLabel = getTextArea("damageResistanceLabel");
    auto line2 = dynamic_cast<Image*>(getUI("line2"));
    auto line3 = dynamic_cast<Image*>(getUI("line3"));
    auto totalWtLabel = getTextArea("totalWtLabel");
    auto weightLabel = getTextArea("weightLabel");
    auto weightMaxLabel = getTextArea("weightMaxLabel");
    auto leftHandLabel = getTextArea("leftHandLabel");
    auto rightHandLabel = getTextArea("rightHandLabel");
    auto screenLabel = getTextArea("screenLabel");

    if (PID == 0)
    {
        playerNameLabel->setText(player->name());
        screenLabel->setVisible(false);
        statsLabel->setVisible(true);
        statsValuesLabel->setVisible(true);
        textLabel->setVisible(true);
        hitPointsLabel->setVisible(true);
        armorClassLabel->setVisible(true);
        damageThresholdLabel->setVisible(true);
        damageResistanceLabel->setVisible(true);
        line2->setVisible(true);
        line3->setVisible(true);
        totalWtLabel->setVisible(true);
        weightLabel->setVisible(true);
        weightMaxLabel->setVisible(true);
        leftHandLabel->setVisible(true);
        rightHandLabel->setVisible(true);
    }
    else
    {
        auto msg = ResourceManager::msgFileType("text/english/game/pro_item.msg");
        playerNameLabel->setText(msg->message(PID*100)->text()); // item name
        screenLabel->setText(msg->message(PID*100+1)->text()); // item dedcription
        screenLabel->setVisible(true);
        statsLabel->setVisible(false);
        statsValuesLabel->setVisible(false);
        textLabel->setVisible(false);
        hitPointsLabel->setVisible(false);
        armorClassLabel->setVisible(false);
        damageThresholdLabel->setVisible(false);
        damageResistanceLabel->setVisible(false);
        line2->setVisible(false);
        line3->setVisible(false);
        totalWtLabel->setVisible(false);
        weightLabel->setVisible(false);
        weightMaxLabel->setVisible(false);
        leftHandLabel->setVisible(false);
        rightHandLabel->setVisible(false);
    }
}

}

