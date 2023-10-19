// Written by KonPet

#include <citro2d.h>
#include <3ds.h>

#include "Phase.h"
#include "CircleGamePhase.h"

#include "fg_circle.h"
#include "bg_button.h"

int main() {

	// Initialize libraries
	romfsInit();
	gfxInitDefault();
	gfxSet3D(true); // Activate stereoscopic 3D
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	printf("\x1b[1;1H Stereoscopic 3D with citro2d\n");

	Phase* phase = new CircleGamePhase();
	phase->enter();

	while (aptMainLoop()) {

		Phase* resultPhase = phase->update();

		if (resultPhase == phase) continue;
		
		phase->exit();
		delete phase;

		phase = resultPhase;

		if (phase == nullptr) break;

		phase->enter();
	}

	if (phase != nullptr) {
		phase->exit();
		delete phase;
	}

	// De-initialize libraries
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
}