#include <citro2d.h>
#include <3ds.h>

#include "Phase.h"
#include "CircleGamePhase.h"

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

	// Per-eye offset
	this->offset = 0;

	// which raised circle
	this->index_raised = 0;
}

Phase* CircleGamePhase::update() {
    // Handle user input
    hidScanInput();
    int keysD = hidKeysDown();
    int keysH = hidKeysHeld();

    if (keysD & KEY_START) return nullptr;

    if (keysD & KEY_DRIGHT || keysH & KEY_DUP) this->offset++;
    if (keysD & KEY_DLEFT || keysH & KEY_DDOWN) this->offset--;

    for (int i = 0; i < 4; i++) {
        if (keysD & DIRECTIONS_KEYS[i]) {
            this->index_raised = i;
            break;
        }
    }

    float slider = osGet3DSliderState();

    // Print useful information
    printf("\x1b[3;1H %.2f | %i | %i          \n", slider, this->offset, this->index_raised);

    // Render the scene
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

    // Render the left eye's view
    this->render_view(this->left, false);

    // Render the right eye's view
    this->render_view(this->right, true);

    C3D_FrameEnd(0);

    return this;
}

void CircleGamePhase::exit() {
	C2D_SpriteSheetFree(this->sheet_fg_circle);
	C2D_SpriteSheetFree(this->sheet_bg_button);
}

void CircleGamePhase::render_view(C3D_RenderTarget* target, bool shift_flag) {
    C2D_TargetClear(target, C2D_Color32(0xff, 0xff, 0xff, 0xff));
	C2D_SceneBegin(target);

	for (int i = 0; i < 4; i++) {

		C2D_DrawImageAt(
			this->image_bg_button,
			CircleGamePhase::POSITIONS_FG_CIRCLE_BASE_X[i] - (CircleGamePhase::SIZE_BG_BUTTON / 2),
			CircleGamePhase::POSITIONS_FG_CIRCLE_BASE_Y[i] - (CircleGamePhase::SIZE_BG_BUTTON / 2),
			0
		);

		int shift = 0;
		if (i == this->index_raised) {
			if (shift_flag) {
				shift = -((this->offset+1) / 2);
			} else {
				shift = this->offset / 2;
			}
		}

		C2D_DrawImageAt(
			this->image_fg_circle,
			CircleGamePhase::POSITIONS_FG_CIRCLE_BASE_X[i] - (CircleGamePhase::SIZE_FG_CIRCLE / 2) + shift,
			CircleGamePhase::POSITIONS_FG_CIRCLE_BASE_Y[i] - (CircleGamePhase::SIZE_FG_CIRCLE / 2),
			0
		);
	}
}