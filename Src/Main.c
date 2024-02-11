
// Librairies Projet

#include <genesis.h>
#include "main.h"
#include "gfx.h"
#include "Variables.h"
#include "Routines.h"
#include "dma.h"
#include "Sprites.h"
#include "Sound.h"
#include "Init.h"
#include "Animations.h"
#include "GestionPAD.h"

// Procédure principale

int main(u16 hardreset)
{
	// Init HARD
	if (!hardreset) SYS_hardReset();

	// Init HW
	InitRoutine();

	// Start !
	MainLoop();

	return 0;
}

//////////////////////////////////////////
//               Init Main
/////////////////////////////////////////
void MainLoop()
{
	while(TRUE)
	{
		// Ecran intro
		InitIntro();

		// Ecran sélection Zone
		InitEcranZone();

		// Init MAP
		InitMAP();

		// Init Scene
		InitScene();

		// Zone Boss ?
		if (NumeroZone==2)
		{
			Zone3();
			// Plus de Zone Boss après coup.
			RequisZone3=99;
			GameOver=1;
		}

		else

		{
			// Fade In Scene.
			VDP_fadeInAll(palette,15,FALSE);
			Score=8700;
			Difficulte=7;

			// Main Loop
			while(TRUE)
			{
				// Scene principale
				RandomSeed();
				UpdateScene();

				// Pause
				if (PauseGame)
				{
					TestPause++;
					if (TestPause<10) PAL_fadeOutAll(6,TRUE);
					else PAL_interruptFade();
				}

				// Game Over ?! On quitte
				if (GameOver==2)  break;

				// Vblank
				SYS_doVBlankProcess();

				// Update Scrolling BG.
				updateVDPScroll();

			}

			// Fin de la boucle !
			VDP_fadeOutAll(16,FALSE);
			RAZ_PCM();
			XGM_stopPlay();
			MEM_free(bgb);
			MEM_free(bga);
			SPR_end();

			// set all palette to black
			VDP_setPaletteColors(0, (u16*) palette_black, 64);
			//SYS_doVBlankProcess();
			VDP_init();

			// Résultat du score
			Result_Screen();
		}

        // Fin
		VDP_fadeOutAll(16,FALSE);
		RAZ_PCM();
		XGM_stopPlay();
		SPR_end();
		// Init général
		Clear_Variable();
		VDP_setPaletteColors(0, (u16*) palette_black, 64);
		VDP_init();
	}

}





