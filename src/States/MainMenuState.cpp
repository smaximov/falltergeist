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
#include "../Engine/Game.h"
#include "../Engine/ResourceManager.h"
#include "../Engine/Graphics/Animation.h"
#include "../Engine/Input/Mouse.h"
#include "../UI/Image.h"
#include "../UI/ImageButton.h"
#include "../UI/TextArea.h"
#include "../States/MainMenuState.h"
#include "../States/SettingsMenuState.h"
#include "../States/NewGameState.h"
#include "../Engine/Graphics/Texture.h"
#include "../Engine/Font.h"

// Third party includes

namespace Falltergeist
{

MainMenuState::MainMenuState() : State()
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::init()
{
    State::init();

    //_game->mouse()->setType(Mouse::BIG_ARROW);

    // Background image
    auto background = new Image("art/intrface/mainmenu.frm");

    // "Into" button
    auto introButton = new ImageButton(ImageButton::TYPE_BIG_RED_CIRCLE, 29, 19);

    // "New game" button
    auto newGameButton = new ImageButton(ImageButton::TYPE_BIG_RED_CIRCLE, 29, 19 + 41);
    newGameButton->addEventHandler("mouseleftclick", this, (EventRecieverMethod)&MainMenuState::onNewGameButtonClick);

    // "Load game" button
    auto loadGameButton = new ImageButton(ImageButton::TYPE_BIG_RED_CIRCLE, 29, 19 + 41*2);

    // "Settings" button
    auto settingsButton = new ImageButton(ImageButton::TYPE_BIG_RED_CIRCLE, 29, 19 + 41*3);
    settingsButton->addEventHandler("mouseleftclick", this, (EventRecieverMethod) &MainMenuState::onSettingsButtonClick);

    // "Credits" button
    auto creditsButton = new ImageButton(ImageButton::TYPE_BIG_RED_CIRCLE, 29, 19 + 41*4);
    
    // "Exit" button
    auto exitButton = new ImageButton(ImageButton::TYPE_BIG_RED_CIRCLE, 29, 19 + 41*5);
    exitButton->addEventHandler("mouseleftclick", this, (EventRecieverMethod) &MainMenuState::onExitButtonClick);


    auto msg = _game->resourceManager()->msgFileType("text/english/game/misc.msg");

    auto font4 = _game->resourceManager()->font("font4.aaf", 0xb89c28ff);

    // "Intro" label
    auto introButtonLabel = new TextArea(msg->message(9), 55, 20);
    introButtonLabel->setFont(font4)->setWidth(150)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_CENTER);

    // "New Game" label
    auto newGameButtonLabel = new TextArea(msg->message(10), 50, 20 + 41);
    newGameButtonLabel->setFont(font4)->setWidth(150)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_CENTER);

    // "Load Game" label
    auto loadGameButtonLabel = new TextArea(msg->message(11), 50, 20 + 41*2);
    loadGameButtonLabel->setFont(font4)->setWidth(150)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_CENTER);

    // "Options" label
    auto optionsButtonLabel = new TextArea(msg->message(12), 50, 20 + 41*3);
    optionsButtonLabel->setFont(font4)->setWidth(150)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_CENTER);

    // "Credits" label
    auto creditsButtonLabel = new TextArea(msg->message(13), 50, 20 + 41*4);
    creditsButtonLabel->setFont(font4)->setWidth(150)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_CENTER);

    // "Exit" label
    auto exitButtonLabel = new TextArea(msg->message(14), 50, 20 + 41*5);
    exitButtonLabel->setFont(font4)->setWidth(150)->setHorizontalAlign(TextArea::HORIZONTAL_ALIGN_CENTER);

    auto testRectangle = new Image(320, 200);
    testRectangle->setX(160);
    testRectangle->setY(140);
    testRectangle->texture()->fill(0xddddddff);

    auto testTextArea = new TextArea(170, 150);
    testTextArea->setWidth(300);
    testTextArea->setHeight(180);
    testTextArea->setWordWrap(true);

    auto testFont = new Font("font2.aaf", 0xff00ffff);
    testTextArea->setFont(testFont);

    testTextArea->setText("On the other hand, we denounce with righteous indignation and dislike men who are so beguiled and demoralized by the charms of pleasure of the moment, so blinded by desire, that they cannot foresee the pain and trouble that are bound to ensue; and equal blame belongs to those who fail in their duty through weakness of will, which is the same as saying through shrinking from toil and pain. These cases are perfectly simple and easy to distinguish. In a free hour, when our power of choice is untrammelled and when nothing prevents our being able to do what we like best, every pleasure is to be welcomed and every pain avoided. But in certain circumstances and owing to the claims of duty or the obligations of business it will frequently occur that pleasures have to be repudiated and annoyances accepted. The wise man therefore always holds in these matters to this principle of selection: he rejects pleasures to secure other greater pleasures, or else he endures pains to avoid worse pains.");



    add(background);

    // buttons
    add(introButton);

    add(newGameButton);
    add(loadGameButton);
    add(settingsButton);
    add(creditsButton);
    add(exitButton);

    // text areas
    add(introButtonLabel);
    add(newGameButtonLabel);
    add(loadGameButtonLabel);
    add(optionsButtonLabel);
    add(creditsButtonLabel);
    add(exitButtonLabel);

    add(testRectangle);
    add(testTextArea);
}

void MainMenuState::think()
{
}

void MainMenuState::onExitButtonClick(MouseEvent* event)
{
    _game->quit();
}

void MainMenuState::onNewGameButtonClick(MouseEvent* event)
{
    _game->pushState(new NewGameState());
}

void MainMenuState::onSettingsButtonClick(MouseEvent* event)
{
    _game->pushState(new SettingsMenuState());
}


}
