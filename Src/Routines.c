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


////////////////////////////////////////
//                  Gestion Scene
////////////////////////////////////////
void UpdateScene()
{
	u16 i;
	Sprite1_* spr;

	// sprite joueur principal
	spr = &Sprites[0];

	PrintFix32(spr->CoordX,15,20);

	// Avec le ShotGun, le joueur ira un peu moins vite.
	if (spr->Slot1) spr->VitesseD=FIX32(0.2);
	else  spr->VitesseD=FIX32(0);

    // Gestion PAD
    GestionPAD(spr);

    // Pause en jeu ?
    if (PauseGame) return;

	// Gestion Bonus
	GestionBonus(spr);

	// Equilibrage des vagues
    GestionVague();

	// Gestion UP
	GestionUP(spr);

    // Tir ?
    GestionTir(spr);

    // Couteau joueur ?!
	GestionCouteau(spr);

    // Gestion attaque hélico
    Sprite1_* spr2;
    spr2=&Sprites[IDPlane];
    GestionAttaqueHelico(spr2);

	// Medailles ?!
    if (ApparitionMedailles) DisplayMedals();

    // Gestion Balles
    GestionProjectiles();

    // Routine de visibilité globale des sprites
    if (!PhaseScene) UpdateViewSprite(spr);
    else Phase_Scene(PhaseScene,spr);

    // Sprite Clignote ?!
    Cligno_Sprite(spr);

    // Animation Sprite
    AnimationSprite(spr);

    // Déplacement (important, c'est ici qu'on met à jour la position du joueur)
    UpdateSprite(spr);

 	// Gestion de la marche de Joe (en raccord avec la nouvelle position du joueur)
    MarcheJoe(spr);

    // Gestion Caméra (on met à jour la position de la caméra en fonction de la position du joueur)
    updateCameraPosition(spr);

    // set new camera position (c'est là que CamPosX et CamPosY sont mis à jour, donc important de le faire maintenant !)
    setCameraPosition(PCamX, PCamY);

    // Gestion Civil MAP
    GestionCivil(spr);

    // Affichage Sprite (on peut maintenant afficher le sprite en fonction de la position de la caméra)
    DisplaySprite(spr);

	// maintenant on peut gérer les autres sprites
	spr = &Sprites[1];
	i = MaxObjet - 1;

    // Boucle Principale
	while (i--)
    {
        // Routine de visibilité globale des sprites
        if (!PhaseScene) UpdateViewSprite(spr);
        else Phase_Scene(PhaseScene,spr);

        // Création vague IA.
        CreateIANew(spr);

        // Animation Sprite
        AnimationSprite(spr);

        // Gestion IA
        GestionIA(spr);

        // Sprite Clignote ?!
        Cligno_Sprite(spr);

        // Déplacement
        UpdateSprite(spr);

        // Affichage Sprite
        DisplaySprite(spr);

        // next sprite
        spr++;
    }

    // update sprites
    if (!PauseGame) SPR_update();
}


//////////////////////////////////////
//       Gestion Bonus
//////////////////////////////////////
void GestionBonus(Sprite1_ *spr)
{
    Sprite1_* SpriteB;
    SpriteB = &Sprites[IDBonus];
    if (!SpriteB->Init) return;

    // Attribution des bonus ?!
    fix32 DistanceIA;
    DistanceIA=abs(spr->CoordX - (SpriteB->CoordX));
    if (DistanceIA<=FIX32(24))
    {
        SpriteB->Visible=0;
        SpriteB->StandBy=1;
        SpriteB->Init=0;
        SND_startPlayPCM_XGM(SFX_GENERIC14, 2, SOUND_PCM_CH4);
        GoCivil=1;
        switch(SpriteB->Transition)
        {
            case 0:
                NombreGrenade+=5;
                GestionNombreGrenades();
                break;

            case 1:
                spr->Slot1=1;
                NombreBalleShotgun=88;
                GestionNombreBallesShotgun();
                break;

            case 2:
                spr->HitPoint=6;
                GestionHUDSante();
                break;

            case 3:
                Score+=250;
                GestionScore();
                break;

            case 4:
                spr->NombreUP++;
                GestionHUDSante();
                break;
        }
    }


}


//////////////////////////////////////
//       Gestion Balles New
//////////////////////////////////////
void GestionProjectiles()
{
    Sprite1_* spr1;
    spr1 = &Sprites[IDBalle];

    // ID Ref joueur
    Sprite1_* SpriteREF;
    SpriteREF=&Sprites[0];

    // Déroulage des 6 entités.
    GestionBalles(spr1, SpriteREF);
    GestionGrenades(spr1);
    GestionBombe(spr1);
    spr1++;
    GestionBalles(spr1, SpriteREF);
    GestionGrenades(spr1);
    GestionBombe(spr1);
    spr1++;
    GestionBalles(spr1, SpriteREF);
    GestionGrenades(spr1);
    GestionBombe(spr1);
    spr1++;
    GestionBalles(spr1, SpriteREF);
    GestionGrenades(spr1);
    GestionBombe(spr1);
    spr1++;
    GestionBalles(spr1, SpriteREF);
    GestionGrenades(spr1);
    GestionBombe(spr1);
    spr1++;
    GestionBalles(spr1, SpriteREF);
    GestionGrenades(spr1);
    GestionBombe(spr1);
    spr1++;
    GestionBalles(spr1, SpriteREF);
    GestionGrenades(spr1);
    GestionBombe(spr1);
}


//////////////////////////////////////
//       Gestion civil
//////////////////////////////////////
void GestionCivil(Sprite1_ *spr)
{
    // Sécurité
    if (PhaseScene || !GoCivil) return;

    //if (NombreIAScene>=NombreIASceneMax && !CivilON) return;

    // Civil déjà présent ?!
    Sprite1_* SprCivil;
    SprCivil=&SpriteCivil;
    if (CivilON && NombreCivil)
    {
        // Distance joueur / IA
        fix32 DistanceIA;
        DistanceIA=abs(spr->CoordX - Civil_CoordX);
        if (DistanceIA<=FIX32(180))
        {
            SPR_setVisibility(SprCivil->SpriteA,HIDDEN);
            CivilVisible=1;
            return;
        }
        if (DistanceIA>FIX32(180) && spr->CoordX>=Civil_CoordX)
        {
            SPR_setAnim(SprCivil->SpriteA,1);
            SPR_setVisibility(SprCivil->SpriteA,VISIBLE);
            SPR_setPosition(SprCivil->SpriteA,0,80);
            CivilVisible=0;
        }
        else if (DistanceIA>FIX32(180) && spr->CoordX<Civil_CoordX)
        {
            SPR_setAnim(SprCivil->SpriteA,0);
            SPR_setVisibility(SprCivil->SpriteA,VISIBLE);
            SPR_setPosition(SprCivil->SpriteA,284,80);
            CivilVisible=0;
        }

        return;
    }

    // Spawn Civil.
    SprCivil->TempoSprite++;
    if (SprCivil->TempoSprite<450+(Difficulte<<2)) return;
    u8 Chance=0;

    // Helico en court d'attaque ? On quitte !
    Sprite1_* spr2;
    spr2=&Sprites[IDPlane];
    if (spr2->Phase==2) return;

    if (getRandomU16(100)<2) Chance=1;

    // On manque de truc ?!
    if (spr->HitPoint<=2 || !NombreBalleShotgun || !NombreGrenade || Chance)
    {
        //RandomSeed();
        if (getRandomU16(100)<(1+Difficulte))
        {
            CivilON=1;
            SprCivil->TempoSprite=0;
            Civil_CoordY=FIX32(140-48);

            //RandomSeed();
            fix32 CX;
            if (getRandomU16(100)<50)
            {
                CX=getRandomF32(FIX32(320))+FIX32(320);
                Civil_CoordX=spr->CoordX+CX;
                if (Civil_CoordX>FIX32(2048)) Civil_CoordX=spr->CoordX-CX;
            }
            else
            {
                CX=getRandomF32(FIX32(320))+FIX32(320);
                Civil_CoordX=spr->CoordX-CX;
                if (Civil_CoordX<FIX32(0)) Civil_CoordX=spr->CoordX+CX;
            }
        }
    }
}


//////////////////////////////////////
//       Gestion vagues
//////////////////////////////////////
void GestionVague()
{
    Sprite1_* spr2;
	spr2 = &Sprites[IDPlane];
    // Paramétrage difficulté / Médailles
    TempoRegen=125-(Difficulte<<1)-(NumeroZone<<2);


	switch(Difficulte)
	{
    case 0:
        NombreIASceneMax=1;
        break;
    case 1:
        NombreIASceneMax=1;
        break;
    case 2:
        NombreIASceneMax=2;
        break;
    case 3:
        NombreIASceneMax=2;
        break;
    case 4:
        NombreIASceneMax=3;
        break;
    case 5:
        NombreIASceneMax=3;
        break;
    case 6:
        NombreIASceneMax=4;
        break;
    case 7:
        NombreIASceneMax=4;
        break;
	}
	if (Difficulte>7) NombreIASceneMax=5;
	if (spr2->Phase) NombreIASceneMax=1;
}



