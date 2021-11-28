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


//////////////////////////////////////////
//                  Animation des Sprites
/////////////////////////////////////////
void AnimationSprite(Sprite1_ *spr)
{
    // Sécurité
    if  (!spr->Visible  || spr->SpriteA==NULL  || spr->StandBy ) return;

    // ID Airplane
    Sprite1_* SpriteREF = &Sprites[0];
    u16 AnimArme=0;

    // Tank Metal Slug du début de scène ?
    if (spr->ID==10)
	{
		switch (spr->Direction)
		{
		case 0:
			SPR_setAnim(spr->SpriteA,0);
			break;

		case 6:
			if (!spr->Transition) SPR_setAnim(spr->SpriteA,1);
			else SPR_setAnim(spr->SpriteA,2);
			break;
		}
		return;
	}

	// Bouclier ?
	if (spr->TypeBouclier)
	{
	    SPR_setAnim(spr->SpriteA,0);
	    return;
	}

	// Balles IA ?
	if (spr->ID==46)
	{
	    // On tire sur le joueur ?! Pas de gerbe de sang.
		if (spr->Hit)
		{
            spr->Hit=0;
            spr->StandBy=1;
            spr->Visible=0;
		}
		else
            // Helico Medium ?
            if (spr->SpeIA)
            {
                SPR_setAnim(spr->SpriteA,11);
                return;
            }
            // Balles sniper ?
            if (spr->Sniper) {SPR_setAnim(spr->SpriteA,1);return;}
            if (spr->IDList==1) SPR_setAnim(spr->SpriteA,1);
            else SPR_setAnim(spr->SpriteA,8);
		return;
	}

	// Balles joueur ?
    if (spr->ID==45)
	{
		if (spr->Hit)
		{
			spr->TempoSprite++;
			if (SpriteREF->Slot1==10) SPR_setAnim(spr->SpriteA,16);
			else
            {
                if (spr->Hit==1) SPR_setAnim(spr->SpriteA,2);
                if (spr->Hit==2) SPR_setAnim(spr->SpriteA,3);
            }
			if (spr->TempoSprite>15)
			{
				spr->Hit=0;
				spr->StandBy=1;
				spr->Visible=0;
				spr->TempoSprite=0;
			}
			return;
		}
		else
            if (SpriteREF->Slot1==1)
            {
                if (spr->Direction!=8)
                {
                    if (spr->Direction==6)
                    {
                        //spr->OffsetX=FIX32(0);
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,9);
                        return;
                    }
                    //spr->OffsetX=FIX32(16);
                    SPR_setHFlip(spr->SpriteA,TRUE);
                    SPR_setAnim(spr->SpriteA,9);
                }
                else  SPR_setAnim(spr->SpriteA,10);
                return;
            }
            if (SpriteREF->Slot1==2)
            {
                if (spr->Direction!=8)
                {
                    if (spr->Direction==6)
                    {
                        //spr->OffsetX=FIX32(0);
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,14);
                        return;
                    }
                    //spr->OffsetX=FIX32(16);
                    SPR_setHFlip(spr->SpriteA,TRUE);
                    SPR_setAnim(spr->SpriteA,14);
                }
                else  SPR_setAnim(spr->SpriteA,15);
                return;
            }
            if (SpriteREF->Slot1==10)
            {
                if (spr->Direction!=8)
                {
                    if (spr->Direction==6)
                    {
                        //spr->OffsetX=FIX32(0);
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,12);
                        return;
                    }
                    //spr->OffsetX=FIX32(16);
                    SPR_setHFlip(spr->SpriteA,TRUE);
                    SPR_setAnim(spr->SpriteA,12);
                }
                else  SPR_setAnim(spr->SpriteA,13);
                return;
            }


            if (spr->Direction!=8) SPR_setAnim(spr->SpriteA,0);
            else  SPR_setAnim(spr->SpriteA,4);
		return;
	}

    // Animation unités
   switch (spr->ID)
   {
    // Hélicoptère ?
    case 1:
        {
            if (spr->AirUnit==10)
            {
                if (spr->Hit)
                {
                    spr->TempoCouvert++;
                    if (spr->TempoCouvert==1) SND_startPlayPCM_XGM(SFX_GENERIC12, 1, SOUND_PCM_CH4);
                    if (spr->TempoCouvert>2)
                    {
                        spr->TempoCouvert=0;
                        spr->Hit=0;
                        VDP_setPalette(PAL3, Palette_Airplane.data);
                    }
                }

                // Destruction ?!
                if (spr->MortIA && spr->Visible)
                {
                    spr->TempoSprite++;
                    if (spr->TempoSprite==10)
                    {
                        TremblementON=1;
                        spr->TempoSprite=0;
                        TempoMax=8;
                        SND_startPlayPCM_XGM(SFX_GENERIC9, 1, SOUND_PCM_CH3);
                    }
                }

                // Tir !
                if (spr->TirBusy==2 && spr->Vitesse==FIX32(2.4))
                {
                    spr->TempoChauffeArme++;
                    if (spr->TempoChauffeArme>5)
                    {
                        spr->TempoChauffeArme=0;
                        spr->TirBusy=0;
                    }
                    if (spr->Direction==6)
                    {
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,1);
                    }
                    else
                    SPR_setHFlip(spr->SpriteA,TRUE);
                    SPR_setAnim(spr->SpriteA,1);
                    return;
                }


                // retournement hélico
                if (spr->CalculTransition==1)
                {
                    SPR_setHFlip(spr->SpriteA,FALSE);
                    SPR_setAnim(spr->SpriteA,2);
                    return;
                }

                if (spr->CalculTransition==2)
                {
                    SPR_setHFlip(spr->SpriteA,TRUE);
                    SPR_setAnim(spr->SpriteA,2);
                    return;
                }

                switch(spr->Direction)
                {
                case 6:
                    SPR_setHFlip(spr->SpriteA,FALSE);
                    if (!spr->Phase)
                    {
                        SPR_setAnim(spr->SpriteA,4);
                        break;
                    }
                    else if (spr->Phase==1)
                    {
                        SPR_setAnim(spr->SpriteA,5);
                        break;
                    }
                    if (spr->MortIA)
                    {
                        SPR_setAnim(spr->SpriteA,3);
                    }
                    //else SPR_setAnim(spr->SpriteA,0);
                    else
                    {
                        if (spr->HitPoint>=15+(Difficulte<<1)) SPR_setAnim(spr->SpriteA,0);
                        else if  (spr->HitPoint<15+(Difficulte<<1) && spr->HitPoint>=10+(Difficulte<<1)) SPR_setAnim(spr->SpriteA,6);
                        else if  (spr->HitPoint<10+(Difficulte<<1) && spr->HitPoint>=5+(Difficulte<<1)) SPR_setAnim(spr->SpriteA,7);
                        else SPR_setAnim(spr->SpriteA,8);
                    }

                    break;

                case 4:
                    SPR_setHFlip(spr->SpriteA,TRUE);
                    if (!spr->Phase)
                    {
                        SPR_setAnim(spr->SpriteA,4);
                        break;
                    }
                    else if (spr->Phase==1)
                    {
                        SPR_setAnim(spr->SpriteA,5);
                        break;
                    }
                    if (spr->MortIA)
                    {
                        SPR_setAnim(spr->SpriteA,3);
                    }
                    //else SPR_setAnim(spr->SpriteA,0);
                    else
                    {
                        if (spr->HitPoint>=15+(Difficulte<<1)) SPR_setAnim(spr->SpriteA,0);
                        else if  (spr->HitPoint<15+(Difficulte<<1) && spr->HitPoint>=10+(Difficulte<<1)) SPR_setAnim(spr->SpriteA,6);
                        else if  (spr->HitPoint<10+(Difficulte<<1) && spr->HitPoint>=5+(Difficulte<<1)) SPR_setAnim(spr->SpriteA,7);
                        else SPR_setAnim(spr->SpriteA,8);
                    }
                    break;
                }
                return;
            }


            if (spr->Hit)
            {
                spr->TempoCouvert++;
                if (spr->TempoCouvert==1) SND_startPlayPCM_XGM(SFX_GENERIC12, 1, SOUND_PCM_CH4);
                if (spr->TempoCouvert>2)
                {
                    spr->TempoCouvert=0;
                    spr->Hit=0;
                    VDP_setPalette(PAL3, Palette_Airplane.data);
                }
            }

            // Destruction ?!
            if (spr->MortIA && spr->Visible)
			{
				spr->TempoSprite++;
				if (spr->TempoSprite==10)
				{
					TremblementON=1;
					spr->TempoSprite=0;
					TempoMax=8;
					SND_startPlayPCM_XGM(SFX_GENERIC9, 1, SOUND_PCM_CH3);
				}
			}


			// retournement hélico
			if (spr->CalculTransition==1)
            {
				SPR_setHFlip(spr->SpriteA,FALSE);
                SPR_setAnim(spr->SpriteA,4);
                return;
            }
            if (spr->CalculTransition==2)
            {
				SPR_setHFlip(spr->SpriteA,TRUE);
                SPR_setAnim(spr->SpriteA,4);
                return;
            }

            switch(spr->Direction)
            {
            case 6:
				SPR_setHFlip(spr->SpriteA,FALSE);
				if (!spr->Phase)
				{
					SPR_setAnim(spr->SpriteA,6);
					break;
				}
				else if (spr->Phase==1)
				{
					SPR_setAnim(spr->SpriteA,7);
					break;
				}
				if (spr->MortIA)
				{
					SPR_setAnim(spr->SpriteA,5);
				}
				else
				{
					if (spr->HitPoint>=18+(Difficulte<<1)) SPR_setAnim(spr->SpriteA,0);
					else if  (spr->HitPoint<18+(Difficulte<<1) && spr->HitPoint>=13+(Difficulte<<1)) SPR_setAnim(spr->SpriteA,1);
					else if  (spr->HitPoint<13+(Difficulte<<1) && spr->HitPoint>=6+(Difficulte<<1)) SPR_setAnim(spr->SpriteA,2);
					else SPR_setAnim(spr->SpriteA,3);
				}
                break;

            case 4:
				SPR_setHFlip(spr->SpriteA,TRUE);
				if (!spr->Phase)
				{
					SPR_setAnim(spr->SpriteA,6);
					break;
				}
				else if (spr->Phase==1)
				{
					SPR_setAnim(spr->SpriteA,7);
					break;
				}
				if (spr->MortIA)
				{
					SPR_setAnim(spr->SpriteA,5);
				}
				else
				{
					if (spr->HitPoint>=18) SPR_setAnim(spr->SpriteA,0);
					else if  (spr->HitPoint<18 && spr->HitPoint>=13) SPR_setAnim(spr->SpriteA,1);
					else if  (spr->HitPoint<13 && spr->HitPoint>=6) SPR_setAnim(spr->SpriteA,2);
					else SPR_setAnim(spr->SpriteA,3);
				}
                break;
            }
            return;
        }


    // Unités terrestre
    // Soldat Bouclier
    case 2:
    {
        // Mort ?
        if (spr->MortIA && spr->IntIA==10)
        {
            SPR_setAnim(spr->SpriteA,6);
            return;
        }

       switch(spr->Direction)
       {
        case 0:
            // Tir ?
            if (spr->TirBusy)
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(6);
                SPR_setAnim(spr->SpriteA,2);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA3);
                return;
            }
            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(8);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,7);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
                else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(0);
                SPR_setAnim(spr->SpriteA,0);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            break;

        case 98:
            // Tir ?
            if (spr->TirBusy)
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(10);
                SPR_setAnim(spr->SpriteA,2);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA3);
                return;
            }

            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(-13);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,7);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
                else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(16);
                SPR_setAnim(spr->SpriteA,0);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            break;

        case 6:
            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(-13);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,7);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
                else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
            	if (spr->IAFuite)
				{
					SPR_setHFlip(spr->SpriteA,TRUE);
					spr->OffsetX=FIX32(-13);
					SPR_setAnim(spr->SpriteA,5);
					SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA2);
				}
				else
				{
					SPR_setHFlip(spr->SpriteA,TRUE);
					spr->OffsetX=FIX32(16);
					SPR_setAnim(spr->SpriteA,1);
					SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);
				}
            }
            break;

        case 4:
            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(8);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,7);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
                else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
            	if (spr->IAFuite)
				{
					SPR_setHFlip(spr->SpriteA,FALSE);
					spr->OffsetX=FIX32(8);
					SPR_setAnim(spr->SpriteA,5);
					SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA2);
				}
				else
				{
					SPR_setHFlip(spr->SpriteA,FALSE);
					spr->OffsetX=FIX32(0);
					SPR_setAnim(spr->SpriteA,1);
					SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);
				}
            }
            break;

       }
       return;
    }

    // Soldat RifleMan
    case 3:
    {
        // Mort ?
        if (spr->MortIA && spr->IntIA==10)
        {
            SPR_setAnim(spr->SpriteA,5);
            return;
        }

       switch(spr->Direction)
       {
        case 0:
            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(8);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,6);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
                else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(0);
                SPR_setAnim(spr->SpriteA,0);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            break;

        case 6:
            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(-13);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,6);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
               else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(0);
                SPR_setAnim(spr->SpriteA,1);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA0);
            }
            break;

        case 4:
            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(8);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,6);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
                else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(0);
                SPR_setAnim(spr->SpriteA,1);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA0);
            }
            break;

        case 50:
            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(8);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,6);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
                else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(0);
                SPR_setAnim(spr->SpriteA,2);
                // Cas d'un coup
                spr->TempoSprite++;
                if (spr->TempoSprite>40)
				{
					// Gestion Collision avec Joueur
					spr->TempoSprite=0;
					spr->CoupCrosse=spr->Direction;
				}
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            break;

        case 51:
            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(-13);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,6);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
                else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(-16);
                SPR_setAnim(spr->SpriteA,2);
                spr->TempoSprite++;
                if (spr->TempoSprite>40)
				{
					// Gestion Collision avec Joueur
					spr->TempoSprite=0;
					spr->CoupCrosse=spr->Direction;
				}
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            break;

        case 98:
            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(-13);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,6);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
                else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(-16);
                SPR_setAnim(spr->SpriteA,0);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            break;
       }
       return;
    }

    // Grenadier
    case 4:
    {
        // Mort ?
        if (spr->MortIA && spr->IntIA==10)
        {
            SPR_setAnim(spr->SpriteA,5);
            return;
        }

       switch(spr->Direction)
       {
        case 6:
            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(-13);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,6);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
                else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
                SPR_setHFlip(spr->SpriteA,TRUE);
                spr->OffsetX=FIX32(0);
                SPR_setAnim(spr->SpriteA,1);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);
                // Lancer de grenades !
                spr->TempoSprite++;
                if (spr->TempoSprite>=30)
                {
                    spr->TempoSprite=0;
                    u16 i=8;
                    Sprite1_*  spr1 = &Sprites[IDBalle];
                    while(i--)
                    {
                        //spr1 = &Sprites[IDBalle+j];
                        if (spr1->StandBy)
                        {
                            spr1->StandBy=0;
                            spr1->TempoRafale=0;
                            spr1->ID=88;     // ID Grenade
                            spr1->Hit=0;
                            SND_startPlayPCM_XGM(SFX_GENERIC4, 1, SOUND_PCM_CH3);
                            //SPR_setAnim(spr1->SpriteA,8);
                            spr1->Visible=1;
                            spr1->TypeIA=1;
                            spr1->Vitesse=FIX32(3)+spr->Boost;
                            spr1->CoordX=spr->CoordX;
                            spr1->CoordY=spr->CoordY+spr->DeltaY;
                            spr1->OffsetY=FIX32(-10);
                            spr1->Direction=6;
                            if (spr1->Direction==4) spr1->OffsetX=FIX32(0);
                            else  spr1->OffsetX=FIX32(-16);
                            spr1->Saut=1;
                            spr1->Acceleration=FIX32(4);
                            spr1->RefY=FIX32(140+50);
                            spr1->SensY=1;
                            break;
                        }
                        spr1++;
                    }
                }
            }
            break;

        case 4:
            if (spr->MortIA)
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(8);
                if (spr->IntIA==12) {SPR_setAnim(spr->SpriteA,6);SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);return;}
                if (spr->MortIA==1) SPR_setAnim(spr->SpriteA,3);
                else SPR_setAnim(spr->SpriteA,4);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            }
            else
            {
                SPR_setHFlip(spr->SpriteA,FALSE);
                spr->OffsetX=FIX32(0);
                SPR_setAnim(spr->SpriteA,1);
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);
                // Lancer de Grenades !
                spr->TempoSprite++;
                if (spr->TempoSprite>=30)
                {
                    spr->TempoSprite=0;
                    u16 i=8;
                    Sprite1_* spr1 = &Sprites[IDBalle];
                    while(i--)
                    {
                        //spr1 = &Sprites[IDBalle+j];
                        if (spr1->StandBy)
                        {
                            spr1->StandBy=0;
                            spr1->TempoRafale=0;
                            spr1->ID=88;     // ID Grenade
                            spr1->Hit=0;
                            SND_startPlayPCM_XGM(SFX_GENERIC4, 1, SOUND_PCM_CH3);
                            //SPR_setAnim(spr1->SpriteA,8);
                            spr1->Visible=1;
                            spr1->Vitesse=FIX32(3)+spr->Boost;
                            spr1->TypeIA=1;
                            spr1->CoordX=spr->CoordX;
                            spr1->CoordY=spr->CoordY+spr->DeltaY;
                            spr1->OffsetY=FIX32(-10);
                            spr1->Direction=4;
                            if (spr1->Direction==4) spr1->OffsetX=FIX32(0);
                            else  spr1->OffsetX=FIX32(-16);
                            spr1->Saut=1;
                            spr1->Acceleration=FIX32(4);
                            spr1->RefY=FIX32(140+50);
                            spr1->SensY=1;
                            break;
                        }
                        spr1++;
                    }
                }
            }
            break;
       }
       return;
    }

    // Parachutiste
    case 5:
    	// Course ?!
    	/*
    	if (spr->IntIA==1)
		{
			SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA);
            switch(spr->Direction)
            {
                case 6:
                    spr->OffsetX=FIX32(0);
                    SPR_setHFlip(spr->SpriteA,TRUE);
                    SPR_setAnim(spr->SpriteA,0);
                    return;

                case 4:
                    spr->OffsetX=FIX32(16);
                    SPR_setHFlip(spr->SpriteA,FALSE);
                    SPR_setAnim(spr->SpriteA,0);
                    return;
            }
            return;
		}
		*/
        // Mort ?!
        if (spr->MortIA)
        {
            SPR_setHFlip(spr->SpriteA,FALSE);
            spr->OffsetX=FIX32(0);
            SPR_setAnim(spr->SpriteA,5);
            SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            return;
        }
        // Animation de visée
        if (!spr->Animation) SPR_setAnim(spr->SpriteA,2);
        if (spr->Animation==6) SPR_setAnim(spr->SpriteA,3);
        if (spr->Animation==4) SPR_setAnim(spr->SpriteA,1);
        return;
        break;


    // Civil ?!
    case 6:

        // Libéré ?
        if (spr->MortIA==20)
        {
            SPR_setAnim(spr->SpriteA,1);
            return;
        }
        // Fuite !
        if (spr->MortIA==21)
        {
            SPR_setAnim(spr->SpriteA,2);
            return;
        }
        // Saut en l'air explosion ?
        if (spr->MortIA==2)
        {
            if (spr->Direction==4)
            {
                SPR_setAnim(spr->SpriteA,3);
                spr->OffsetX=FIX32(0);
                SPR_setHFlip(spr->SpriteA,FALSE);
            }
            else
            {
                SPR_setAnim(spr->SpriteA,3);
                spr->OffsetX=FIX32(0);
                SPR_setHFlip(spr->SpriteA,TRUE);
            }
            return;
        }
        SPR_setAnim(spr->SpriteA,0);
        return;
        break;

    // Sniper Zone 2
    case 7:

        if (spr->MortIA)
        {
            SPR_setHFlip(spr->SpriteA,FALSE);
            spr->OffsetX=FIX32(0);
            SPR_setAnim(spr->SpriteA,7);
            SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA1);
            return;
        }
        // Les civils se dévoilent
        if (spr->Reach==1)
        {
            spr->TempoSprite++;
            SPR_setAnim(spr->SpriteA,0);
            if (spr->TempoSprite>25)
            {
                spr->TempoSprite=0;
                spr->Reach=2;
            }
        }
        else if (spr->Reach==2)
        {
            switch (spr->Animation)
            {
            case 1:
                spr->OffsetX=FIX32(16);
                break;

            case 2:
                spr->OffsetX=FIX32(-11);
                break;

            case 3:
                spr->OffsetX=FIX32(0);
                break;
            }
            if (spr->TempoRafale>15 && spr->TempoRafale<25) SPR_setAnim(spr->SpriteA,spr->Animation+3);
            else SPR_setAnim(spr->SpriteA,spr->Animation);
            return;
        }

        break;

    // Bonus !
    case 40:
        SPR_setAnim(spr->SpriteA,spr->Transition+2);
        return;
        break;

