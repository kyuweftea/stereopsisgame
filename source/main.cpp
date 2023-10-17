// Written by KonPet

#include <citro2d.h>
#include <3ds.h>

#include "fg_circle.h"
#include "bg_buttons.h"

int main() {
	int WIDTH_SCREEN_UPPER = 400;
	int HEIGHT_SCREEN_UPPER = 240;

	int POSITION_SCREEN_CENTER_X = WIDTH_SCREEN_UPPER / 2;
	int POSITION_SCREEN_CENTER_Y = HEIGHT_SCREEN_UPPER / 2;

	int SIZE_BG_BUTTONS = 240;
	int SIZE_FG_CIRCLE = 50;

	int POSITION_BG_BUTTONS_X = (WIDTH_SCREEN_UPPER - SIZE_BG_BUTTONS) / 2;

	int POSITION_FG_CIRCLE_CENTER_OFFSET = 70;

	int POSITIONS_FG_CIRCLE_BASE_X[4] = {
		POSITION_SCREEN_CENTER_X,
		POSITION_SCREEN_CENTER_X + POSITION_FG_CIRCLE_CENTER_OFFSET,
		POSITION_SCREEN_CENTER_X,
		POSITION_SCREEN_CENTER_X - POSITION_FG_CIRCLE_CENTER_OFFSET,
	};

	int POSITIONS_FG_CIRCLE_BASE_Y[4] = {
		POSITION_SCREEN_CENTER_Y - POSITION_FG_CIRCLE_CENTER_OFFSET,
		POSITION_SCREEN_CENTER_Y,
		POSITION_SCREEN_CENTER_Y + POSITION_FG_CIRCLE_CENTER_OFFSET,
		POSITION_SCREEN_CENTER_Y,
	};

	int DIRECTIONS_KEYS[4] = {
		KEY_X,
		KEY_A,
		KEY_B,
		KEY_Y,
	};

	// Targets for left and right eye respectively
	C3D_RenderTarget* left;
	C3D_RenderTarget* right;
	
	C2D_SpriteSheet sheet_fg_circle;
	C2D_SpriteSheet sheet_bg_buttons;
	C2D_Image image_fg_circle;
	C2D_Image image_bg_buttons;

	int keysD;
	int keysH;
	float slider;

	// Per-eye offset
	int offset = 0;

	// which raised circle
	int index_raised = 0;

	// Initialize libraries
	romfsInit();
	gfxInitDefault();
	gfxSet3D(true); // Activate stereoscopic 3D
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	printf("\x1b[1;1H Stereoscopic 3D with citro2d\n");

	// Create targets for both eyes on the top screen
	left = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	right = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);

	// Load fg_circle
	sheet_fg_circle = C2D_SpriteSheetLoad("romfs:/gfx/fg_circle.t3x");	
	image_fg_circle = C2D_SpriteSheetGetImage(sheet_fg_circle, 0);

	// Lead bg_buttons
	sheet_bg_buttons = C2D_SpriteSheetLoad("romfs:/gfx/bg_buttons.t3x");
	image_bg_buttons = C2D_SpriteSheetGetImage(sheet_bg_buttons, 0);

	while (aptMainLoop()) {
		// Handle user input
		hidScanInput();
		keysD = hidKeysDown();
		keysH = hidKeysHeld();

		if (keysD & KEY_START) break;

		if (keysD & KEY_DRIGHT || keysH & KEY_DUP) offset++;
		if (keysD & KEY_DLEFT || keysH & KEY_DDOWN) offset--;

		for (int i = 0; i < 4; i++) {
			if (keysD & DIRECTIONS_KEYS[i]) {
				index_raised = i;
				break;
			}
		}

		slider = osGet3DSliderState();

		// Print useful information
		printf("\x1b[3;1H %.2f | %i | %i          \n", slider, offset, index_raised);

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		// Render the left eye's view
		{
			C2D_TargetClear(left, C2D_Color32(0xff, 0xff, 0xff, 0xff));
			C2D_SceneBegin(left);

			C2D_DrawImageAt(image_bg_buttons, POSITION_BG_BUTTONS_X, 0, 0);

			for (int i = 0; i < 4; i++) {
				int shift = 0;
				if (i == index_raised) {
					shift = offset / 2;
				}

				C2D_DrawImageAt(
					image_fg_circle,
					POSITIONS_FG_CIRCLE_BASE_X[i] - (SIZE_FG_CIRCLE / 2) + shift,
					POSITIONS_FG_CIRCLE_BASE_Y[i] - (SIZE_FG_CIRCLE / 2),
					0
				);
			}

			// ScreenWidth - (faceWidth / 2) + offset for 3D
			// C2D_DrawImageAt(image_fg_circle, 100 + offsetUpper * slider, 30, 0);		
			// C2D_DrawImageAt(image_fg_circle, 100 + offsetLower * slider, 140, 0);
		}

		// Render the right eye's view
		{
			C2D_TargetClear(right, C2D_Color32(0xff, 0xff, 0xff, 0xff));
			C2D_SceneBegin(right);

			C2D_DrawImageAt(image_bg_buttons, POSITION_BG_BUTTONS_X, 0, 0);

			for (int i = 0; i < 4; i++) {
				int shift = 0;
				if (i == index_raised) {
					shift = (offset+1) / 2;
				}

				C2D_DrawImageAt(
					image_fg_circle,
					POSITIONS_FG_CIRCLE_BASE_X[i] - (SIZE_FG_CIRCLE / 2) - shift,
					POSITIONS_FG_CIRCLE_BASE_Y[i] - (SIZE_FG_CIRCLE / 2),
					0
				);
			}

			// ScreenWidth - (faceWidth / 2) - offset for 3D
			// C2D_DrawImageAt(image_fg_circle, 100 - offsetUpper * slider, 30, 0);
			// C2D_DrawImageAt(image_fg_circle, 100 - offsetLower * slider, 140, 0);
		}

		C3D_FrameEnd(0);
	}

	C2D_SpriteSheetFree(sheet_fg_circle);
	C2D_SpriteSheetFree(sheet_bg_buttons);

	// De-initialize libraries
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
}