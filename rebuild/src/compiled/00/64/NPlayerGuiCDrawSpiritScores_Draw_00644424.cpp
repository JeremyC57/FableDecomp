// NPlayerGui::CDrawSpiritScores::Draw -- draw each available player's score with a black shadow.
// Native00644424,258bytes. Actual PlayerGui/PlayerManager/Player/FontBank/CWideString headers.
// Calls4498C0 GetPlayer(long),44A2A0 GetSpiritScoreText(long,CWideString&)const
// (PDB donor445C20, wrong native propagated DrawGetEnvironmentInfo name),
// 449B60 GetMultiplayerColour(long)const,438010 Draw2DText (thunk to64B6DA).
// Draw2DText signature PDB44AAD0: const vector/string/font/colour&,float,EEngine2DLayer,bool.
// Main component header unavailable; its native font pointer1614C is used without claiming
// a proved full-layout correspondence (donor PCutSceneMainFont occupies same offset).
// Globals13961E8/13961F0 are double coordinate scales; literals retain widened float precision.
// No actual 2D vector/colour/PrepareInfo header. Colour's real hidden-return ABI is proven
// 449B60(resultbuffer,index),ret8,EAX=resultbuffer; nontrivial constructor preserves it.
// Leaf: Draw
#pragma optimize("y",off)
#pragma optimize("s",on)
#include "engine/CPlayerGui.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CFontBank.h"
#include "fable_string.h"
struct CEnginePrimitivePrepareInfo;
struct C2DVector {float X,Y;C2DVector(){}C2DVector(float x,float y):X(x),Y(y){} C2DVector operator+(const C2DVector&v)const{return C2DVector(X+v.X,Y+v.Y);}};
struct CRGBColour {unsigned char Blue,Green,Red,Alpha;CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a=255){Red=r;Green=g;Blue=b;Alpha=a;}};
enum EEngine2DLayer {NativeLayer67=67};
struct PlayerManagerMethods:CPlayerManager {CPlayer&GetPlayer(long);bool GetSpiritScoreText(long,CWideString&)const;CRGBColour GetMultiplayerColour(long)const;};
struct PlayerGuiMethods:CPlayerGui {void Draw2DText(const C2DVector&,const CWideString&,const CFontBank&,const CRGBColour&,float,EEngine2DLayer,bool);};
struct MainComponentFontView {unsigned char UnrecoveredPrefix[0x1614c];CFontBank* Font;const CFontBank&GetFont()const{return *Font;}};
extern double g_CoordinateWidth,g_CoordinateHeight;
struct DrawSpiritScoresNative {void*VTable;PlayerGuiMethods*PlayerGui;PlayerManagerMethods*PlayerManager;void*World;MainComponentFontView*Component;void*Bank;bool Drawing;void Draw(const CEnginePrimitivePrepareInfo&);};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CRGBColour)==4);
FABLE_STATIC_ASSERT(offsetof(MainComponentFontView,Font)==0x1614c);
FABLE_STATIC_ASSERT(offsetof(DrawSpiritScoresNative,Drawing)==0x18);
#endif
void DrawSpiritScoresNative::Draw(const CEnginePrimitivePrepareInfo&){
 Drawing=true;C2DVector position(float(g_CoordinateWidth*double(.4f)),float(g_CoordinateHeight*double(.83f)));
 for(long i=0;i<4;++i){CWideString text;PlayerManager->GetPlayer(i);
  if(PlayerManager->GetSpiritScoreText(i,text)){
   CRGBColour colour=PlayerManager->GetMultiplayerColour(i);
   PlayerGui->Draw2DText(position+C2DVector(250.0f,250.0f),text,Component->GetFont(),CRGBColour(0,0,0),1.0f,NativeLayer67,true);
   PlayerGui->Draw2DText(position,text,Component->GetFont(),colour,1.0f,NativeLayer67,true);
   position.X+=float(g_CoordinateWidth*double(.12f));
  }
 }
}