// Bombe ?!
    case 66:

        SPR_setHFlip(spr->SpriteA,FALSE);
        spr->OffsetX=FIX32(0);

        if (spr->Hit)
        {
            spr->TempoSprite++;
            if (spr->TempoSprite==1) SND_startPlayPCM_XGM(SFX_GENERIC9, 1, SOUND_PCM_CH3);
            SPR_setAnim(spr->SpriteA,5);
            if (spr->TempoSprite>=30)
            {
                spr->StandBy=1;
                spr->Hit=0;
                spr->Visible=0;
                spr->TempoSprite=0;
                Bombardement=0;
                return;
            }
        }
        else
        {
            SPR_setAnim(spr->SpriteA,6);
        }
        break;

    // Grenade ?!
    case 88:
            if (spr->Hit)
            {
                spr->TempoSprite++;
                if (spr->TempoSprite==1) SND_startPlayPCM_XGM(SFX_GENERIC9, 1, SOUND_PCM_CH3);
                SPR_setAnim(spr->SpriteA,5);
                if (spr->TempoSprite>=30)
                {
                    spr->StandBy=1;
                    spr->Hit=0;
                    spr->Visible=0;
                    spr->TempoSprite=0;
                    return;
                }
            }
            else
            {
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChangedIA0);
                SPR_setAnim(spr->SpriteA,7);
            }
        return;

    // Cas de joe !
    case 99:

        // Phase Scene 4 ?
        if (PhaseScene>=3)
        {
            spr->TempoSprite++;
            if (spr->TempoSprite==10) SND_startPlayPCM_XGM(SFX_GENERIC13, 2, SOUND_PCM_CH4);
            if (spr->TempoSprite<90) SPR_setAnim(spr->SpriteA,36);
            else  SPR_setAnim(spr->SpriteA,0);
            return;
        }
        // Shotgun ?!
        if (SpriteREF->Slot1==1 || SpriteREF->Slot1==10) AnimArme=18;
        else if (SpriteREF->Slot1==2 ) AnimArme=37;

        // Game over ?!
        if (GameOver==1)
        {
            SPR_setHFlip(spr->SpriteA,FALSE);
            SPR_setAnim(spr->SpriteA,10);
            return;
        }

        // Lancer de Grenade ?!
        if (spr->LancerGrenade)
        {
            spr->TempoSprite++;
            // Fin de lancer ?!
            if (spr->TempoSprite>30)
            {
                spr->TempoSprite=0;
                spr->LancerGrenade=0;
                return;
            }
            SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
            switch(spr->MemDir)
            {
                case 0:
                    spr->OffsetX=FIX32(0);
                    SPR_setHFlip(spr->SpriteA,FALSE);
                    SPR_setAnim(spr->SpriteA,11+AnimArme);
                    return;

                case 6:
                    spr->OffsetX=FIX32(0);
                    SPR_setHFlip(spr->SpriteA,FALSE);
                    SPR_setAnim(spr->SpriteA,11+AnimArme);
                    return;

                case 4:
                    spr->OffsetX=FIX32(16);
                    SPR_setHFlip(spr->SpriteA,TRUE);
                    SPR_setAnim(spr->SpriteA,11+AnimArme);
                    return;
            }
            return;
        }
        // Tir / Action du joueur ?!

        if (spr->Feu==1)
        {
        	// Cas d'un saut
            if (spr->Saut)
            {
                    SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);

                    // Cas des sauts et Tir en haut.
                    if (spr->Direction==88)
                    {
                        SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                        switch(spr->MemDir)
                        {
                            case 0:
                                SPR_setHFlip(spr->SpriteA,FALSE);
                                SPR_setAnim(spr->SpriteA,14+AnimArme);
                                spr->OffsetX=FIX32(0);
                                return;

                            case 6:
                                SPR_setHFlip(spr->SpriteA,FALSE);
                                SPR_setAnim(spr->SpriteA,14+AnimArme);
                                spr->OffsetX=FIX32(0);
                                return;

                            case 4:
                                SPR_setHFlip(spr->SpriteA,TRUE);
                                SPR_setAnim(spr->SpriteA,14+AnimArme);
                                spr->OffsetX=FIX32(16);
                                return;
                        }
                    }

                     if (spr->Direction==84)
                     {
                        SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                        spr->OffsetX=FIX32(16);
                        SPR_setHFlip(spr->SpriteA,TRUE);
                        SPR_setAnim(spr->SpriteA,14+AnimArme);
                        return;
                     }

                     if (spr->Direction==86)
                     {
                        SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                        spr->OffsetX=FIX32(0);
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,14+AnimArme);
                        return;
                     }

                    // Cas normaux.
                    switch(spr->MemDir)
                    {
                        case 0:
                            spr->OffsetX=FIX32(5);
                            SPR_setHFlip(spr->SpriteA,FALSE);
                            SPR_setAnim(spr->SpriteA,7+AnimArme);
                            return;

                        case 6:
                            spr->OffsetX=FIX32(5);
                            SPR_setHFlip(spr->SpriteA,FALSE);
                            SPR_setAnim(spr->SpriteA,7+AnimArme);
                            return;

                        case 4:
                            spr->OffsetX=FIX32(10);
                            SPR_setHFlip(spr->SpriteA,TRUE);
                            SPR_setAnim(spr->SpriteA,7+AnimArme);
                            return;
                    }
                    return;
            }

            // Couteau ?!
            if (spr->Couteau)
			{
                    SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                    switch(spr->MemDir)
                    {
                        case 0:
                            spr->OffsetX=FIX32(0);
                            SPR_setHFlip(spr->SpriteA,FALSE);
                            SPR_setAnim(spr->SpriteA,9+AnimArme);
                            return;

                        case 6:
                            spr->OffsetX=FIX32(0);
                            SPR_setHFlip(spr->SpriteA,FALSE);
                            SPR_setAnim(spr->SpriteA,9+AnimArme);
                            return;

                        case 4:
                            spr->OffsetX=FIX32(16);
                            SPR_setHFlip(spr->SpriteA,TRUE);
                            SPR_setAnim(spr->SpriteA,9+AnimArme);
                            return;
                    }
                    return;
			}
            switch(spr->Direction)
            {
            case 0:
                // Ajustement du timer d'animation du SE.
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                switch(spr->MemDir)
                {
                    case 0:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,5+AnimArme);
                        spr->OffsetX=FIX32(0);
                        return;

                    case 6:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,5+AnimArme);
                        spr->OffsetX=FIX32(0);
                        return;

                    case 4:
                        SPR_setHFlip(spr->SpriteA,TRUE);
                        SPR_setAnim(spr->SpriteA,5+AnimArme);
                        spr->OffsetX=FIX32(16);
                        return;
                }

            case 2:
                switch(spr->MemDir)
                {
                    case 0:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,8+AnimArme);
                        spr->OffsetX=FIX32(-6);
                        return;

                    case 6:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,8+AnimArme);
                        spr->OffsetX=FIX32(-6);
                        return;

                    case 4:
                        SPR_setHFlip(spr->SpriteA,TRUE);
                        SPR_setAnim(spr->SpriteA,8+AnimArme);
                        spr->OffsetX=FIX32(22);
                        return;
                }
                return;

            case 4:
                spr->OffsetX=FIX32(16);
                SPR_setHFlip(spr->SpriteA,TRUE);
                SPR_setAnim(spr->SpriteA,6+AnimArme);
                break;

            case 6:
                spr->OffsetX=FIX32(0);
                SPR_setHFlip(spr->SpriteA,FALSE);
                SPR_setAnim(spr->SpriteA,6+AnimArme);
                break;

            case 84:
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                spr->OffsetX=FIX32(16);
                SPR_setHFlip(spr->SpriteA,TRUE);
                SPR_setAnim(spr->SpriteA,13+AnimArme);
                break;

            case 86:
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                spr->OffsetX=FIX32(0);
                SPR_setHFlip(spr->SpriteA,FALSE);
                SPR_setAnim(spr->SpriteA,13+AnimArme);
                break;

            case 88:
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                switch(spr->MemDir)
                {
                    case 0:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,12+AnimArme);
                        spr->OffsetX=FIX32(0);
                        return;

                    case 6:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,12+AnimArme);
                        spr->OffsetX=FIX32(0);
                        return;

                    case 4:
                        SPR_setHFlip(spr->SpriteA,TRUE);
                        SPR_setAnim(spr->SpriteA,12+AnimArme);
                        spr->OffsetX=FIX32(16);
                        return;
                }
                break;
            }
            return;
        }

        //Saut ?!
        if (spr->Saut)
        {
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                switch(spr->MemDir)
                {
                    case 0:
                        spr->OffsetX=FIX32(5);
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,2+AnimArme);
                        return;

                    case 6:
                        spr->OffsetX=FIX32(5);
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,2+AnimArme);
                        return;

                    case 4:
                        spr->OffsetX=FIX32(10);
                        SPR_setHFlip(spr->SpriteA,TRUE);
                        SPR_setAnim(spr->SpriteA,2+AnimArme);
                        return;
                }
        }

        // Déplacement / Divers
        switch(spr->Direction)
        {
            // Idle Joueur
            case 0:
                // Ajustement du timer d'animation du SE.
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                switch(spr->MemDir)
                {
                    case 0:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,0+AnimArme);
                        spr->OffsetX=FIX32(0);
                        return;

                    case 6:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,0+AnimArme);
                        spr->OffsetX=FIX32(0);
                        return;

                    case 4:
                        SPR_setHFlip(spr->SpriteA,TRUE);
                        SPR_setAnim(spr->SpriteA,0+AnimArme);
                        spr->OffsetX=FIX32(16);
                        return;
                }

            case 2:
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                switch(spr->MemDir)
                {
                    case 0:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,3+AnimArme);
                        spr->OffsetX=FIX32(0);
                        return;

                    case 6:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,3+AnimArme);
                        spr->OffsetX=FIX32(0);
                        return;

                    case 4:
                        SPR_setHFlip(spr->SpriteA,TRUE);
                        SPR_setAnim(spr->SpriteA,3+AnimArme);
                        spr->OffsetX=FIX32(16);
                        return;
                }
                break;

            case 4:
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                spr->OffsetX=FIX32(16);
                SPR_setHFlip(spr->SpriteA,TRUE);
                SPR_setAnim(spr->SpriteA,1+AnimArme);
                break;

            case 6:
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                spr->OffsetX=FIX32(0);
                SPR_setHFlip(spr->SpriteA,FALSE);
                SPR_setAnim(spr->SpriteA,1+AnimArme);
                break;

            case 88:
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                switch(spr->MemDir)
                {
                    case 0:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,15+AnimArme);
                        spr->OffsetX=FIX32(0);
                        return;

                    case 6:
                        SPR_setHFlip(spr->SpriteA,FALSE);
                        SPR_setAnim(spr->SpriteA,15+AnimArme);
                        spr->OffsetX=FIX32(0);
                        return;

                    case 4:
                        SPR_setHFlip(spr->SpriteA,TRUE);
                        SPR_setAnim(spr->SpriteA,15+AnimArme);
                        spr->OffsetX=FIX32(16);
                        return;
                }
                break;

            case 24:
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                spr->OffsetX=FIX32(16);
                SPR_setHFlip(spr->SpriteA,TRUE);
                SPR_setAnim(spr->SpriteA,4+AnimArme);
                break;

            case 26:
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                spr->OffsetX=FIX32(0);
                SPR_setHFlip(spr->SpriteA,FALSE);
                SPR_setAnim(spr->SpriteA,4+AnimArme);
                break;

            case 84:
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                spr->OffsetX=FIX32(16);
                SPR_setHFlip(spr->SpriteA,TRUE);
                SPR_setAnim(spr->SpriteA,16+AnimArme);
                break;

            case 86:
                SPR_setFrameChangeCallback(spr->SpriteA, &TimerChanged);
                spr->OffsetX=FIX32(0);
                SPR_setHFlip(spr->SpriteA,FALSE);
                SPR_setAnim(spr->SpriteA,16+AnimArme);
                break;

        }
        break;
   }
}


