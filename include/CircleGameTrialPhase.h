#ifndef CIRCLE_GAME_TRIAL_PHASE_H
#define CIRCLE_GAME_TRIAL_PHASE_H

#include <citro2d.h>

#include "Phase.h"

class CircleGameTrialPhase : public Phase {
  public:
    CircleGameTrialPhase(
        C3D_RenderTarget* left,
        C3D_RenderTarget* right,
        C2D_Image& image_fg_circle,
        C2D_Image& image_bg_button,
        int offset_i
    );

    void enter();
    Phase* update();
    void exit();
  private:
    void render_view(C3D_RenderTarget* target, bool shift_flag);

    // Targets for left and right eye respectively
    C3D_RenderTarget* left;
    C3D_RenderTarget* right;

	C2D_Image* image_fg_circle;
	C2D_Image* image_bg_button;

    int offset_i;
    int offset;
    int index_raised;
};

#endif