#include <citro2d.h>
#include <3ds.h>

#include "Phase.h"
#include "CircleGamePhase.h"
#include "CircleGameTrialPhase.h"

CircleGameTrialPhase::CircleGameTrialPhase(
        C3D_RenderTarget* left,
        C3D_RenderTarget* right,
        C2D_Image& image_fg_circle,
        C2D_Image& image_bg_button,
        int offset_i
) {
    this->left = left;
    this->right = right;

    *(this->image_fg_circle) = image_fg_circle;
    *(this->image_bg_button) = image_bg_button;

    this->offset_i = offset_i;
    this->offset = CircleGamePhase::OFFSETS[offset_i];
}

void CircleGameTrialPhase::enter() {
    // TODO determine random index_raised

	// which raised circle
	this->index_raised = 2;
}

Phase* CircleGameTrialPhase::update() {

    float slider = osGet3DSliderState();

    // Print useful information
    printf("\x1b[3;1H %.2f | %i | %i          \n", slider, this->offset_i, this->index_raised);

    // Render the scene
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

    // Render the left eye's view
    this->render_view(this->left, false);

    // Render the right eye's view
    this->render_view(this->right, true);

    C3D_FrameEnd(0);


    // Handle user input
    hidScanInput();
    int keysD = hidKeysDown();
    // int keysH = hidKeysHeld();

    if (keysD & KEY_START) return nullptr;

    // if (keysD & KEY_DRIGHT || keysH & KEY_DUP) this->offset++;
    // if (keysD & KEY_DLEFT || keysH & KEY_DDOWN) this->offset--;

    if (keysD & KEY_B) {
        int next_offset_i = this->offset_i+1;
        
        if (next_offset_i >= 30) return nullptr;
        // if (true) return nullptr;

        this->offset_i = next_offset_i;
        this->offset = CircleGamePhase::OFFSETS[this->offset_i];
        
        // return new CircleGameTrialPhase(
        //     this->left, this->right, *this->image_fg_circle, *this->image_bg_button, next_offset_i
        // );
    }

    // for (int i = 0; i < 4; i++) {
    //     if (keysD & DIRECTIONS_KEYS[i]) {
    //         this->index_raised = i;
    //         break;
    //     }
    // }


    return this;
}

void CircleGameTrialPhase::exit() {}


void CircleGameTrialPhase::render_view(C3D_RenderTarget* target, bool shift_flag) {
    C2D_TargetClear(target, C2D_Color32(0xff, 0xff, 0xff, 0xff));
	C2D_SceneBegin(target);

	for (int i = 0; i < 4; i++) {

		C2D_DrawImageAt(
			*(this->image_bg_button),
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
			*(this->image_fg_circle),
			CircleGamePhase::POSITIONS_FG_CIRCLE_BASE_X[i] - (CircleGamePhase::SIZE_FG_CIRCLE / 2) + shift,
			CircleGamePhase::POSITIONS_FG_CIRCLE_BASE_Y[i] - (CircleGamePhase::SIZE_FG_CIRCLE / 2),
			0
		);
	}
}