//////////////////////////////////////////
//                  Init Timer Sprite IA
/////////////////////////////////////////

void TimerChangedIA(Sprite* sprite)
{
    sprite->timer=4;
}

void TimerChangedIA3(Sprite* sprite)
{
    sprite->timer=6;
}

void TimerChangedIA0(Sprite* sprite)
{
    sprite->timer=3;
}

void TimerChangedIA2(Sprite* sprite)
{
    sprite->timer=2;
}

void TimerChangedIA1(Sprite* sprite)
{
    sprite->timer=10;
}

//////////////////////////////////////////
//                  Init Timer Sprite joueur
/////////////////////////////////////////
void TimerChanged(Sprite* sprite)
{
    Sprite1_* spr = &Sprites[0];

	// Lancer Grenade ?!
	if (spr->LancerGrenade)
    {
        sprite->timer=4;
        return;
    }

    // Tir ?
	// Saut ?
	if (spr->Feu==1)
    {
        sprite->timer=4;
        return;
    }

	// Haut ?
	if (spr->Direction==88)
    {
        sprite->timer=10;
        return;
    }

	if (spr->Direction==86 || spr->Direction==84)
    {
        sprite->timer=4;
        return;
    }

	// Saut ?
	if (spr->Saut)
    {
        sprite->timer=8;
        return;
    }

    // Couteau ?
    if (spr->Couteau)
	{
		sprite->timer=6;
		return;
	}


	// Déplacement / Divers
    switch (spr->Direction)
    {
        case 0:
            sprite->timer=12;
            break;
        case 2:
            sprite->timer=12;
            break;
        case 24:
            sprite->timer=10;
            break;

        case 26:
            sprite->timer=10;
            break;
        case 6:
            sprite->timer=3;
            break;
        case 4:
            sprite->timer=3;
            break;
    }
}