//////////////////////////////////////
//       Gestion création liste IA
//////////////////////////////////////
void CreateIANew(Sprite1_ *spr)
{

    if (!spr->SpriteDYN || PhaseScene) return;
    if (NombreIAScene>=NombreIASceneMax) return;
    if (spr->SpriteA!=NULL) return;

    // Génération IA Vague.
    spr->TempoRespawn++;
    if (spr->TempoRespawn>TempoRegen)
    {
        spr->TempoRespawn=0;
        ChoixUnite++;
        if (ChoixUnite>5) ChoixUnite=1;

        // Type ?
        //RandomSeed();
        while (TRUE)
        {
            if (CivilON && !NombreCivil)
            {
                spr->ID=6;
                spr->SpriteA = SPR_addSpriteSafe(&Civil_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                NombreCivil=1;
                break;
            }
            switch (ChoixUnite)
            {
                // Bouclier
                case 1:
                    spr->ID=2;
                    spr->SpriteA = SPR_addSpriteSafe(&SoldatType1_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                    spr->HitPointMax=4;
                    break;
                // Fantassin
                case 2:
                    spr->ID=3;
                    spr->SpriteA = SPR_addSpriteSafe(&SoldatType2_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                    spr->HitPointMax=1;
                    break;
                // Grenadier
                case 3:
                    if (NumeroZone)
                    {
                        Sprite1_* spr2;
                        spr2=&Sprites[IDPlane];
                        // Evitons les grenadiers avec les hélicos.
                        if (spr2->Phase)
                        {
                            spr->ID=3;
                            spr->SpriteA = SPR_addSpriteSafe(&SoldatType2_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                            spr->HitPointMax=1;
                            break;
                        }
                        spr->ID=4;
                        spr->SpriteA = SPR_addSpriteSafe(&SoldatType3_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                        spr->HitPointMax=1;
                    }
                    else
                    {
                        spr->ID=3;
                        spr->SpriteA = SPR_addSpriteSafe(&SoldatType2_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                        spr->HitPointMax=1;
                    }

                    break;

                // Parachutiste
                case 5:
                    spr->ID=5;
                    spr->SpriteA = SPR_addSpriteSafe(&SoldatType4_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                    spr->HitPointMax=2;
                    break;
            }
            break;
        }

        // Sécurité !
        if (spr->SpriteA==NULL)
        {
            // Civil non crée ?!
            if (spr->ID==6)
            {
                NombreCivil=0;
                CivilON=0;
                spr->TempoSprite=0;
            }
            SPR_releaseSprite(spr->SpriteA);
            return;
        }

        // Incrémentation unité scène.
        NombreIAScene++;

        CreateSpriteDYN(spr,spr->ID);
         // On entend les civils hurler !!
        if (spr->ID==6)
        {
            SND_startPlayPCM_XGM(SFX_GENERIC13, 2, SOUND_PCM_CH4);
            SPR_setVisibility(spr->SpriteA,HIDDEN);
            spr->Visible=0;
        }
        else
        {
            SPR_setVisibility(spr->SpriteA,VISIBLE);
            spr->Visible=1;
        }
        SPR_setPriorityAttribut(spr->SpriteA, TRUE);
        SPR_setAlwaysOnTop(spr->SpriteA,TRUE);
        spr->StandBy=0;
        spr->IAFuite=0;
        spr->TypeBouclier=0;
        spr->TempoCountRafaleMax=getRandomU16(50)+15;
        spr->HitPoint=spr->HitPointMax;
        spr->TypeIA=1;
        spr->Reach=0;
        spr->MortIA=0;
        return;
    }
}


//////////////////////////////////////
//       Gestion MArche Joe
//////////////////////////////////////
void MarcheJoe(Sprite1_ *spr)
{
    Sprite1_* SprMarcheJoe;
    SprMarcheJoe=&Marche_Joe;
    // Gestion
	if (spr->Feu)
    {
        if (spr->Direction==4 || spr->Direction==6 ||spr->Direction==86 || spr->Direction==84)
        {
            SPR_setVisibility(SprMarcheJoe->SpriteA,VISIBLE);
            SprMarcheJoe->CoordX=spr->CoordX;
            SprMarcheJoe->CoordY=spr->CoordY;
            SprMarcheJoe->Visible=1;
            if (spr->Direction==4 || spr->Direction==84)
            {
                SprMarcheJoe->OffsetX=FIX32(16);
                SPR_setHFlip(SprMarcheJoe->SpriteA,TRUE);
                SPR_setAnim(SprMarcheJoe->SpriteA,0);
            }
            else if (spr->Direction==6 || spr->Direction==86)
            {
                SprMarcheJoe->OffsetX=FIX32(0);
                SPR_setHFlip(SprMarcheJoe->SpriteA,FALSE);
                SPR_setAnim(SprMarcheJoe->SpriteA,0);
            }
        }
    }

    // Saut ?!
    if (spr->Saut || spr->Couteau || spr->Direction==2 || spr->Direction==88 || !spr->Direction || !spr->Feu)
    {
        SprMarcheJoe->Visible=0;
        SPR_setVisibility(SprMarcheJoe->SpriteA,HIDDEN);
    }

    // on positionne le sprite relativement à la camera
    CollisionGroundMAP(spr);
    s16 x = fix32ToInt(SprMarcheJoe->CoordX-SprMarcheJoe->OffsetX) - CamPosX;
    s16 y = fix32ToInt(SprMarcheJoe->CoordY+spr->DeltaY-spr->OffsetY) - CamPosY;
    // Affichage
    SPR_setPosition(SprMarcheJoe->SpriteA, x, y);
}

//////////////////////////////////////
//       Apparition Medailles
//////////////////////////////////////

void DisplayMedals()
{
    Sprite1_* SprLvl;
    SprLvl= &NombreLevel;
    SprLvl->Vitesse+=FIX32(0.25);
    SprLvl->CoordX-=SprLvl->Vitesse;
    if (SprLvl->CoordX<=FIX32(-6)) {ApparitionMedailles=0;SprLvl->CoordX=FIX32(-6);}
    // on positionne le sprite relativement à la camera
    s16 x = fix32ToInt(SprLvl->CoordX);
    s16 y = fix32ToInt(SprLvl->CoordY);
    // Affichage
    SPR_setPosition(SprLvl->SpriteA, x, y);
    return;
    }

//////////////////////////////////////
//                 Largage Bombes
//////////////////////////////////////
void GestionBombe(Sprite1_ *spr)
{
    if (spr->ID!=66 || spr->StandBy) return;

    // Bombe larguée !
    if ( !spr->Hit)
    {
        // ID Airplane
        Sprite1_* spr2;
        spr2 = &Sprites[IDPlane];
        fix32 VX=FIX32(0);
        if (spr2->Direction==6) VX=FIX32(0.35);
        else VX=FIX32(-0.35);
        spr->CoordY+=spr->Vitesse;
        spr->CoordX+=VX;
        spr->Vitesse+=FIX32(0.05);
        if (spr->CoordY>=FIX32(140+50))
        {
            spr->Hit=1;
            spr->TempoSprite=0;
            TremblementON=1;
            TempoMax=8;
            TempoTremblement=0;
        }
        return;
    }


     // Explosion au sol !
    if (spr->TempoSprite==10)
    {
        // Massacre au sol !
        // Joueur principal
        u16 NbrIA=NombreIA;
        Sprite1_* SprIA;
        SprIA=&Sprites[IDUnite];
        Sprite1_* SpriteREF;
        SpriteREF = &Sprites[0];
        fix32 DX_Joueur=abs(spr->CoordX - (SpriteREF->CoordX));
        fix32 DY_Joueur=abs(spr->CoordY - (SpriteREF->CoordY+FIX32(48)));
        if (DX_Joueur<=FIX32(32) && DY_Joueur<=FIX32(32) && !SpriteREF->GodMode)
        {
            SND_startPlayPCM_XGM(SFX_GENERIC6, 1, SOUND_PCM_CH2);
            SpriteREF->ClignoSprite=1;
            SpriteREF->ClignoDuree=20;
            SpriteREF->ClignoReach=0;
            SpriteREF->ClignoTic=2;
            SpriteREF->HitPoint-=3;
            if (SpriteREF->HitPoint>200) SpriteREF->HitPoint=0;

            if (!SpriteREF->HitPoint)
            {
                if (!SpriteREF->NombreUP)
                {
                    SpriteREF->HitPoint=0;
                    return;
                }
                SpriteREF->HitPoint=SpriteREF->HitPointMax;
                if (SpriteREF->NombreUP) SpriteREF->NombreUP--;
            }
            GestionHUDSante();
        }
        while(NbrIA--)
        {
            if (!SprIA->StandBy && SprIA->Visible && !SprIA->MortIA && SprIA->ID!=5)
            {
                fix32 DX_IA=abs(spr->CoordX - (SprIA->CoordX));
                if (DX_IA<=FIX32(32))
                {
                    //RandomSeed();
                    if (getRandomU16(100)<50) SprIA->Direction=6;
                    else SprIA->Direction=4;
                    u16 VV=getRandomU16(5);
                    if (VV<2) VV=2;
                    SprIA->Vitesse=FIX32(VV);
                    SprIA->MortIA=2;
                    SprIA->TempoInScene=0;
                    SprIA->InScene=0;
                    SprIA->IntIA=10;
                    //RandomSeed();
                    u16 VV1=getRandomU16(8);
                    if (VV1<4) VV1=4;
                    SprIA->Acceleration=FIX32(VV1);
                    SprIA->RefY=FIX32(240+50);
                    SprIA->SensY=1;
                    SprIA->TirBusy=0;
                    SprIA->TempoSprite=0;
                    if (SprIA->ID==2)
                    {
                        // Bouclier !
                        Sprite1_* SprBou;
                        SprBou=&Sprites[IDBouclier];
                        u8 NrBou=2;
                        while(NrBou--)
                        {
                            if (!SprBou->Visible)
                            {
                                if (SprBou->SpriteA==NULL) SprBou->SpriteA = SPR_addSprite(&Bouclier_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                                SprBou->Visible=1;
                                SprBou->CoordX=SprIA->CoordX;
                                SprBou->CoordY=SprIA->CoordY;
                                SprBou->Acceleration=FIX32(4);
                                SprBou->Direction=0;
                                SprBou->StandBy=0;
                                SprBou->TypeBouclier=1;
                                break;
                            }
                            SprBou++;
                        }
                    }
                    if (SND_isPlayingPCM_XGM(SOUND_PCM_CH2)) SND_startPlayPCM_XGM(SFX_GENERIC5, 1, SOUND_PCM_CH2);
                }
            }
            SprIA++;
        }
    }
}


//////////////////////////////////////
//         Gestion Attaque Helico
//////////////////////////////////////
void GestionAttaqueHelico(Sprite1_ *spr)
{

    if (!spr->Visible) return;

    // Attaque !
    if (!spr->MortIA && spr->Phase==2)
    {

        if (spr->AirUnit==10)
        {
            if (!spr->TirBusy) spr->TirBusy=1;
            // Balle hélico léger
             if (spr->TirBusy  && spr->Vitesse==FIX32(2.4)) GestionBallesIA(spr);
            return;
        }

        // Hélico lourd
        Sprite1_* SpriteREF;
        SpriteREF = &Sprites[0];
        fix32 DX=abs(SpriteREF->CoordX-spr->CoordX);
        if (DX<=FIX32(120)) spr->TempoAggro++;
        if (spr->TempoAggro>(90 - (Difficulte<<2)))
        {
            spr->TempoAggro=0;
            u16 i=4,j=0;
            Sprite1_* spr1;
            spr1 = &Sprites[IDBalle+4];
            while(i--)
            {
                spr1 = &Sprites[IDBalle+j];
                if (spr1->StandBy)
                {
                    spr1->StandBy=0;
                    Bombardement=1;
                    spr1->ID=66;
                    spr1->Hit=0;
                    spr1->TempoSprite=0;
                    SND_startPlayPCM_XGM(SFX_GENERIC8, 1, SOUND_PCM_CH3);
                    SPR_setAnim(spr1->SpriteA,6);
                    spr1->Visible=1;
                    spr1->Vitesse=FIX32(2);
                    spr1->CoordX=spr->CoordX+FIX32(16);
                    spr1->CoordY=spr->CoordY+spr->DeltaY+FIX32(16);
                    spr1->OffsetY=FIX32(-7);
                    spr1->Direction=2;
                    break;
                }
                j++;
            }
        }
    }

    }



///////////////////////////////
//           Ecran de fin
///////////////////////////////
void Result_Screen()
{
	//VDP_init();
	// set all palette to black
    VDP_setPaletteColors(0, (u16*) palette_black, 64);
    memcpy(&palette[0], Palette_JoeEnd.data, 16 * 2);
    memcpy(&palette[16], Palette_Score.data, 16 * 2);
    memcpy(&palette[32],Palette_Airplane.data,16*2);
    memcpy(&palette[48],Palette_Joe.data,16*2);

    SPR_initEx(512);

    // Medaille Difficulté
    if (Difficulte)
    {
        Sprite1_* SprLvl;
        SprLvl= &NombreLevel;
        //SprLvl->CoordX=FIX32(-6);
        SprLvl->CoordX=FIX32(320);
        SprLvl->CoordY=FIX32(0);
        SprLvl->Vitesse=FIX32(5);
        SprLvl->SpriteA = SPR_addSprite(&Medailles_Sprite, 0, 0, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
        SPR_setAnim(SprLvl->SpriteA,Difficulte-1);
        SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
        SPR_setPosition(SprLvl->SpriteA,130,70);
    }


	// Score
    NombreDigitScore=5;
	Sprite1_* SprScore;
    SprScore=&NombreScore[0];
    u8 i=NombreDigitScore;
    u8 j=0;
    u16 Pos=0;
    while(i--)
    {
        Pos=180-(j*16);
        j++;
        SprScore->SpriteA = SPR_addSprite(&Nombre_SpriteXL, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
        SPR_setPriorityAttribut(SprScore->SpriteA, TRUE);
        SPR_setAnim(SprScore->SpriteA,0);
        SPR_setPosition(SprScore->SpriteA,Pos,180);
        SprScore++;
    }


    // Variables
    //Score=1500;
    Sprite1_ *sco;
    char Buffer[5];
    char *Car;

    // Traitement Chaine.
    uintToStr(Score,Texte,0);
    u16 Lon=strlen(Texte);
	strclr(Buffer);
    strcat(Buffer,Texte);

    // Score
    sco=&NombreScore[0];
    i=Lon;
    j=0;
    Pos=0;
    while(i--)
    {
        Pos=180-(j*16);
        j++;
        Car=&Buffer[i];
        //NombreIA+3+6+7+1+6;
        if (*Car=='0') SPR_setAnim(sco->SpriteA,0);
        if (*Car=='1') SPR_setAnim(sco->SpriteA,1);
        if (*Car=='2') SPR_setAnim(sco->SpriteA,2);
        if (*Car=='3') SPR_setAnim(sco->SpriteA,3);
        if (*Car=='4') SPR_setAnim(sco->SpriteA,4);
        if (*Car=='5') SPR_setAnim(sco->SpriteA,5);
        if (*Car=='6') SPR_setAnim(sco->SpriteA,6);
        if (*Car=='7') SPR_setAnim(sco->SpriteA,7);
        if (*Car=='8') SPR_setAnim(sco->SpriteA,8);
        if (*Car=='9') SPR_setAnim(sco->SpriteA,9);
        SPR_setPosition(sco->SpriteA,Pos,180);
        sco++;
    }

	Sprite1_* spr;
	spr = &Sprites[0];
	spr->CoordX=FIX32(100);
	spr->CoordY=FIX32(0);
    // Chargement Sprite Joe
	spr->SpriteA = SPR_addSprite(&JoeEnd_Sprite, 0, 0, TILE_ATTR(PAL0, TRUE, FALSE, FALSE));

	SPR_setPriorityAttribut(spr->SpriteA, TRUE);
	SPR_setVisibility(spr->SpriteA,VISIBLE);
	SPR_setPosition(spr->SpriteA,fix32ToInt(spr->CoordX),fix32ToInt(spr->CoordY));
	SPR_setAnim(spr->SpriteA,0);
	spr++;
	spr->CoordX=FIX32(77);
	spr->CoordY=FIX32(136);
    // Chargement Sprite Joe
	spr->SpriteA = SPR_addSprite(&Score_Sprite, 0, 0, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));

	SPR_setPriorityAttribut(spr->SpriteA, TRUE);
	SPR_setVisibility(spr->SpriteA,VISIBLE);
	SPR_setPosition(spr->SpriteA,fix32ToInt(spr->CoordX),fix32ToInt(spr->CoordY));
	SPR_setAnim(spr->SpriteA,0);

    VDP_fadeInAll(palette,32,FALSE);
    // start music
    XGM_startPlay(Thunder_Music);
    // update sprites
    SPR_update();

    // Vblank
    SYS_doVBlankProcess();
    i=0;
    j=5;

	while(TRUE)
	{
        u16 value=JOY_readJoypad(JOY_1);
        if (value & BUTTON_START ) break;

	    // Clignotement résultat !
	    i++;
        sco=&NombreScore[0];
        while(j--)
        {
            if (i<=8) SPR_setVisibility(sco->SpriteA,VISIBLE);
            else SPR_setVisibility(sco->SpriteA,HIDDEN);
            sco++;
        }
        j=5;
	    if (i>16) i=0;


        SPR_setPosition(spr->SpriteA,fix32ToInt(spr->CoordX),fix32ToInt(spr->CoordY));
        // update sprites
        SPR_update();

		// Vblank
		SYS_doVBlankProcess();
	}
    XGM_pausePlay();
    XGM_stopPlay();
    VDP_fadeOutAll(32,FALSE);
	return;
}

///////////////////////////////
//                  Clignotement
///////////////////////////////
void Cligno_Sprite(Sprite1_ *spr)
{
    // Pas de cligno ?!
    if (!spr->ClignoSprite || spr->ClignoReach) return;
    spr->HitTempo++;
    if (spr->ID==99) spr->GodMode=1;

    // Visibilité ?
    if (spr->ClignoON) spr->Visible=1;
    else spr->Visible=0;

    if (spr->HitTempo==spr->ClignoTic)
    {
        if (!spr->ClignoON) spr->ClignoON=1;
        else spr->ClignoON=0;
        spr->ClignoTempo++;
        spr->HitTempo=0;
    }
    // Fin ?
    if (spr->ClignoTempo==spr->ClignoDuree)
    {
        spr->ClignoON=0;
        spr->HitTempo=0;
        spr->ClignoSprite=0;
        spr->ClignoTempo=0;
        spr->Visible=1;
        spr->ClignoReach=1;
        spr->GodMode=0;
    }
}

///////////////////////////////
//                  Phase Scene
///////////////////////////////
void Phase_Scene(u8 Scene,Sprite1_ *spr)
{
    // MEtal Slug ?!
    if (spr->ID!=10 && spr->ID!=11) return;
    Sprite1_* SprFont;
    SprFont=&Sprites[MaxObjet-2];
    u8 i=0;

    // Création & Chargement Sprites Logo & gestion
    if (spr->ID==11 )
        {
            if (PhaseScene>=2)
            {
                if (PhaseAffichageScene==1)
                {
                    if (SprFont->CoordX<=FIX32(240)) spr->Direction=6;
                    else
                    {
                        PhaseAffichageScene=2;
                        spr->Direction=0;
                    }
                }
                if (PhaseAffichageScene==2)
                {
                    spr->Direction=0;
                    spr->ClignoSprite=1;
                    spr->ClignoDuree=10;
                    spr->ClignoTic=5;
                    spr->Acceleration=FIX32(6);
                    spr->RefY=FIX32(240+24);
                    spr->SensY=1;
                    //spr->Vitesse=FIX32(1);
                    i=6;
                    fix32 j=FIX32(0);
                    Sprite1_* SprFonte;
                    SprFonte=&Sprites[MaxObjet-7];
                    while(i--)
                    {
                        SprFonte->Vitesse=FIX32(2)-j;
                        j+=FIX32(1);
                        SprFonte++;
                    }
                }
                if (PhaseAffichageScene==3)
                {
                    //spr->Visible=1;

                    spr->Direction=4;
                    spr->Saut=1;
                    spr->ClignoSprite=0;
                    // Saut
                    if (spr->Saut) Saut_Sprite(spr);
                }
                if (SprFont->CoordY>=SprFont->RefY && PhaseAffichageScene==3) PhaseAffichageScene=4;
                // On efface Start !
                if (PhaseAffichageScene==4)
                {
                    // On détruit les Fonts 32x32
                    spr->Direction=0;
                    i=MaxObjet;
                    SprFont=&Sprites[0];
                    while(i--)
                    {
                        if ((SprFont!=NULL) && SprFont->ID==11)
                        {
                            SPR_releaseSprite(SprFont->SpriteA);
                            SprFont->SpriteA=NULL;
                            SprFont->Visible=0;
                            SprFont->StandBy=1;
                            PhaseAffichageScene=5;
                             spr->TempoSprite=0;
                        }
                        SprFont++;
                    }
                    MaxObjet-=6;
                }
            }
            return;
        }

    // Id Marco
    Sprite1_* SprMarco;
    SprMarco=&Sprites[0];

    switch(Scene)
    {
    case 1:
        if (spr->CoordX<FIX32(80))
        {
            spr->Direction=6;
            spr->Vitesse=FIX32(0.7);
            PhaseScene=1;
            // Son du tank
            if (!spr->Init)
            {
                spr->Init=1;
                SND_startPlayPCM_XGM(SFX_GENERIC7, 1, SOUND_PCM_CH2);
            }
        }
        if (spr->CoordX>=FIX32(80))
        {
            spr->Direction=0;
            PhaseScene=2;
            SprMarco->Saut=1;
            SprMarco->Acceleration=FIX32(6);
            SprMarco->RefY=FIX32(140-24);
            SprMarco->SensY=1;
            SND_startPlayPCM_XGM(SFX_GENERIC4, 1, SOUND_PCM_CH2);
        }
        SprMarco->CoordX=spr->CoordX+FIX32(16);
        SprMarco->CoordY=spr->CoordY-FIX32(60);

        break;

    case 2:
       if (SprMarco->Saut) Saut_Sprite(SprMarco);
       if (!SprMarco->Saut)
       {
           SprMarco->Direction=0;
           if (SprMarco->CoordY==FIX32(140-24)) PhaseScene=3;
       }
        break;

    case 3:
        spr->Vitesse=FIX32(1.2);
        spr->Direction=6;
        if (spr->CoordX>=FIX32(200) &&  PhaseAffichageScene==2)
        {
            PhaseAffichageScene=3;
            SND_startPlayPCM_XGM(SFX_GENERIC9, 1, SOUND_PCM_CH3);
            TremblementON=1;
            TempoMax=8;
            TempoTremblement=0;
        }

        // Animation pente
        if (spr->CoordX>=FIX32(236) && spr->CoordX<=FIX32(260) && !NumeroZone)
        {
            spr->DeltaY=FIX32(20);
            spr->Transition=1;
        }
        else spr->Transition=0;
        // Son du tank
        if (spr->Init==1)
        {
            spr->Init=2;
            SND_startPlayPCM_XGM(SFX_GENERIC7, 1, SOUND_PCM_CH2);
        }

         if (spr->CoordX>FIX32(320))
         {
            if (spr->SpriteA!=NULL)
            {
                SPR_releaseSprite(spr->SpriteA);
                spr->SpriteA=NULL;
                spr->Visible=0;
                PhaseScene=0;
                Sprite1_* SprAvion;
                SprAvion=&Sprites[IDPlane];
                SprAvion->Visible=1;
                // start music
                XGM_startPlay(Demo_Music);
                return;
            }
         }
        break;
    }
}


///////////////////////////////
//                  Gestion Tir
///////////////////////////////
void GestionTir(Sprite1_ *spr)
{
    // Pas de tir ou couteau en cours ?
    if (!spr->Feu || spr->Couteau) return;

    spr->TempoRafale++;
    u16 Speed=12;
    if (spr->Slot1) Speed=10;
    if (spr->TempoRafale>Speed)
    {
        spr->Feu=0;
        spr->TempoRafale=0;
    }
}


////////////////////////////////////
//                  Gestion Couteau
////////////////////////////////////
void GestionCouteau(Sprite1_ *spr)
{

    if (!spr->Couteau) return;

    spr->TempoCouteau++;

    // Fin animation
    if (spr->TempoCouteau>35)
    {
        spr->Couteau=0;
        spr->TempoCouteau=0;
         spr->Feu=0;
        return;
    }

    // Contact ?
    if (spr->TempoCouteau==25)
    {
        // Test collision IA
        u16 NbrIA=NombreIA;
        Sprite1_* SprIA;
        SprIA=&Sprites[IDUnite];
        while(NbrIA--)
        {
            if (!SprIA->StandBy && SprIA->Visible && !SprIA->MortIA && SprIA->ID!=5)
            {
                fix32 DX_IA=abs(spr->CoordX - SprIA->CoordX);
                switch (spr->MemDir)
                {
                case 6:
                    if (spr->CoordX<(SprIA->CoordX))
                    {
                        if (DX_IA<=FIX32(32))
                        {
                            SprIA->MortIA=2;
                            SprIA->IntIA=0;
                            SprIA->TirBusy=0;
                            SprIA->TempoInScene=0;
                            SprIA->InScene=0;
                            if (SprIA->ID==2)  Score+=35;
                            if (SprIA->ID==3)  Score+=20;
                            if (SprIA->ID==4) Score+=25;
                            if (SprIA->ID==5) Score+=50;
                            if (SprIA->ID==6) {Score+=75;SprIA->MortIA=20;}
                            SprIA->TirBusy=0;
                            SprIA->TempoSprite=0;
                            GestionScore();
                            if (SprIA->ID==2)
                            {
                                // Bouclier !
                                Sprite1_* SprBou;
                                SprBou=&Sprites[IDBouclier];
                                u8 NrBou=2;
                                while(NrBou--)
                                {
                                    if (!SprBou->Visible)
                                    {
                                        if (SprBou->SpriteA==NULL) SprBou->SpriteA = SPR_addSprite(&Bouclier_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                                        SprBou->Visible=1;
                                        SprBou->CoordX=SprIA->CoordX;
                                        SprBou->CoordY=SprIA->CoordY;
                                        SprBou->Acceleration=FIX32(4);
                                        SprBou->Direction=0;
                                        SprBou->StandBy=0;
                                        SprBou->TypeBouclier=1;
                                        break;
                                    }
                                    SprBou++;
                                }
                            }
                            if (SND_isPlayingPCM_XGM(SOUND_PCM_CH2)) SND_startPlayPCM_XGM(SFX_GENERIC5, 1, SOUND_PCM_CH2);
                            return;
                        }
                    }

                    break;

                case 4:
                    if (spr->CoordX>=SprIA->CoordX)
                    {
                        if (DX_IA<=FIX32(48))
                        {
                            SprIA->MortIA=2;
                            SprIA->TempoInScene=0;
                            SprIA->InScene=0;
                            SprIA->TirBusy=0;
                            if (SprIA->ID==2)  Score+=35;
                            if (SprIA->ID==3)  Score+=20;
                            if (SprIA->ID==4) Score+=25;
                            if (SprIA->ID==5) Score+=50;
                            if (SprIA->ID==6) {Score+=75;SprIA->MortIA=20;}
                            GestionScore();
                            if (SprIA->ID==2)
                            {
                                // Bouclier !
                                Sprite1_* SprBou;
                                SprBou=&Sprites[IDBouclier];
                                u8 NrBou=2;
                                while(NrBou--)
                                {
                                    if (!SprBou->Visible)
                                    {
                                        if (SprBou->SpriteA==NULL) SprBou->SpriteA = SPR_addSprite(&Bouclier_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                                        SprBou->Visible=1;
                                        SprBou->CoordX=SprIA->CoordX;
                                        SprBou->CoordY=SprIA->CoordY;
                                        SprBou->Acceleration=FIX32(4);
                                        SprBou->Direction=0;
                                        SprBou->StandBy=0;
                                        SprBou->TypeBouclier=1;
                                        break;
                                    }
                                    SprBou++;
                                }
                            }
                            if (SND_isPlayingPCM_XGM(SOUND_PCM_CH2)) SND_startPlayPCM_XGM(SFX_GENERIC5, 1, SOUND_PCM_CH2);
                            return;
                        }
                    }
                    break;
                }
            }
            SprIA++;
        }
    }
}


/////////////////////////////////////////
//        Gestion Nombre Shotgun
/////////////////////////////////////////
void GestionNombreBallesShotgun()
{
    // Variables
    Sprite1_ *sco;
    char Buffer[2];
    char *Car;

    // Traitement Chaine.
    uintToStr(NombreBalleShotgun,Texte,0);
    u16 Lon=strlen(Texte);
	strclr(Buffer);
    strcat(Buffer,Texte);


    // Score
    sco=&NombreBallesShot[0];
    u8 i=Lon;
    u8 j=0;
    u16 Pos=0;

    // Chiffre 1 digit ?!
    sco++;
    if (Lon==1) SPR_setVisibility(sco->SpriteA,HIDDEN);
    else SPR_setVisibility(sco->SpriteA,VISIBLE);
    sco--;

    // Mode Pistolet ?!
    if (!NombreBalleShotgun)
    {
        SPR_setAnim(sco->SpriteA,10);
        SPR_setPosition(sco->SpriteA,93-8,17);
        return;
    }

    while(i--)
    {
        Pos=93-(j<<3);
        j++;
        Car=&Buffer[i];
        //NombreIA+3+6+7+1+6;
        if (*Car=='0') SPR_setAnim(sco->SpriteA,0);
        if (*Car=='1') SPR_setAnim(sco->SpriteA,1);
        if (*Car=='2') SPR_setAnim(sco->SpriteA,2);
        if (*Car=='3') SPR_setAnim(sco->SpriteA,3);
        if (*Car=='4') SPR_setAnim(sco->SpriteA,4);
        if (*Car=='5') SPR_setAnim(sco->SpriteA,5);
        if (*Car=='6') SPR_setAnim(sco->SpriteA,6);
        if (*Car=='7') SPR_setAnim(sco->SpriteA,7);
        if (*Car=='8') SPR_setAnim(sco->SpriteA,8);
        if (*Car=='9') SPR_setAnim(sco->SpriteA,9);
        SPR_setPosition(sco->SpriteA,Pos,17);
        sco++;
    }
}


/////////////////////////////////////////
//        Gestion Nombre Grenades
/////////////////////////////////////////
void GestionNombreGrenades()
{
    // Variables
    Sprite1_ *sco;
    char Buffer[2];
    char *Car;

    // Traitement Chaine.
    uintToStr(NombreGrenade,Texte,0);
    u16 Lon=strlen(Texte);
	strclr(Buffer);
    strcat(Buffer,Texte);

    // Score
    sco=&NombreGrenades[0];
    u8 i=Lon;
    u8 j=0;
    u16 Pos=0;

    // Chiffre 1 digit ?!
    sco++;
    if (Lon==1) SPR_setVisibility(sco->SpriteA,HIDDEN);
    else SPR_setVisibility(sco->SpriteA,VISIBLE);
    sco--;

    while(i--)
    {
        Pos=118-(j<<3);
        j++;
        Car=&Buffer[i];
        //NombreIA+3+6+7+1+6;
        if (*Car=='0') SPR_setAnim(sco->SpriteA,0);
        if (*Car=='1') SPR_setAnim(sco->SpriteA,1);
        if (*Car=='2') SPR_setAnim(sco->SpriteA,2);
        if (*Car=='3') SPR_setAnim(sco->SpriteA,3);
        if (*Car=='4') SPR_setAnim(sco->SpriteA,4);
        if (*Car=='5') SPR_setAnim(sco->SpriteA,5);
        if (*Car=='6') SPR_setAnim(sco->SpriteA,6);
        if (*Car=='7') SPR_setAnim(sco->SpriteA,7);
        if (*Car=='8') SPR_setAnim(sco->SpriteA,8);
        if (*Car=='9') SPR_setAnim(sco->SpriteA,9);
        SPR_setPosition(sco->SpriteA,Pos,17);
        sco++;
    }
}


/////////////////////////////////////////
//                  Gestion Score
/////////////////////////////////////////
void GestionScore()
{

    // Variables
    Sprite1_ *sco;
    char Buffer[5];
    char *Car;

    // Augmentation de la difficulté / Score
    if (Score>=750 && Score<=1500 && !Difficulte)
    {
        Sprite1_* SprLvl;
        ApparitionMedailles=1;
        SprLvl= &NombreLevel;
        SPR_setAnim(SprLvl->SpriteA,0);
        SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
        if (!Difficulte)  SND_startPlayPCM_XGM(SFX_GENERIC11, 1, SOUND_PCM_CH4);
        Difficulte=1;
    }
    if (Score>1500 && Score<=2250 && Difficulte==1)
    {
        Sprite1_* SprLvl;
        SprLvl= &NombreLevel;
        SprLvl->CoordX=FIX32(320);
        ApparitionMedailles=1;
        SPR_setAnim(SprLvl->SpriteA,1);
        SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
        ApparitionMedailles=1;
        if (Difficulte==1)  SND_startPlayPCM_XGM(SFX_GENERIC11, 1, SOUND_PCM_CH4);
        Difficulte=2;
    }
    if (Score>2250 && Score<=3000 && Difficulte==2)
    {
        Sprite1_* SprLvl;
        SprLvl= &NombreLevel;
        SprLvl->CoordX=FIX32(320);
        SPR_setAnim(SprLvl->SpriteA,2);
        SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
        ApparitionMedailles=1;
        if (Difficulte==2)  SND_startPlayPCM_XGM(SFX_GENERIC11, 1, SOUND_PCM_CH4);
        Difficulte=3;
    }
    if (Score>3000 && Score<=3750 && Difficulte==3)
    {
        Sprite1_* SprLvl;
        SprLvl= &NombreLevel;
        SprLvl->CoordX=FIX32(320);
        SPR_setAnim(SprLvl->SpriteA,3);
        SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
        ApparitionMedailles=1;
        if (Difficulte==3)  SND_startPlayPCM_XGM(SFX_GENERIC11, 1, SOUND_PCM_CH4);
        Difficulte=4;
    }
    if (Score>3750 && Score<=4500 && Difficulte==4)
    {
        Sprite1_* SprLvl;
        SprLvl= &NombreLevel;
        SprLvl->CoordX=FIX32(320);
        SPR_setAnim(SprLvl->SpriteA,4);
        SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
        ApparitionMedailles=1;
        if (Difficulte==4)  SND_startPlayPCM_XGM(SFX_GENERIC11, 1, SOUND_PCM_CH4);
        Difficulte=5;
    }
    if (Score>4500 && Difficulte==5 && Score<=5400)
    {
        Sprite1_* SprLvl;
        SprLvl= &NombreLevel;
        SprLvl->CoordX=FIX32(320);
        SPR_setAnim(SprLvl->SpriteA,5);
        SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
        ApparitionMedailles=1;
        if (Difficulte==5)  SND_startPlayPCM_XGM(SFX_GENERIC11, 1, SOUND_PCM_CH4);
        Difficulte=6;
    }
    if (Score>5400 && Difficulte==6 && Score<=6400)
    {
        Sprite1_* SprLvl;
        SprLvl= &NombreLevel;
        SprLvl->CoordX=FIX32(320);
        SPR_setAnim(SprLvl->SpriteA,6);
        SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
        ApparitionMedailles=1;
        if (Difficulte==6)  SND_startPlayPCM_XGM(SFX_GENERIC11, 1, SOUND_PCM_CH4);
        Difficulte=7;
    }
    if (Score>6400 && Difficulte==7 && Score<=7500)
    {
        Sprite1_* SprLvl;
        SprLvl= &NombreLevel;
        SprLvl->CoordX=FIX32(320);
        SPR_setAnim(SprLvl->SpriteA,7);
        SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
        ApparitionMedailles=1;
        if (Difficulte==7)  SND_startPlayPCM_XGM(SFX_GENERIC11, 1, SOUND_PCM_CH4);
        Difficulte=8;
    }
    if (Score>7500 && Difficulte==8 && Score<=9000)
    {
        Sprite1_* SprLvl;
        SprLvl= &NombreLevel;
        SprLvl->CoordX=FIX32(320);
        SPR_setAnim(SprLvl->SpriteA,8);
        SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
        ApparitionMedailles=1;
        if (Difficulte==8)  SND_startPlayPCM_XGM(SFX_GENERIC11, 1, SOUND_PCM_CH4);
        Difficulte=9;
    }
    if (Score>9000 && Difficulte==9)
    {
        Sprite1_* SprLvl;
        SprLvl= &NombreLevel;
        SprLvl->CoordX=FIX32(320);
        SPR_setAnim(SprLvl->SpriteA,9);
        SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
        ApparitionMedailles=1;
        if (Difficulte==9)  SND_startPlayPCM_XGM(SFX_GENERIC11, 1, SOUND_PCM_CH4);
        Difficulte=10;
    }

    // Traitement Chaine.
    uintToStr(Score,Texte,0);
    u16 Lon=strlen(Texte);
	strclr(Buffer);
    strcat(Buffer,Texte);

    // Score
    sco=&NombreScore[0];
    u8 i=Lon;
    u8 j=0;
    u16 Pos=0;
    while(i--)
    {
        Pos=64-(j<<3);
        j++;
        Car=&Buffer[i];
        //NombreIA+3+6+7+1+6;
        if (*Car=='0') SPR_setAnim(sco->SpriteA,0);
        if (*Car=='1') SPR_setAnim(sco->SpriteA,1);
        if (*Car=='2') SPR_setAnim(sco->SpriteA,2);
        if (*Car=='3') SPR_setAnim(sco->SpriteA,3);
        if (*Car=='4') SPR_setAnim(sco->SpriteA,4);
        if (*Car=='5') SPR_setAnim(sco->SpriteA,5);
        if (*Car=='6') SPR_setAnim(sco->SpriteA,6);
        if (*Car=='7') SPR_setAnim(sco->SpriteA,7);
        if (*Car=='8') SPR_setAnim(sco->SpriteA,8);
        if (*Car=='9') SPR_setAnim(sco->SpriteA,9);
        SPR_setPosition(sco->SpriteA,Pos,8);
        sco++;
    }
}


/////////////////////////////////////////
//                  Gestion UP
/////////////////////////////////////////
void GestionUP(Sprite1_ *spr)
{
    // Variables
    Sprite1_ *sco;
    char Buffer[5];
    char *Car;

    // Traitement Chaine.
    uintToStr(spr->NombreUP,Texte,0);
	strclr(Buffer);
    strcat(Buffer,Texte);

    // Nombre UP
    sco=&NombreUP;

    Car=&Buffer[0];
    if (*Car=='0') SPR_setAnim(sco->SpriteA,0);
    if (*Car=='1') SPR_setAnim(sco->SpriteA,1);
    if (*Car=='2') SPR_setAnim(sco->SpriteA,2);
    if (*Car=='3') SPR_setAnim(sco->SpriteA,3);
    if (*Car=='4') SPR_setAnim(sco->SpriteA,4);
    if (*Car=='5') SPR_setAnim(sco->SpriteA,5);
    if (*Car=='6') SPR_setAnim(sco->SpriteA,6);
    if (*Car=='7') SPR_setAnim(sco->SpriteA,7);
    if (*Car=='8') SPR_setAnim(sco->SpriteA,8);
    if (*Car=='9') SPR_setAnim(sco->SpriteA,9);    // Sécurité
    if (spr->StandBy  || spr->Hit ||GameOver)  return;
    if (spr->SpriteA==NULL) return;
    SPR_setPosition(sco->SpriteA,52,24);
}



/////////////////////////////////////////
//                  Gestion Grenades
/////////////////////////////////////////
void GestionGrenades(Sprite1_ *spr)
{
    // Sécurité
    if (spr->ID!=88 || spr->StandBy) return;
    if (GameOver)  return;
    if (spr->SpriteA==NULL) return;


    // Au contact ?! BOUM !
    if (!spr->TypeIA && !spr->Hit)
    {
        u16 NbrIA=NombreIA;
        Sprite1_* SprIA;
        SprIA=&Sprites[IDUnite];
        while(NbrIA--)
        {
            if (!SprIA->StandBy && SprIA->Visible && !SprIA->MortIA && SprIA->ID!=5)
            {
                fix32 DX_IA=abs(spr->CoordX - (SprIA->CoordX));
                if (DX_IA<=FIX32(16))
                {
                    spr->Hit=1;
                    spr->Direction=0;
                    spr->TempoSprite=0;
                    spr->Saut=0;
                    break;
                }
            }
            SprIA++;
        }
    }

    // Explosion ?!
    if (spr->Hit)
        {
            if (spr->TempoSprite==10)
            {
                // Massacre au sol !
                // Marco ?!
                if (spr->TypeIA)
                {
                    // ID Ref joueur
                    Sprite1_* SpriteREF;
                    SpriteREF = &Sprites[0];
                    fix32 DX_IA=abs(spr->CoordX - SpriteREF->CoordX);
                    fix32 DY_IA=abs(spr->CoordY - (SpriteREF->CoordY+FIX32(48)));
                    // Cas de collision.
                    if (DX_IA<=FIX32(32) &&  DY_IA<=FIX32(30) && !SpriteREF->GodMode)
                    {
                        SND_startPlayPCM_XGM(SFX_GENERIC6, 1, SOUND_PCM_CH2);
                        SpriteREF->ClignoSprite=1;
                        SpriteREF->ClignoDuree=20;
                        SpriteREF->ClignoReach=0;
                        SpriteREF->ClignoTic=2;
                        SpriteREF->HitPoint-=2;
                        if (SpriteREF->HitPoint>200) SpriteREF->HitPoint=0;
                        if (!SpriteREF->HitPoint)
                        {
                            if (!SpriteREF->NombreUP)
                            {
                                SpriteREF->HitPoint=0;
                                return;
                            }
                            SpriteREF->HitPoint=SpriteREF->HitPointMax;
                            if (SpriteREF->NombreUP) SpriteREF->NombreUP--;
                        }
                        GestionHUDSante();
                    }
                }

                // Joueur principal
                u16 NbrIA=NombreIA;
                Sprite1_* SprIA;
                SprIA=&Sprites[IDUnite];
                while(NbrIA--)
                {
                    if (!SprIA->StandBy && !SprIA->MortIA && SprIA->ID!=5 )
                    {
                        fix32 DX_IA=abs(spr->CoordX - (SprIA->CoordX));
                        if (DX_IA<=FIX32(48))
                        {
                            //RandomSeed();
                            if (getRandomU16(100)<50) SprIA->Direction=6;
                            else SprIA->Direction=4;
                            u16 VV=getRandomU16(5);
                            if (VV<2) VV=2;
                            SprIA->Vitesse=FIX32(VV);
                            SprIA->MortIA=2;
                            SprIA->TempoInScene=0;
                            SprIA->InScene=0;
                            SprIA->IntIA=10;
                            //RandomSeed();
                            u16 VV1=getRandomU16(8);
                            if (VV1<4) VV1=4;
                            SprIA->Acceleration=FIX32(VV1);
                            SprIA->RefY=FIX32(240+50);
                            SprIA->SensY=1;
                            SprIA->TirBusy=0;
                            SprIA->TempoSprite=0;
                            if (!spr->TypeIA)
                            {
                                if (SprIA->ID==2)  Score+=35+(Difficulte<<2);
                                if (SprIA->ID==3)  Score+=20+(Difficulte<<2);
                                if (SprIA->ID==4) Score+=25+(Difficulte<<2);
                                if (SprIA->ID==5) Score+=50+(Difficulte<<2);
                                GestionScore();
                            }
                            if (SprIA->ID==2)
                            {
                                // Bouclier !
                                Sprite1_* SprBou;
                                SprBou=&Sprites[IDBouclier];
                                u8 NrBou=2;
                                while(NrBou--)
                                {
                                    if (!SprBou->Visible)
                                    {
                                        if (SprBou->SpriteA==NULL) SprBou->SpriteA = SPR_addSprite(&Bouclier_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                                        SprBou->Visible=1;
                                        SprBou->CoordX=SprIA->CoordX;
                                        SprBou->CoordY=SprIA->CoordY;
                                        SprBou->Acceleration=FIX32(4);
                                        SprBou->Direction=0;
                                        SprBou->StandBy=0;
                                        SprBou->TypeBouclier=1;
                                        break;
                                    }
                                    SprBou++;
                                }
                            }
                            if (SND_isPlayingPCM_XGM(SOUND_PCM_CH2)) SND_startPlayPCM_XGM(SFX_GENERIC5, 1, SOUND_PCM_CH2);
                        }
                    }
                    SprIA++;
                }
            }
        }

    // Physique
    //if (spr->Saut)
    //{
        //MoveSprite(spr);
        //Saut_Sprite(spr);
    //}

    return;
}


/////////////////////////////////////////
//                  Gestion Balles
/////////////////////////////////////////
void GestionBalles(Sprite1_ *spr, Sprite1_ *SpriteREF)
{
    // Sécurité
    if (spr->StandBy  || spr->Hit || GameOver || spr->ID==88 || spr->ID==66)  return;
    if (spr->SpriteA==NULL) return;

    // Game Over ?!
    if (!SpriteREF->NombreUP && !SpriteREF->HitPoint )
    {
        GameOver=1;
        SpriteREF->HitPoint=0;
        GestionHUDSante();
        SpriteREF->NombreUP=0;
        SpriteREF->Direction=0;
        SpriteREF->Sprint=0;
        SpriteREF->Saut=1;
        SpriteREF->Acceleration=FIX32(6);
        SpriteREF->RefY=FIX32(240);
        SpriteREF->SensY=1;
        return;
    }

    // Couteau ?!
    if (SpriteREF->Couteau) return;

    // Balles Joueurs
    if (spr->ID==45)
    {
        // Test collision IA
        u16 NbrIA=NombreIA;
        Sprite1_* SprIA;
        SprIA=&Sprites[IDPlane];

        if (SprIA->Phase==2)
        {
            // Helico ?!
            fix32 DX_IA=abs(spr->CoordX - (SprIA->CoordX+FIX32(16)));
            fix32 DY_IA=abs(spr->CoordY - (SprIA->CoordY+FIX32(16)));
            fix32 C=FIX32(44);
            if (SprIA->AirUnit==10) C=FIX32(24);
            if (DX_IA<=C && DY_IA<=FIX32(24) && !SprIA->MortIA)
            {
                spr->Hit=1;
                spr->Direction=0;
                if (!SpriteREF->Slot1) SprIA->HitPoint--;
                else  SprIA->HitPoint-=3;
                if (SprIA->HitPoint>100) SprIA->HitPoint=0;
                if (!SprIA->HitPoint)
                {
                    SprIA->MortIA=1;
                    SprIA->TempoInScene=0;
                    SprIA->InScene=0;
                    SprIA->TempoSprite=0;
                    if (spr->AirUnit==1) Score+=500+(Difficulte<<2);
                    else Score+=350+(Difficulte<<2);
                    SprIA->Transition=0;
                    SprIA->CalculTransition=0;
                    GestionScore();
                    XGM_startPlay(Demo_Music);
                }
                VDP_setPalette(PAL3, BlankPalette);
                SprIA->Hit=1;
                return;
            }
        }

        SprIA=&Sprites[IDUnite];
        while(NbrIA--)
        {
            if (!SprIA->StandBy && !SprIA->MortIA && SprIA->ID!=6)
            {
                fix32 DX=FIX32(0);
                if (spr->Direction==6)  DX=FIX32(-12);
                else DX=FIX32(12);
                if (SprIA->ID==5 || SprIA->AirUnit==10) DX=FIX32(12);
                fix32 DX_IA=abs(spr->CoordX - (SprIA->CoordX-DX));
                fix32 DY_IA=abs(spr->CoordY - (SprIA->CoordY+SprIA->Spotting));
                u8 TES=0;
                // Test Balle <-> Unités
                if (spr->Direction==6 && (SpriteREF->CoordX>SprIA->CoordX)) TES=1;
                if (spr->Direction==4 && (SpriteREF->CoordX<SprIA->CoordX)) TES=1;

                if (DX_IA<=FIX32(20) && DY_IA<=FIX32(24) && !TES)
                {
                    if (!SpriteREF->Slot1) SprIA->HitPoint--;
                    else  SprIA->HitPoint-=3;
                    if (SprIA->HitPoint>100) SprIA->HitPoint=0;
                    if (spr->Direction==6) spr->OffsetX=FIX32(0);
                    else spr->OffsetX=FIX32(-16);

                    if (!SprIA->HitPoint)
                    {
                        // Les soldats bouclier se cassent !!
                        if (SprIA->ID==2 && !SprIA->IAFuite)
                        {
                            spr->Hit=0;
                            spr->StandBy=1;
                            spr->Visible=0;
                            SprIA->IAFuite=1;
                            SprIA->HitPoint++;
                            switch (SprIA->Direction)
                            {
                            case 4:
                                SprIA->Direction=6;
                                break;

                            case 6:
                                SprIA->Direction=4;
                                break;
                            }

                            if (SprIA->Direction==98) SprIA->Direction=4;
                            if (!SprIA->Direction) SprIA->Direction=6;

                            SprIA->Vitesse=FIX32(2.5);
                            // Bouclier !
                            Sprite1_* SprBou;
                            SprBou=&Sprites[IDBouclier];
                            u8 NrBou=2;
                            while(NrBou--)
                            {
                                if (!SprBou->Visible)
                                {
                                    if (SprBou->SpriteA==NULL) SprBou->SpriteA = SPR_addSprite(&Bouclier_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
                                    SprBou->Visible=1;
                                    SprBou->CoordX=SprIA->CoordX;
                                    SprBou->CoordY=SprIA->CoordY;
                                    SprBou->Acceleration=FIX32(4);
                                    SprBou->Direction=0;
                                    SprBou->StandBy=0;
                                    SprBou->TypeBouclier=1;
                                    break;
                                }
                                SprBou++;
                            }
                            return;
                        }
                        SprIA->MortIA=1;
                        SprIA->TempoInScene=0;
                        SprIA->InScene=0;
                        spr->TempoSprite=0;
                        if (SpriteREF->Slot1) {SprIA->IntIA=12;SprIA->Direction=spr->Direction;}
                        else SprIA->IntIA=0;
                        spr->Hit=2;
                        if (SpriteREF->Slot1) spr->Hit=8;
                        if (SprIA->ID==2)  Score+=35+(Difficulte<<2);
                        if (SprIA->ID==3)  Score+=20+(Difficulte<<2);
                        if (SprIA->ID==4) Score+=25+(Difficulte<<2);
                        if (SprIA->ID==5) Score+=50+(Difficulte<<2);
                        GestionScore();
                        spr->Direction=0;
                        if (SND_isPlayingPCM_XGM(SOUND_PCM_CH2))
                        {
                            if (getRandomU16(100)<50) SND_startPlayPCM_XGM(SFX_GENERIC2, 1, SOUND_PCM_CH2);
                            else SND_startPlayPCM_XGM(SFX_GENERIC3, 1, SOUND_PCM_CH2);
                        }
                        return;
                    }
                    else
                    {
                        spr->Hit=1;
                        spr->Direction=0;
                        return;
                    }
                }
            }
            SprIA++;
        }
        return;
    }

    // Balles IA
    if (!SpriteREF->GodMode)
    {

        // Balles des Paras
        if (spr->IDList==2)
        {
            //fix32 DY=FIX32(44);
            //if (SpriteREF->Direction==24 || SpriteREF->Direction==26 || SpriteREF->Direction==2)  DY=FIX32(60);
            fix32 DCX;
            fix32 DCY;
            if (spr->Direction==42) {DCX=FIX32(8);DCY=FIX32(0);}
            if (spr->Direction==36) {DCX=FIX32(-12);DCY=FIX32(0);}
            if (spr->Direction==14) {DCX=FIX32(32);DCY=FIX32(0);}
            fix32 CX_IA=spr->CoordX+FIX32(13);
            fix32 CY_IA=spr->CoordY-FIX32(13);
            fix32 CX_Player=SpriteREF->CoordX+DCX;
            fix32 CY_Player=SpriteREF->CoordY-DCY;
            fix32 DX_IA=abs(CX_IA-CX_Player);
            fix32 DY_IA=abs(CY_IA-CY_Player);
            fix32 C=FIX32(16);
            if (spr->SpeIA) C=FIX32(24);

            if (DX_IA<=C && DY_IA<=FIX32(16) && !spr->Hit)
            {
                spr->Hit=2;
                SpriteREF->ClignoSprite=1;
                SpriteREF->ClignoDuree=20;
                SpriteREF->ClignoReach=0;
                SpriteREF->ClignoTic=2;
                spr->Direction=0;
                SND_startPlayPCM_XGM(SFX_GENERIC6, 1, SOUND_PCM_CH2);
                if (!spr->SpeIA) SpriteREF->HitPoint--;
                else SpriteREF->HitPoint-=2;
                if (SpriteREF->HitPoint>200) SpriteREF->HitPoint=0;
                if (!SpriteREF->HitPoint)
                {
                    if (!SpriteREF->NombreUP)
                    {
                        SpriteREF->HitPoint=0;
                        return;
                    }
                    SpriteREF->HitPoint=SpriteREF->HitPointMax;
                    if (SpriteREF->NombreUP) SpriteREF->NombreUP--;
                }
                GestionHUDSante();
            }
            return;
        }

        fix32 DX_IA=abs(spr->CoordX - SpriteREF->CoordX);
        fix32 DY_IA=abs(spr->CoordY - (SpriteREF->CoordY+FIX32(48)));
        u8 COL=1;
        u8 Test=0;
        fix32 PosYJoueur=(SpriteREF->CoordY+SpriteREF->DeltaY+FIX32(48));

        // Cas de collision.
        if (spr->CoordY==PosYJoueur)
        {
            if (SpriteREF->Direction==2 || SpriteREF->Direction==24 || SpriteREF->Direction==26) COL=0;
            if (SpriteREF->Couteau) COL=1;
            Test=1;
        }

        if (spr->CoordY<PosYJoueur && !Test)
        {
            if (SpriteREF->Direction==2 || SpriteREF->Direction==24 || SpriteREF->Direction==26) COL=0;
            if (SpriteREF->Couteau) COL=1;
            Test=1;
        }
        if (spr->CoordY>PosYJoueur && !Test)
        {
            if (SpriteREF->Direction==2 || SpriteREF->Direction==24 || SpriteREF->Direction==26) COL=1;
            if (SpriteREF->Couteau) COL=1;
        }
        if (DX_IA<=FIX32(8) && DY_IA<=FIX32(30) && COL)
        {
            spr->Hit=2;
            SpriteREF->ClignoSprite=1;
            SpriteREF->ClignoDuree=20;
            SpriteREF->ClignoReach=0;
            SpriteREF->ClignoTic=2;
            spr->Direction=0;
            SND_startPlayPCM_XGM(SFX_GENERIC6, 1, SOUND_PCM_CH2);
            if (SpriteREF->HitPoint) SpriteREF->HitPoint--;

            if (!SpriteREF->HitPoint)
            {
                if (!SpriteREF->NombreUP)
                {
                    SpriteREF->HitPoint=0;
                    return;
                }
                SpriteREF->HitPoint=SpriteREF->HitPointMax;
                if (SpriteREF->NombreUP) SpriteREF->NombreUP--;
            }
            GestionHUDSante();
        }
    }

}

//////////////////////////////////////
//                  Gestion HUD Santé
//////////////////////////////////////
void GestionHUDSante()
{
	Sprite1_* SprHUD;
	SprHUD= & SpriteHUD[0];

	Sprite1_* SpriteRef;
	SpriteRef=&Sprites[0];

	SPR_setAnim(SprHUD->SpriteA,6-SpriteRef->HitPoint);
}


///////////////////////////////
//        Balles IA
///////////////////////////////
void GestionBallesIA(Sprite1_ *spr)
{
    // mort pendant le tir ?
    if (spr->MortIA)
    {
        spr->TirBusy=0;
        spr->TempoSprite=0;
        return;
    }
    spr->TempoRafale++;
    // Son tir et gestion Balle.
    if (spr->TempoRafale==30)
    {
        if (spr->IAFuite)
        {
             spr->TempoRafale=0;
             spr->TirBusy=0;
             return;
        }
        u16 i=4,j=0,Nb=IDBalle+4,h=0;
        Sprite1_* spr1;
        spr1 = &Sprites[Nb];
        while(i--)
        {
            spr1 = &Sprites[Nb+j];
            if (spr1->StandBy)
            {
                spr1->StandBy=0;
                spr1->ID=46;
                spr1->Hit=0;
                if (Difficulte==1) spr->Boost=FIX32(0.25);
                if (Difficulte==2) spr->Boost=FIX32(0.35);
                if (Difficulte==3) spr->Boost=FIX32(0.45);
                if (Difficulte==4) spr->Boost=FIX32(0.55);

                // Hélico de combat  Tir x 2 !
                if (spr->AirUnit==10)
                {
                    spr1->Vitesse=FIX32(2.5);
                    if (spr->SpeIA) spr1->Vitesse=FIX32(3);
                    spr1->Visible=1;
                    spr1->SpeIA=spr->SpeIA;
                    spr1->IDList=2;
                    spr->TirBusy=2;
                    spr->TempoChauffeArme=0;
                    spr1->CoordX=spr->CoordX;
                    spr1->DeltaY=FIX32(0);
                    spr1->CoordY=spr->CoordY;
                    spr1->OffsetY=FIX32(-64);
                    if (spr->Direction==4)
                    {
                        spr1->Direction=14;
                        if (!h) {spr1->OffsetX=FIX32(-10);SND_startPlayPCM_XGM(SFX_GENERIC10, 1, SOUND_PCM_CH2);}
                        else spr1->OffsetX=FIX32(12);
                    }
                    else
                    {
                        spr1->Direction=16;
                        if (!h) {spr1->OffsetX=FIX32(-38);SND_startPlayPCM_XGM(SFX_GENERIC10, 1, SOUND_PCM_CH2);}
                        else spr1->OffsetX=FIX32(-20);
                    }
                    if (h) break;
                    h++;
                }
                else
                {
                    // Tir du soldat Bouclier
                    if (spr->ID!=5)
                    {
                        SND_startPlayPCM_XGM(SFX_GENERIC1, 1, SOUND_PCM_CH2);
                        spr1->Vitesse=FIX32(3)+spr->Boost;
                        spr1->Visible=1;
                        spr1->IDList=1;
                        spr1->SpeIA=0;
                        spr1->CoordX=spr->CoordX;
                        spr1->CoordY=spr->CoordY+spr->DeltaY;
                        spr1->OffsetY=FIX32(-7);
                        spr1->Direction=spr->Direction;
                        if (spr->Direction==0) spr1->Direction=4;
                        if (spr->Direction==98) spr1->Direction=6;
                        if (spr->Direction==4) spr1->OffsetX=FIX32(8);
                        else  spr1->OffsetX=FIX32(0);
                        break;
                    }
                    // Tir Para
                    else if (spr->ID==5)
                    {
                        SND_startPlayPCM_XGM(SFX_GENERIC10, 1, SOUND_PCM_CH2);
                        spr1->Vitesse=FIX32(1.5)+spr->Boost;
                        spr1->Visible=1;
                        spr1->IDList=2;
                        spr1->SpeIA=0;
                        spr1->CoordX=spr->CoordX;
                        spr1->DeltaY=FIX32(0);
                        spr1->CoordY=spr->CoordY;
                        spr1->OffsetX=FIX32(-10);
                        spr1->OffsetY=FIX32(-64);
                        if (!spr->Animation) spr1->Direction=42;
                        if (spr->Animation==6) {spr1->Direction=36;spr1->OffsetX=FIX32(-24);spr1->OffsetY=FIX32(-56);}
                        if (spr->Animation==4) {spr1->Direction=14;spr1->OffsetX=FIX32(24);spr1->OffsetY=FIX32(-56);}
                        break;
                    }
                }
            }
            j++;
        }
    }
    if (spr->TempoRafale>70)
    {
        spr->TempoRafale=0;
        spr->TempoCountRafale=0;
        spr->TirBusy=0;
    }
}


///////////////////////////////
//                  Gestion IA
///////////////////////////////
void GestionIA(Sprite1_  *spr)
{
    // On vire le joueur et l'airplane.
    if (spr->StandBy || !spr->SpriteDYN) return;

	Sprite1_* SpriteREF;
	SpriteREF=&Sprites[0];

    // Mort IA ?!
    if (spr->MortIA)
    {
        spr->TempoSprite++;
        // Civil ?!
        if (spr->ID==6)
        {
            // Libéré ?!
            if (spr->MortIA==20)
            {
                Sprite1_* SpriteB;
                SpriteB = &Sprites[IDBonus];
                if (spr->TempoSprite==35)
                {
                    SpriteB->CoordX=spr->CoordX;
                    SpriteB->CoordY=spr->CoordY+FIX32(16);
                    SpriteB->Visible=1;
                    SND_startPlayPCM_XGM(SFX_GENERIC15, 2, SOUND_PCM_CH4);
                    SpriteB->StandBy=0;
                    UpdateSprite(SpriteB);
                    // Attribution des bonus !
                    //RandomSeed();
                    while (TRUE)
                    {
                        if (getRandomU16(100)<15) {SpriteB->Transition=4;break;}
                        if (NombreBalleShotgun<15 && getRandomU16(100)<25) {SpriteB->Transition=1;break;}
                        if (!SpriteREF->Slot1 && getRandomU16(100)<35) {SpriteB->Transition=1;break;}
                        if (SpriteREF->HitPoint<3 && getRandomU16(100)<40) {SpriteB->Transition=2;break;}
                        if (NombreGrenade<6 && getRandomU16(100)<45) {SpriteB->Transition=0;break;}
                        SpriteB->Transition=3;
                        break;
                    }
                }
                if (spr->TempoSprite==2) SND_startPlayPCM_XGM(SFX_GENERIC13, 2, SOUND_PCM_CH4);
                if  (spr->TempoSprite>80)
                {
                    spr->MortIA=21;
                    SpriteB->Init=1;
                    GoCivil=0;
                }
            }
            // Fuite !!
            if (spr->MortIA==21)
            {
                spr->Direction=4;
                spr->Vitesse=FIX32(2.8);
                MoveSprite(spr);
                CollisionGroundMAP(spr);
            }
            return;
        }

        // Para ?
        if (spr->ID==5 )
        {
            spr->Direction=42;
            spr->Vitesse+=FIX32(0.2);
            MoveSprite(spr);
            //spr->Acceleration+=FIX32(1);
            //spr->Vitesse+=spr->Acceleration;
        }
        u8 T1=100;
        // Les victimes du Heavy MC reculent et explosent à l'impact !
        if (spr->IntIA==12 && spr->ID!=5)
        {
            if (spr->Vitesse>FIX32(0)) spr->Vitesse-=FIX32(0.1);
            MoveSprite(spr);
            T1=40;
        }
        if (spr->TempoSprite>T1)
        {
            spr->TempoSprite=0;
            spr->MortIA=0;
            spr->TempoInScene=0;
            spr->InScene=0;
            spr->IntIA=0;
            spr->Saut=0;
            //spr->Visible=0;
            if (spr->SpriteA!=NULL)
            {
                SPR_releaseSprite(spr->SpriteA);
                NombreIAScene--;
                if (NombreIAScene>32760) NombreIAScene=0;
                spr->SpriteA=NULL;
                spr->StandBy=1;
                if (spr->ID==4) GrenadierON=0;
                if (spr->ID==5)
                {
                    if (NombrePara) NombrePara--;
                }
                spr->Visible=0;
                spr->IntIA=0;
            }
        }
        return;
    }

    // Fuite ?
    if (spr->IAFuite || spr->MortIA) return;

    //Game Over ?
    if (GameOver)
    {
        spr->TirBusy=0;
        spr->CoupCrosse=0;
        if (spr->Direction==4) spr->Direction=6;
        if (spr->Direction==6) spr->Direction=4;
        if (!spr->Direction) spr->Direction=6;
        return;
    }

    if (spr->ID==6) return;


    // Les unités Snipers tirent quand elles sont en position
    if (spr->ID==2 && spr->Reach && !spr->TirBusy)
    {
        spr->TempoCountRafale++;
        if (spr->TempoCountRafale>spr->TempoCountRafaleMax)
        {
            spr->TempoCountRafale=0;
            if (spr->IAFuite) return;
            spr->TirBusy=1;
        }
    }

    // Algoririthme de traitement
    // Distance joueur / IA
    fix32 DistanceIA;
    DistanceIA=abs(SpriteREF->CoordX - (spr->CoordX+FIX32(16)));

    // Cas des parachutistes
    if (spr->ID==5)
    {
        // Tir !  Les paras ne tirent que si ils sont au dessus du joueur.
        fix32 CYR=SpriteREF->CoordY+FIX32(128);
        if (!spr->TirBusy && spr->CoordY<=CYR)
        {
            spr->TempoCountRafale++;
            if (spr->TempoCountRafale>65-(Difficulte<<2))
            {
                spr->TempoCountRafale=0;
                spr->TirBusy=1;
            }
        }

        //   Déplacement en l'air
        spr->Direction=42;
        spr->TempoCouvert++;
        if (spr->TempoCouvert<=200) spr->VitesseInit=FIX32(0.2);
        else spr->VitesseInit=FIX32(-0.2);
        if (spr->TempoCouvert>400) spr->TempoCouvert=0;

        // Visée / Joueur
        if (DistanceIA<=FIX32(56))  spr->Animation=0;
        if (DistanceIA>FIX32(56) && spr->CoordX>SpriteREF->CoordX)  spr->Animation=4;
        if (DistanceIA>FIX32(56) && spr->CoordX<=SpriteREF->CoordX)  spr->Animation=6;
    }

    // Tir ? On locke
    if (spr->TirBusy) GestionBallesIA(spr);

    // Game Over ?!
    if (!SpriteREF->NombreUP && !SpriteREF->HitPoint )
    {
        GameOver=1;
        SpriteREF->HitPoint=0;
        GestionHUDSante();
        SpriteREF->NombreUP=0;
        SpriteREF->Direction=0;
        SpriteREF->Sprint=0;
        SpriteREF->Saut=1;
        SpriteREF->Acceleration=FIX32(6);
        SpriteREF->RefY=FIX32(240);
        SpriteREF->SensY=1;
        return;
    }

    // Attaque Corps à corps Soldat 3.
    if (spr->ID==3)
    {
        if (spr->CoupCrosse)
        {
            fix32 COL_X=abs(SpriteREF->CoordX-spr->CoordX);
            fix32 OF_X;
            if (spr->CoupCrosse==50) OF_X=FIX32(0);
            else OF_X=FIX32(32);
            spr->CoupCrosse=0;
            if (COL_X<=FIX32(16)+OF_X)
            {
                SND_startPlayPCM_XGM(SFX_GENERIC6, 1, SOUND_PCM_CH2);
                if (SpriteREF->HitPoint) SpriteREF->HitPoint--;
                SpriteREF->ClignoSprite=1;
                SpriteREF->ClignoDuree=20;
                SpriteREF->ClignoReach=0;
                SpriteREF->ClignoTic=2;
                if (!SpriteREF->HitPoint)
                {
                    if (!SpriteREF->NombreUP) return;
                    SpriteREF->HitPoint=SpriteREF->HitPointMax;
                    if (SpriteREF->NombreUP) SpriteREF->NombreUP--;
                    if (!SpriteREF->NombreUP) SpriteREF->NombreUP=0;
                }
                GestionHUDSante();
            }
            return;
        }
        if (DistanceIA<=FIX32(32))
        {
            if (SpriteREF->CoordX<=spr->CoordX) spr->Direction=50;
            else spr->Direction=51;
            spr->Reach=1;
            return;
        }
    }


    // Suffisamment proche, l'IA stoppe !  LEs grenadiers filent tout droit !
    if (spr->ID==4 ||spr->ID==5)   return;


    if (DistanceIA<=spr->DistanceAggro-FIX32(32))
    {
        if (SpriteREF->CoordX<=spr->CoordX) spr->Direction=0;
        else spr->Direction=98;
        spr->Reach=1;
        return;
    }

    if (DistanceIA>spr->DistanceAggro)
    {
        // Reach ? Tempo !InitIntro
        if (spr->Reach)
        {
            spr->TempoAggro++;
            if (spr->TempoAggro>25)
            {
                spr->TempoAggro=0;
                spr->Reach=0;
            }
        }
        if (!spr->Reach)
        {
            if (SpriteREF->CoordX<=spr->CoordX) spr->Direction=4;
            else spr->Direction=6;
        }
    }

}

//////////////////////////
//  Random SEED
//////////////////////////
void RandomSeed()
{
    RNDSEED++;
    if (RNDSEED>65000) RNDSEED=0;
    setRandomSeed(RNDSEED);
}


///////////////////////////////////////////
//           Fonction RANDOM
///////////////////////////////////////////
// range = interval (should be power of 2)
// returned value is between [0,range-1]
u16 getRandomU16(u16 range)
{
    return random() & (range - 1);
}


fix32 getRandomF32(fix32 range)
{
    return ((random() << 16) | random()) & (range - 1);
}


///////////////////////////////
//                  Scrolling
///////////////////////////////
void updateVDPScroll()
{
    VDP_setHorizontalScroll(BG_A,(-CamPosX)>>1);
    VDP_setVerticalScroll(BG_A, CamPosY>>1);

    // Tremblement écran !!
    if (TremblementON)
    {
        TempoTremblement++;
        // Fin tremblement
        if (TempoTremblement>=TempoMax)
        {
            TempoTremblement=0;
            TremblementON=0;
            VDP_setVerticalScroll(BG_B, CamPosY);
            return;
        }
        if (TempoTremblement & 1)  VDP_setVerticalScroll(BG_B,CamPosY -3);
        else VDP_setVerticalScroll(BG_B,CamPosY+3);
    }
}


///////////////////////////////////////////////////
//                  Gestion Camera / Scrolling
///////////////////////////////////////////////////
void setCameraPosition(s16 x, s16 y)
{
    if ((x != CamPosX) || (y != CamPosY))
    {
        CamPosX = x;
        CamPosY = y;

        // scroll maps
        MAP_scrollTo(bgb, x, y);
    }
}

///////////////////////////////////////////////////
//                  Gestion Caméra position
///////////////////////////////////////////////////
void updateCameraPosition(Sprite1_ *spr)
{
    if (GameOver) return;
    //if (PhaseScene) return;

    // get player position (pixel)
    fix32 posX=spr->CoordX;
    fix32 posY=spr->CoordY;
    s16 px = fix32ToInt(posX);
    s16 py = fix32ToInt(posY);

    // current sprite position on screen
    s16 px_scr = px - CamPosX;
    s16 py_scr = py - CamPosY;

    s16 npx_cam, npy_cam;

    // adjust new camera position
    if (px_scr > 140) npx_cam = px - 140;
    else if (px_scr <= 140) npx_cam = px - 140;
    else npx_cam = CamPosX;
    if (py_scr > 90) npy_cam = py - 90;
    else if (py_scr < 60) npy_cam = py - 60;
    else npy_cam = CamPosY;

    VisCamX=0;
    // clip camera position
    if (npx_cam < 0)
    {
        npx_cam = 0;
        VisCamX=1;
    }
    else if (npx_cam > (MAP_WIDTH - 320))
    {
        npx_cam = (MAP_WIDTH - 320);
        VisCamX=2;
    }
    if (npy_cam < 0) npy_cam = 0;
    else if (npy_cam > (MAP_HEIGHT - 224)) npy_cam = (MAP_HEIGHT - 224);

    PCamX=npx_cam;
    PCamY=npy_cam;
}



///////////////////////////////////////
//           Fonction Visibilité Zone Sprite
///////////////////////////////////////
void UpdateViewSprite(Sprite1_ *spr)
{
    // Sécurité
    if (spr->ID==99 || spr->IntIA==10 || spr->SpriteA==NULL) return;

    // ID Ref joueur
    Sprite1_* SpriteREF;
    SpriteREF=&Sprites[0];

    // Algoririthme de traitement
    fix32 DX=abs((SpriteREF->CoordX -FIX32(24))- spr->CoordX);
    u8 DET=0;

    fix32 CurrentFrame=FIX32(224);
    if (VisCamX) CurrentFrame=FIX32(308);
    if (spr->CoordX>FIX32(2048)) DET=1;
    if (spr->CoordX<=FIX32(-48))  DET=1;
    if (spr->CoordY>=FIX32(350)) DET=1;
    if (spr->CoordY<=FIX32(0))
    {
        if (spr->ID==45) DET=1;
        //if (spr->MortIA) DET=1;
    }

    // ID Joueur ? On quitte !
    if (!spr->SpriteDYN)
    {

        // Sprites en dehors des limites ?!
        if (DX>=CurrentFrame || DET)
        {
            if (spr->ID==66  || spr->ID==45 || spr->ID==46)
            {
                spr->StandBy=1;
                spr->Visible=0;
                return;
            }

            // Les bonus hors limites seront éliminés.
            if (spr->ID==40)
            {
                spr->StandBy=1;
                GoCivil=1;
                spr->Visible=0;
                return;
            }

            // Destruction des hélicos et attribution de nouveaux.
            if (spr->AirUnit && spr->MortIA)
            {
                //spr->Visible=0;
                //SPR_setVisibility(spr->SpriteA,HIDDEN);
                spr->MortIA=0;
                spr->TempoCouvert=0;
                spr->TempoSprite=0;
                spr->CoordX=FIX32(-64);
                spr->CoordY=FIX32(8);
                spr->MemDir=0;
                spr->Vitesse=FIX32(0.5);
                spr->Phase=0;
                spr->Direction=6;
                spr->TempoAggro=0;
                SPR_releaseSprite(spr->SpriteA);
                // Choix de l'hélico
                //RandomSeed();

                // Hélico léger
                u8 Choix=getRandomU16(100);
                if (NumeroZone)
                {
                    if (Choix<50) Choix=1;
                    else Choix=0;
                    if (VisCamX==2) Choix=0;
                }
                else Choix=1;
                if (Choix)
                {
                    spr->AirUnit=10;
                    spr->SpeIA=0;
                    if (Difficulte<4) spr->SpriteA = SPR_addSprite(&AirplaneLight_Sprite, 0, 0, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
                    else {spr->SpeIA=1;spr->SpriteA = SPR_addSprite(&AirplaneMedium_Sprite, 0, 0, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));}
                }
                else
                // Hélico lourd
                {
                    spr->AirUnit=1;
                    spr->SpeIA=0;
                    spr->SpriteA = SPR_addSprite(&Airplane_Sprite, 0, 0, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
                }
                return;
            }
        }

        // Trajectoire Hélico
        if (spr->AirUnit)
        {
            CurrentFrame=FIX32(70);
            if (spr->CoordX<FIX32(-96)) {spr->CoordX=FIX32(-96);Bombardement=0;spr->TempoAggro=0;}
            fix32 DX=abs((SpriteREF->CoordX -FIX32(24))- spr->CoordX);

            // A l'extrême droite, on évite la triche ! ^^
            if (spr->CoordX>FIX32(2048-128) && spr->Phase)
            {
                DX=FIX32(220);
                spr->Phase=2;
            }

            // Init phase
            if (!spr->Phase)
            {
                spr->Direction=6;
                if (!NumeroZone)
                {
                    spr->Vitesse=FIX32(0.5);
                    fix32 DX_L=FIX32(200);
                    if (VisCamX) DX_L=FIX32(300);
                    if (DX>DX_L)
                    {
                        spr->Visible=0;
                        if (spr->CoordX<=SpriteREF->CoordX)
                        {
                            spr->Vitesse=SpriteREF->Vitesse+FIX32(1.5);
                        }
                        else
                        {
                            spr->CoordX=SpriteREF->CoordX-FIX32(200);
                        }
                    }
                    else spr->Visible=1;
                }
                else
                {
                    spr->Visible=1;
                    if (!VisCamX) spr->CoordX=SpriteREF->CoordX-FIX32(220);
                    if (VisCamX==2) spr->CoordX=FIX32(1500);
                    if (VisCamX==1) spr->CoordX=FIX32(-96);
                }


                return;
            }
        }
        return;
    }

    // Sécurité
    /*
    if (spr->SpriteA==NULL && !spr->StandBy && spr->SpriteDYN)
    {
        SYS_reset();
        NombreIAScene--;
        if (NombreIAScene>32760) NombreIAScene=0;
        spr->SpriteDYN=1;
        spr->IntIA=0;
        spr->Direction=0;
        spr->StandBy=1;
        spr->Visible=0;
        if (spr->ID==6)
        {
            CivilON=0;
            NombreCivil=0;
        }
        spr->ID=0;
        return;
    }
    */

    // Civil toujours visible par défaut
    if (spr->ID==6 && spr->MortIA!=21)
    {
        if (CivilVisible) spr->Visible=1;
        else spr->Visible=0;
        if (!spr->MortIA) spr->Direction=0;
        return;
    }

    // Sprites en dehors des limites ?!
    if (DX>=CurrentFrame || DET)
    {
        SPR_setVisibility(spr->SpriteA,HIDDEN);
        SPR_releaseSprite(spr->SpriteA);
        NombreIAScene--;
        if (NombreIAScene>32760) NombreIAScene=0;
        spr->SpriteDYN=1;
        spr->SpriteA=NULL;
        spr->IntIA=0;
        spr->StandBy=1;
        spr->Direction=0;
        spr->Visible=0;
        if (spr->ID==4) GrenadierON=0;
        if (spr->ID==5)
        {
            if (NombrePara) NombrePara--;
        }
        if (spr->ID==6)
        {
            CivilON=0;
            NombreCivil=0;
        }
        spr->ID=0;
        return;
    }
}


///////////////////////////////
//            Liste Sprite DYN
///////////////////////////////
void CreateSpriteDYN(Sprite1_ *spr, u8 Type)
{
        //RandomSeed();
        spr->MemDir=0;
        spr->AirUnit=0;
        spr->StandBy=0;
        spr->SpriteDYN=1;
        spr->TypeIA=1;
        spr->CoordY=FIX32(140+24);
        CollisionGroundMAP(spr);
        spr->Acceleration=FIX32(0);
        spr->SensY=FIX32(0);
        spr->Direction=0;
        spr->Saut=0;
        //spr->InScene=1;
        spr->TempoSprite=0;
        spr->TirBusy=0;

        switch (Difficulte)
        {
            case 1:
            spr->Boost=FIX32(0.15);
            break;

            case 2:
            spr->Boost=FIX32(0.2);
            spr->HitPointMax++;
            break;

            case 3:
            spr->Boost=FIX32(0.25);
            spr->HitPointMax++;
            break;

            case 4:
            spr->Boost=FIX32(0.3);
            spr->HitPointMax+=2;
            break;

            case 5:
            spr->Boost=FIX32(0.35);
            spr->HitPointMax+=2;
            break;

            case 6:
            spr->Boost=FIX32(0.4);
            spr->HitPointMax+=3;
            break;

            case 7:
            spr->Boost=FIX32(0.5);
            spr->HitPointMax+=3;
            break;

            case 8:
            spr->Boost=FIX32(0.55);
            spr->HitPointMax+=4;
            break;

            case 9:
            spr->Boost=FIX32(0.6);
             spr->HitPointMax+=4;
            break;

            case 10:
            spr->Boost=FIX32(0.65);
            spr->HitPointMax+=5;
            break;
        }



		// Init Coordonnées
		Sprite1_* SpriteRef;
		SpriteRef=&Sprites[0];

        // Spé unités.
        switch (Type)
        {
            case 2:
			spr->Vitesse=FIX32(0.8)+getRandomF32(FIX32(0.5))+spr->Boost;
			//RandomSeed();
			spr->CaC=0;
			spr->Sniper=1;
			//RandomSeed();
			spr->DistanceAggro=getRandomF32(FIX32(48))+FIX32(48)+FIX32(40);
			spr->Spotting=FIX32(0);
            spr->VitesseInit=FIX32(0);
            break;

            case 3:
			spr->CaC=1;
			spr->Sniper=0;
            spr->DistanceAggro=FIX32(0);
            spr->Vitesse=FIX32(1)+getRandomF32(FIX32(0.5))+spr->Boost;
            spr->Spotting=FIX32(0);
            spr->VitesseInit=FIX32(0);
            break;

            case 4:
			spr->CaC=0;
			spr->Sniper=0;
            spr->DistanceAggro=FIX32(0);
            spr->Spotting=FIX32(0);
            spr->Vitesse=FIX32(1)+getRandomF32(FIX32(0.5));
            spr->VitesseInit=FIX32(0);
            break;

            case 5:
			spr->CaC=0;
			spr->Sniper=0;
			spr->Spotting=FIX32(40);
            spr->DistanceAggro=FIX32(0);
            spr->Vitesse=FIX32(0.20)+getRandomF32(FIX32(0.15));
            spr->CoordY=FIX32(-64);
            spr->InScene=0;
            //RandomSeed();
            spr->VitesseInit=getRandomF32(FIX32(0.25))+FIX32(0.1);
            if (getRandomU16(100)<50) spr->CoordX=SpriteRef->CoordX+getRandomF32(FIX32(150));
            else  spr->CoordX=SpriteRef->CoordX-getRandomF32(FIX32(150));
            break;

            case 6:
			spr->Vitesse=FIX32(0);
			spr->CaC=0;
			spr->Sniper=0;
			spr->Direction=0;
			spr->Vitesse=FIX32(1);
			spr->DistanceAggro=FIX32(0);
			spr->Spotting=FIX32(0);
            spr->VitesseInit=FIX32(0);
            spr->Direction=0;
            spr->InScene=0;
            spr->CoordX=Civil_CoordX;
            spr->Visible=0;
            return;
            break;
        }

        if (VisCamX==2)
        {
            if (SpriteRef->CoordX>FIX32(2048-160))
            {
                if (Type==5)
                {
                    spr->CoordX=SpriteRef->CoordX-FIX32(320);
                    return;
                }
                //RandomSeed();
                if (getRandomU16(100)<50)
                {
                    spr->CoordX=SpriteRef->CoordX+FIX32(180);
                    if (Type==4) spr->Direction=4;
                }
                else
                {
                    spr->CoordX=SpriteRef->CoordX-FIX32(320);
                    if (Type==4) spr->Direction=6;
                }
                return;
            }
        }

        if (VisCamX==1)
        {
            //RandomSeed();
            if (getRandomU16(100)<50)
            {
                spr->CoordX=FIX32(0);
                if (Type==4) spr->Direction=6;
            }
            else
            {
                spr->CoordX=FIX32(320);
                if (Type==4) spr->Direction=6;
            }
            return;
        }

        if (getRandomU16(100)<50)
        {
            spr->CoordX=SpriteRef->CoordX+FIX32(180);
            if (Type==4) spr->Direction=4;
        }
        else
        {
            spr->CoordX=SpriteRef->CoordX-FIX32(180);
            if (Type==4) spr->Direction=6;
        }

}


////////////////////////////////////////
//                  Display Sprites
////////////////////////////////////////
void DisplaySprite(Sprite1_ *spr)
{

    // Sécurité
    if (spr->SpriteA == NULL )  return;

    //Phase scene en cours ?!
    if (PhaseScene)
    {
        switch(PhaseScene)
        {
        case 1:
            if (spr->ID!=10 && spr->ID!=11) spr->Visible=0;
            else spr->Visible=1;
            break;

        case 2:
            if (spr->ID==99) spr->Visible=1;
            break;
        }
    }

    // on positionne le sprite relativement à la camera
    s16 x = fix32ToInt(spr->CoordX-spr->OffsetX) - CamPosX;
    s16 y = fix32ToInt(spr->CoordY+spr->DeltaY-spr->OffsetY) - CamPosY;

    if (spr->Visible==1) SPR_setVisibility(spr->SpriteA,VISIBLE);
    else SPR_setVisibility(spr->SpriteA,HIDDEN);

    // Traitement priorité hélico
    if (spr->AirUnit)
    {
        if (!spr->Phase) {SPR_setAlwaysOnTop(spr->SpriteA,FALSE);SPR_setPriorityAttribut(spr->SpriteA, FALSE);}
        else {SPR_setAlwaysOnTop(spr->SpriteA,TRUE);SPR_setPriorityAttribut(spr->SpriteA, TRUE);}
    }

    // Affichage
    SPR_setPosition(spr->SpriteA, x, y);
}

////////////////////////////////////////
//                  Gestion Sprites
////////////////////////////////////////
void UpdateSprite(Sprite1_ *spr)
{

    if (PhaseScene && spr->ID!=10 && spr->ID!=11) return;
    if (spr->StandBy) return;

    //if (spr->ID==66 || spr->ID==88) return;
    if (spr->SpriteA==NULL) return;

    // Bouclier qui vole ?
    if (spr->TypeBouclier && spr->Visible)
    {
        spr->CoordY-=spr->Acceleration;
        spr->Acceleration-= FIX32(0.2);
        spr->Direction=0;
        if (spr->CoordY>FIX32(240))
        {
            if (spr->SpriteA!=NULL)
            {
                SPR_releaseSprite(spr->SpriteA);
                spr->SpriteA=NULL;
                spr->Visible=0;
                spr->StandBy=1;
                return;
            }
        }
    }

    // Mort par bombe ?!
    if (spr->IntIA==10)
    {
        MoveSprite(spr);
        Saut_Sprite(spr);
        return;
    }

    // Ia Morte ? On vire !
    if (spr->MortIA && spr->SpriteDYN) return;

    // Déplacement ( Couteau ?! OFF ! )
    if (!spr->Couteau) MoveSprite(spr);

    if (spr->ID==45 || spr->ID==46 || spr->ID==10  || spr->ID==11) return;

    // Trajectoire Airplane
    if (spr->AirUnit && spr->Visible)
    {
        //return;    // On désactive l'hélico.
        Sprite1_* SpriteREF;
        SpriteREF = &Sprites[0];
        fix32 DistAir=abs(SpriteREF->CoordX-spr->CoordX);
        // Phase d'hélicoptère.
        if (!spr->Phase)
        {
            u16 T2=500;
            if (VisCamX==2) T2=50;
            if (spr->Visible) spr->TempoCouvert++;
            if (spr->TempoCouvert>(T2-(Difficulte<<3)) && NombreIAScene<3 && spr->Visible && !VisCamX && !CivilON)
            {
                spr->Phase=1;
                spr->TempoCouvert=0;
                SPR_setPriorityAttribut(spr->SpriteA, TRUE);
                 XGM_startPlay(Thunder_Music);
            }
        }
        if (spr->Phase==1)
        {
            spr->TempoCouvert++;
            if (spr->TempoCouvert>60)
            {
                spr->TempoCouvert=0;
                spr->Phase=2;
                spr->TempoAggro=0;
                Bombardement=0;
                if (spr->AirUnit==10)
                {
                    spr->Vitesse=FIX32(2.4);
                    if (!spr->SpeIA) spr->HitPoint=20+(Difficulte<<1);
                    else spr->HitPoint=25+(Difficulte<<1);
                }
                else
                {
                    spr->Vitesse=FIX32(1.85);
                    spr->HitPoint=30+(Difficulte<<1);
                }
            }
            return;
        }
        if (!spr->MortIA)
        {
            // Manoeuvre d'attaque
            if (spr->Phase==2)
            {
                fix32 DX=abs((SpriteREF->CoordX -FIX32(24))- spr->CoordX);
                fix32 Course=FIX32(90);

                fix32 ZX1=FIX32(1.8);
                fix32 VH=FIX32(1.85);
                fix32 DH=FIX32(0.05);
                if (spr->AirUnit==10)
                {
                    ZX1=FIX32(1.4);
                    VH=FIX32(2.4);
                    DH=FIX32(0.08);
                }
                // Ralentissement droite
                if (spr->Transition==1)
                {
                    spr->CalculTransition=1;
                    spr->Direction=6;
                    spr->Vitesse-=DH;
                    if (spr->Vitesse<=FIX32(0.15))
                    {
                        spr->Transition=2;
                    }
                }
                if (spr->Transition==2)
                {
                    spr->Direction=4;
                    if (spr->Vitesse>ZX1) spr->CalculTransition=0;
                    spr->Vitesse+=DH;
                    if (spr->Vitesse>VH)
                    {
                        spr->Vitesse=VH;
                        spr->Transition=3;
                    }
                }
                if (spr->Transition==4)
                {
                    spr->CalculTransition=2;
                    spr->Direction=4;
                    spr->Vitesse-=DH;
                    if (spr->Vitesse<=FIX32(0.15))
                    {
                        spr->Transition=5;
                    }
                }

                if (spr->Transition==5)
                {
                    spr->Direction=6;
                    spr->Vitesse+=DH;
                    if (spr->Vitesse>ZX1) spr->CalculTransition=0;
                    if (spr->Vitesse>VH)
                    {
                        spr->Vitesse=VH;
                        spr->Transition=0;
                    }
                }

                if (DX>Course && spr->CoordX>SpriteREF->CoordX && !spr->Transition) spr->Transition=1;

                if (DX>Course && spr->CoordX<=SpriteREF->CoordX && spr->Transition==3)
                {
                    spr->Transition=4;
                }
            }
            if (spr->Phase)
            {
                if (spr->SensY) spr->CoordY+=FIX32(0.2);
                else  spr->CoordY-=FIX32(0.2);
                if (spr->CoordY>=FIX32(60) && spr->SensY==1) spr->SensY=0;
                if (spr->CoordY<FIX32(15) && !spr->SensY) spr->SensY=1;
            }
            // Calcul Distance / Joueur
            if (DistAir>FIX32(256))
            {
                if (spr->Direction==6 && (spr->CoordX>=SpriteREF->CoordX)) spr->Direction=4;
                if (spr->Direction==4 && (spr->CoordX<SpriteREF->CoordX)) spr->Direction=6;
            }
            if (spr->CoordX>FIX32(2200) && spr->Direction==6) spr->Direction=4;
            if (spr->CoordX<FIX32(-100) && spr->Direction==4) spr->Direction=6;
        }
        else
        {
            if (spr->Direction==6)
            {
                spr->CoordX+=FIX32(1);
                spr->CoordY+=FIX32(1.3);
            }
            else
            {
                spr->CoordX-=FIX32(1);
                spr->CoordY+=FIX32(1.3);
            }
        }
        return;
    }

    // Saut
    if (spr->Saut) Saut_Sprite(spr);


    // Limite et collision sol
    // Limite sprites
    if (!spr->IAFuite)
    {
        if (spr->CoordX<=FIX32(0)) spr->CoordX=FIX32(0);
        if (spr->CoordX>FIX32(2016)) spr->CoordX=FIX32(2016);
    }

   CollisionGroundMAP(spr);

}


//////////////////////////////////////////
//       Routine Collision / Sol MAP
/////////////////////////////////////////
void CollisionGroundMAP(Sprite1_ *spr)
{
    if (spr->Saut || spr->ID==88 || NumeroZone) return;

    // N'appliquons pas cela aux paras et hélicos et balles.
    if (spr->ID==5 || spr->AirUnit || spr->IDList) return;

    // Collision Sol MAP.
    if (spr->CoordX<FIX32(276)) {spr->DeltaY=FIX32(0);return;}
    if (spr->CoordX>=FIX32(276) && spr->CoordX<FIX32(792)) {spr->DeltaY=FIX32(16);return;}
    if (spr->CoordX>FIX32(792) && spr->CoordX<FIX32(1400)) {spr->DeltaY=FIX32(0);return;}
    if (spr->CoordX>=FIX32(1400) && spr->CoordX<FIX32(1700)) {spr->DeltaY=FIX32(-16);return;}
    if (spr->CoordX>=FIX32(1700)) spr->DeltaY=FIX32(0);
}

//////////////////////////////////////////
//            Routine déplacement
/////////////////////////////////////////
void MoveSprite(Sprite1_ *spr)
{
    switch(spr->Direction)
    {
        case 4:
            spr->CoordX-=spr->Vitesse-spr->VitesseD;
            break;

        case 6:
           spr->CoordX+=spr->Vitesse-spr->VitesseD;
            break;

        // uniquement pour les balles
        case 8:
            spr->CoordY-=spr->Vitesse;
            break;

        case 14:
            spr->CoordX-=spr->Vitesse;
            spr->CoordY+=spr->Vitesse;
            break;

        case 16:
            spr->CoordX+=spr->Vitesse;
            spr->CoordY+=spr->Vitesse;
            break;

        case 24:
            // Si tir en accroupi, alors on avance pas !  Pour Marco
            if (!spr->Feu) spr->CoordX-=spr->Vitesse-FIX32(0.8);
            break;

        case 26:
            if (!spr->Feu) spr->CoordX+=spr->Vitesse-FIX32(0.8);
            break;


        case 36:
           spr->CoordX+=spr->Vitesse;
            spr->CoordY+=spr->Vitesse;
            break;

        case 42:
            spr->CoordY+=spr->Vitesse;
            spr->CoordX+=spr->VitesseInit;
            break;

        case 84:
            spr->CoordX-=spr->Vitesse-spr->VitesseD;
            break;

        case 86:
           spr->CoordX+=spr->Vitesse-spr->VitesseD;
            break;
    }
}

//////////////////////////////////////////
//                  Routine Saut Sprite
/////////////////////////////////////////
void Saut_Sprite(Sprite1_ *spr)
{
    fix32 S=FIX32(0.25);
    if (GameOver) S=FIX32(0.2);
    spr->CoordY-=spr->Acceleration;
    spr->Acceleration-= S;
    if (spr->CoordY>spr->RefY)
    {
        spr->CoordY=spr->RefY;
        // Cas du rebond de la grenade !
        if (spr->ID==88)
        {
            // 1er rebond ?!
            if (spr->Saut==1)
            {
                spr->Vitesse=FIX32(1);
                spr->OffsetY=FIX32(0);
                //spr->Direction=6;
                spr->Saut=2;
                spr->Acceleration=FIX32(4);
                spr->RefY=FIX32(140+50);
                spr->SensY=1;
                return;
            }
            else
            {
                spr->Direction=0;
                spr->Hit=1;
                spr->TempoSprite=0;
                TremblementON=1;
                TempoMax=8;
                TempoTremblement=0;
            }
        }

        spr->Saut=0;

        if (spr->IntIA==10)
        {
            if (spr->SpriteA!=NULL)
            {
                SPR_releaseSprite(spr->SpriteA);
                NombreIAScene--;
                if (NombreIAScene>32760) NombreIAScene=0;
                spr->SpriteA=NULL;
                spr->StandBy=1;
                spr->Visible=0;
                spr->IntIA=0;
                if (spr->ID==4) GrenadierON=0;
                if (spr->ID==6) {CivilON=0;CivilVisible=0;}
            }
        }
        if (GameOver)
        {
            VDP_fadeOutAll(32,FALSE);
            GameOver=2;
        }
    }
}

//////////////////////////////////////////
//                  Routine PRINT DATA
/////////////////////////////////////////
void PrintFix32(fix32 D,u8 X,u8 Y)
{
    sprintf(Texte,"D :%ld",fix32ToInt(D));
    VDP_drawText(Texte,X,Y);
}
void PrintU8(u8 D,u8 X,u8 Y)
{
    sprintf(Texte,"D :%d",D);
    VDP_drawText(Texte,X,Y);
}

void PrintU16(u16 D,u8 X,u8 Y)
{
    sprintf(Texte,"D :%d",D);
    VDP_drawText(Texte,X,Y);
}
