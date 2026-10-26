// NPlayerGui::CDrawMorality::Draw -- choose the signed stat-bar position/frame and draw it.
// Native006448A4,248bytes. Actual PlayerGui/PlayerGuiDef/GraphicDataBank headers retained.
// Native PlayerGui Alpha2A0/Morality2DC versus donor2B4/2F0 (-20 after smaller containers).
// Native definition fields used before StatUpdateColours differ donor-48;
// graphics vector54C versus donor580 (-52 after preceding vector shrinks16->12).
// No actual DrawMorality/StatBar/C2DVector/CRGBColour/PrepareInfo header exists.
// StatBar value10 and GraphicFrame50 native; PDB GraphicFrame80 corroborates.
// Its real first virtual slot DrawStatBarOnLeft at nativeVT125A738=646762;
// exact seven-argument const-reference signature PDB donor68982B. Only verified VT prefix declared.
// 438570 PeekGuiDef,122DEDC floatzero,1230014 float255,BFEA70 float-to-integer runtime.
// Raw PDB CRGBColour_u_0_s_0 proves B0,G1,R2,A3.
// Leaf: Draw
#pragma optimize("y",off)
#pragma optimize("s",on)
#include "engine/CPlayerGui.h"
#include "engine/_quarantine/CPlayerGuiDef.h"
#include "engine/_quarantine/CGraphicDataBank.h"
class CEnginePrimitivePrepareInfo;
class C2DVector { public:float X,Y;};
class CRGBColour { public:unsigned char Blue,Green,Red,Alpha;};
struct StatBarNative {
 virtual void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,const C2DVector&,const CRGBColour&,unsigned char,bool);
 float GetValue()const{return Value;}
 void SetGraphicFrame(unsigned long n){GraphicFrame=n;}
 unsigned char Unrecovered04[12];float Value;unsigned char Unrecovered14[60];unsigned long GraphicFrame;
};
struct PlayerGuiNative {unsigned char Unrecovered00[0x2a0];float Alpha;unsigned char Unrecovered2A4[0x38];StatBarNative* Morality;};
struct LongArray {long*Start;long*Finish;long*Limit;};
struct PlayerGuiDefNative {
 unsigned char Unrecovered00[0x4ac];C2DVector MoneyTextOffset,MoneyTextRightOffset;float MoneyWidth;CRGBColour MoneyTextColour;
 unsigned char Unrecovered4C4[0x38];C2DVector MoralityGoodPos,MoralityEvilPos;unsigned char Unrecovered50C[0x24];
 long MoralityGoodGraphic,MoralityEvilGraphic,MoralityGoodEffect,MoralityEvilEffect;
 unsigned char StatUpdateColours[12];LongArray StatUpdateGraphics;
};
PlayerGuiDefNative* __fastcall PeekGuiDef(); // genuine zero-argument fastcall00438570; native view of CPlayerGuiDef
const float g_FloatZero=0.0f;const float g_AlphaScale255=255.0f;
struct DrawMoralityNative {
 void*VTable;PlayerGuiNative*PlayerGui;void*PlayerManager;void*World;void*Component;CGraphicDataBank*Bank;bool Drawing;
 void Draw(const CEnginePrimitivePrepareInfo&);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(StatBarNative,Value)==0x10);
FABLE_STATIC_ASSERT(offsetof(StatBarNative,GraphicFrame)==0x50);
FABLE_STATIC_ASSERT(offsetof(PlayerGuiNative,Alpha)==0x2a0);
FABLE_STATIC_ASSERT(offsetof(PlayerGuiNative,Morality)==0x2dc);
FABLE_STATIC_ASSERT(offsetof(PlayerGuiDefNative,MoneyTextColour)==0x4c0);
FABLE_STATIC_ASSERT(offsetof(PlayerGuiDefNative,MoralityGoodPos)==0x4fc);
FABLE_STATIC_ASSERT(offsetof(PlayerGuiDefNative,MoralityGoodGraphic)==0x530);
FABLE_STATIC_ASSERT(offsetof(PlayerGuiDefNative,StatUpdateGraphics)==0x54c);
FABLE_STATIC_ASSERT(offsetof(DrawMoralityNative,Drawing)==0x18);
#endif
void DrawMoralityNative::Draw(const CEnginePrimitivePrepareInfo&info){
 Drawing=true;PlayerGuiDefNative*def=PeekGuiDef();StatBarNative*bar=PlayerGui->Morality;
 bar->SetGraphicFrame(def->StatUpdateGraphics.Start[5]);
 if(bar->GetValue()!=g_FloatZero){C2DVector pos,offset;bool positive;
  if(bar->GetValue()>g_FloatZero){pos=def->MoralityGoodPos;offset=def->MoneyTextRightOffset;bar->GraphicFrame=def->MoralityGoodGraphic;positive=true;}
  else{pos=def->MoralityEvilPos;offset=def->MoneyTextOffset;bar->GraphicFrame=def->MoralityEvilGraphic;positive=false;}
  bar->DrawStatBarOnLeft(info,*Bank,pos,offset,def->MoneyTextColour,(unsigned char)(g_AlphaScale255*PlayerGui->Alpha),positive);
 }
}