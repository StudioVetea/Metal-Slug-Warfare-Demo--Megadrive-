#ifndef _ROUTINES_H
#define _ROUTINES_H


extern void GestionCollisionGrenades(Sprite1_ *SprIA, Sprite1_ *spr);
extern void GestionCollisionBombes(Sprite1_ *SprIA, Sprite1_ *spr, Sprite1_ *SpriteREF);
extern void GestionCollisionBalles(Sprite1_ *SprIA, Sprite1_ *spr, Sprite1_ *SpriteREF);
extern  void PaletteEffectZone2();
extern void GestionBombe(Sprite1_ *spr);
extern void GestionProjectiles();
extern void GestionHelico();
extern void GestionCivil(Sprite1_ *spr);
extern void GestionNombreBallesShotgun();
extern void GestionNombreGrenades();
extern void GestionBallesIA(Sprite1_ *spr);
extern void GestionVague();
extern void MarcheJoe(Sprite1_ *spr);
extern void InitRoutine();
extern void InitScene();
extern void InitMAP();
extern void Result_Screen();
extern void updateVDPScroll();
extern void setCameraPosition(s16 x, s16 y);
extern void GestionAttaqueHelico(Sprite1_ *spr);
extern void updateCameraPosition(Sprite1_ *spr);
extern void GestionTir(Sprite1_ *spr);
extern void GestionGrenades(Sprite1_ *spr);
extern void UpdateViewSprite(Sprite1_ *spr);
extern void GestionCouteau(Sprite1_ *spr);
extern void GestionBalles(Sprite1_ *spr, Sprite1_ *SpriteREF);
extern void GestionIA(Sprite1_  *spr);
extern void TimerChanged(Sprite* sprite);
extern void TimerChangedIA(Sprite* sprite);
extern void TimerChangedIA1(Sprite* sprite);
extern void DisplaySprite(Sprite1_ *spr);
extern void CreateSpriteDYN(Sprite1_* spr, u8 Type);
extern void UpdateSprite(Sprite1_ *spr);
extern void UpdateScene();
extern void AnimationSprite(Sprite1_ *spr);
extern void PrintFix32(fix32 D,u8 X,u8 Y);
extern void PrintU8(u8 D,u8 X,u8 Y);
extern void PrintU16(u16 D,u8 X,u8 Y);
extern void RandomSeed();
extern void GestionScore();
extern void GestionUP(Sprite1_ *spr);
extern  u16 getRandomU16(u16 range);
extern fix32 getRandomF32(fix32 range);
extern void GestionHUDSante();
extern void Phase_Scene(u8 Scene,Sprite1_ *spr);
extern void Saut_Sprite(Sprite1_ *spr);
extern void Cligno_Sprite(Sprite1_ *spr);
extern void MoveSprite(Sprite1_ *spr);
extern void CollisionGroundMAP(Sprite1_ *spr);
extern void DisplayMedals();
extern void GestionBonus(Sprite1_ *spr);
extern void CreateIANew(Sprite1_ *spr);

#endif // _MAIN
