#ifndef CIRCLE_GAME_PHASE_H
#define CIRCLE_GAME_PHASE_H

#include <citro2d.h>

#include "Phase.h"

class CircleGamePhase : public Phase {
  public:
    void enter();
    Phase* update();
    void exit();

    static const int OFFSETS[30];

	static const int WIDTH_SCREEN_UPPER;
	static const int HEIGHT_SCREEN_UPPER;

	static const int POSITION_SCREEN_CENTER_X;
	static const int POSITION_SCREEN_CENTER_Y;

	static const int SIZE_BG_BUTTON;
	static const int SIZE_FG_CIRCLE;

	static const int POSITION_FG_CIRCLE_CENTER_OFFSET;

	static const int POSITIONS_FG_CIRCLE_BASE_X[4];

	static const int POSITIONS_FG_CIRCLE_BASE_Y[4];

	static const int DIRECTIONS_KEYS[4];
  private:

    // Targets for left and right eye respectively
    C3D_RenderTarget* left;
    C3D_RenderTarget* right;
    
    C2D_SpriteSheet sheet_fg_circle;
	C2D_SpriteSheet sheet_bg_button;
	C2D_Image image_fg_circle;
	C2D_Image image_bg_button;

    Phase* trial;
};

#endif
