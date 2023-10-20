#include <citro2d.h>
#include <3ds.h>

#include "Phase.h"
#include "CircleGamePhase.h"
#include "CircleGameTrialPhase.h"

const int CircleGamePhase::WIDTH_SCREEN_UPPER = 400;
const int CircleGamePhase::HEIGHT_SCREEN_UPPER = 240;

const int CircleGamePhase::POSITION_SCREEN_CENTER_X = CircleGamePhase::WIDTH_SCREEN_UPPER / 2;
const int CircleGamePhase::POSITION_SCREEN_CENTER_Y = CircleGamePhase::HEIGHT_SCREEN_UPPER / 2;

const int CircleGamePhase::SIZE_BG_BUTTON = 100;
const int CircleGamePhase::SIZE_FG_CIRCLE = 50;

const int CircleGamePhase::POSITION_FG_CIRCLE_CENTER_OFFSET = 70;

const int CircleGamePhase::POSITIONS_FG_CIRCLE_BASE_X[4] = {
    CircleGamePhase::POSITION_SCREEN_CENTER_X,
    CircleGamePhase::POSITION_SCREEN_CENTER_X + CircleGamePhase::POSITION_FG_CIRCLE_CENTER_OFFSET,
    CircleGamePhase::POSITION_SCREEN_CENTER_X,
    CircleGamePhase::POSITION_SCREEN_CENTER_X - CircleGamePhase::POSITION_FG_CIRCLE_CENTER_OFFSET,
};

const int CircleGamePhase::POSITIONS_FG_CIRCLE_BASE_Y[4] = {
    CircleGamePhase::POSITION_SCREEN_CENTER_Y - CircleGamePhase::POSITION_FG_CIRCLE_CENTER_OFFSET,
    CircleGamePhase::POSITION_SCREEN_CENTER_Y,
    CircleGamePhase::POSITION_SCREEN_CENTER_Y + CircleGamePhase::POSITION_FG_CIRCLE_CENTER_OFFSET,
    CircleGamePhase::POSITION_SCREEN_CENTER_Y,
};

const int CircleGamePhase::DIRECTIONS_KEYS[4] = {
    KEY_X,
    KEY_A,
    KEY_B,
    KEY_Y,
};

// Per-eye offset
const int CircleGamePhase::OFFSETS[30] = {
    5, 5, 5, 5, 5, 5,
    4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1,
};

void CircleGamePhase::enter() {

    // Create targets for both eyes on the top screen
	this->left = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	this->right = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);
    
    // Load fg_circle
	this->sheet_fg_circle = C2D_SpriteSheetLoad("romfs:/gfx/fg_circle.t3x");	
	this->image_fg_circle = C2D_SpriteSheetGetImage(sheet_fg_circle, 0);

	// Lead bg_buttons
	this->sheet_bg_button = C2D_SpriteSheetLoad("romfs:/gfx/bg_button.t3x");
	this->image_bg_button = C2D_SpriteSheetGetImage(sheet_bg_button, 0);

    this->trial = new CircleGameTrialPhase(
        this->left, this->right, this->image_fg_circle, this->image_bg_button, 0
    );
    this->trial->enter();
}

Phase* CircleGamePhase::update() {

    Phase* resultPhase = this->trial->update();
    if (resultPhase == this->trial) return this;

    this->trial->exit();
    delete this->trial;

    this->trial = resultPhase;
    if (this->trial == nullptr) return nullptr;

    this->trial->enter();

    return this;
}

void CircleGamePhase::exit() {

    if (this->trial != nullptr) {
        this->trial->exit();
        delete this->trial;
    }

	C2D_SpriteSheetFree(this->sheet_fg_circle);
	C2D_SpriteSheetFree(this->sheet_bg_button);
}
