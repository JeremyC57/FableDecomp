// CEnvironmentSpellEffectInterpolator::FrameUpdate() -- update priority stacks and publish the four current screen effects.
// Retail 006b8640,233 bytes. Calls CScreenEffectPriorityStack::FrameUpdate 00880e50 and interface setters 00884e90/00884f60/00884f90/00884fe0.
// No generated headers for these classes. Native stack uses smaller STL containers than donor: current radial +1c, position +4c, displacement +84, filter +ac.
struct CRGBFloatColour { float R,G,B,A; };
struct CDisplacementParams { CRGBFloatColour MonochromeEffectColour; };
struct C3DVector { float X,Y,Z; C3DVector() {} C3DVector(const C3DVector& x):X(x.X),Y(x.Y),Z(x.Z) {} };
struct CRadialBlurParams { CRGBFloatColour ThresholdColour,IntensityColour; float Length,Fade,Range,Twist; };
struct CRadialBlurPositionParams { C3DVector AreaCenter,BlurCenter; long AreaTrackPos,BlurTrackPos; };
struct CScreenFilterParams { float Alpha,Saturation,Contrast,Brightness; CRGBFloatColour Tint; };
struct CEnvironmentSpellEffectInterface {
 void SetRadialBlurParams(const CRadialBlurParams&,bool); //00884e90
 void SetRadialBlurPositionParams(const CRadialBlurPositionParams&,bool); //00884f60
 void SetDisplacementParams(const CDisplacementParams&,bool); //00884f90
 void SetScreenFilterParams(const CScreenFilterParams&,bool); //00884fe0
};
struct CScreenEffectPriorityStack {
 unsigned char radialListsAndKeys[0x1c];
 CRadialBlurParams CurrentRadialBlurParams;
 CRadialBlurPositionParams CurrentRadialBlurPosParams;
 unsigned char displacementListsAndKeys[0x18];
 CRGBFloatColour CurrentDisplacementParam;
 unsigned char filterListsAndKeys[0x18];
 CScreenFilterParams CurrentScreenFilterParam;
 void FrameUpdate(); //00880e50
 CRadialBlurParams GetCurrentRadialBlurParams() const { return CurrentRadialBlurParams; }
 CRadialBlurPositionParams GetCurrentRadialBlurPositionParams() const { return CurrentRadialBlurPosParams; }
 CDisplacementParams GetCurrentDisplacementParams() const { CDisplacementParams p; p.MonochromeEffectColour=CurrentDisplacementParam; return p; }
 CScreenFilterParams GetCurrentScreenFilterParams() const { return CurrentScreenFilterParam; }
};
struct CEnvironmentSpellEffectInterpolator {
 void* Environment; CEnvironmentSpellEffectInterface* SpellInterface;
 unsigned char fadeAndTransitionState[0x88];
 CScreenEffectPriorityStack ScreenEffectPriorityStack;
 void FrameUpdate();
};
void CEnvironmentSpellEffectInterpolator::FrameUpdate() {
 ScreenEffectPriorityStack.FrameUpdate();
 SpellInterface->SetRadialBlurParams(ScreenEffectPriorityStack.GetCurrentRadialBlurParams(),true);
 SpellInterface->SetRadialBlurPositionParams(ScreenEffectPriorityStack.GetCurrentRadialBlurPositionParams(),true);
 SpellInterface->SetDisplacementParams(ScreenEffectPriorityStack.GetCurrentDisplacementParams(),true);
 SpellInterface->SetScreenFilterParams(ScreenEffectPriorityStack.GetCurrentScreenFilterParams(),true);
}

#include <stdio.h>
#include <stddef.h>
#include <string.h>
static int sequence,failed;
static CEnvironmentSpellEffectInterpolator* owner;
static CEnvironmentSpellEffectInterface *firstReceiver,*laterReceiver;
void CScreenEffectPriorityStack::FrameUpdate() {
 if(sequence++!=0 || this!=&owner->ScreenEffectPriorityStack) failed=1;
 float values[12];for(int i=0;i<12;++i) values[i]=float(i+1);
 memcpy(&CurrentRadialBlurParams,values,sizeof(CurrentRadialBlurParams));
 CurrentRadialBlurPosParams.AreaCenter.X=31; CurrentRadialBlurPosParams.AreaCenter.Y=32; CurrentRadialBlurPosParams.AreaCenter.Z=33;
 CurrentRadialBlurPosParams.BlurCenter.X=41; CurrentRadialBlurPosParams.BlurCenter.Y=42; CurrentRadialBlurPosParams.BlurCenter.Z=43;
 CurrentRadialBlurPosParams.AreaTrackPos=3; CurrentRadialBlurPosParams.BlurTrackPos=7;
 CRGBFloatColour& c=CurrentDisplacementParam;
 c.R=51;c.G=52;c.B=53;c.A=54;
 memcpy(&CurrentScreenFilterParam,values,sizeof(CurrentScreenFilterParam));
}
void CEnvironmentSpellEffectInterface::SetRadialBlurParams(const CRadialBlurParams& p,bool b) {
 CScreenEffectPriorityStack& x=owner->ScreenEffectPriorityStack;
 if(sequence++!=1 || this!=firstReceiver || !b || &p==&x.CurrentRadialBlurParams || memcmp(&p,&x.CurrentRadialBlurParams,sizeof(p))) failed=1;
 x.CurrentRadialBlurParams.Length=777;if(p.Length!=9) failed=1;
 x.CurrentRadialBlurPosParams.AreaCenter.X=81;
 owner->SpellInterface=laterReceiver;
}
void CEnvironmentSpellEffectInterface::SetRadialBlurPositionParams(const CRadialBlurPositionParams& p,bool b) {
 CScreenEffectPriorityStack& x=owner->ScreenEffectPriorityStack;
 if(sequence++!=2 || this!=laterReceiver || !b || &p==&x.CurrentRadialBlurPosParams || memcmp(&p,&x.CurrentRadialBlurPosParams,sizeof(p)) || p.AreaCenter.X!=81) failed=1;
 x.CurrentRadialBlurPosParams.AreaCenter.X=999;if(p.AreaCenter.X!=81) failed=1;
 x.CurrentDisplacementParam.R=91;
}
void CEnvironmentSpellEffectInterface::SetDisplacementParams(const CDisplacementParams& p,bool b) {
 CScreenEffectPriorityStack& x=owner->ScreenEffectPriorityStack;
 if(sequence++!=3 || this!=laterReceiver || !b || &p.MonochromeEffectColour==&x.CurrentDisplacementParam || memcmp(&p,&x.CurrentDisplacementParam,sizeof(p)) || p.MonochromeEffectColour.R!=91) failed=1;
 x.CurrentDisplacementParam.R=999;if(p.MonochromeEffectColour.R!=91) failed=1;
 x.CurrentScreenFilterParam.Alpha=101;
}
void CEnvironmentSpellEffectInterface::SetScreenFilterParams(const CScreenFilterParams& p,bool b) {
 CScreenEffectPriorityStack& x=owner->ScreenEffectPriorityStack;
 if(sequence++!=4 || this!=laterReceiver || !b || &p==&x.CurrentScreenFilterParam || memcmp(&p,&x.CurrentScreenFilterParam,sizeof(p)) || p.Alpha!=101) failed=1;
 x.CurrentScreenFilterParam.Alpha=999;if(p.Alpha!=101) failed=1;
}
int main() {
 if(sizeof(void*)==4 && (offsetof(CEnvironmentSpellEffectInterpolator,ScreenEffectPriorityStack)!=0x90 || offsetof(CScreenEffectPriorityStack,CurrentRadialBlurParams)!=0x1c || offsetof(CScreenEffectPriorityStack,CurrentRadialBlurPosParams)!=0x4c || offsetof(CScreenEffectPriorityStack,CurrentDisplacementParam)!=0x84 || offsetof(CScreenEffectPriorityStack,CurrentScreenFilterParam)!=0xac)) return 2;
 CEnvironmentSpellEffectInterpolator x; CEnvironmentSpellEffectInterface y,z;
 owner=&x;firstReceiver=&y;laterReceiver=&z;
 for(int i=0;i<2;++i) {x.SpellInterface=&y;sequence=0;x.FrameUpdate(); if(sequence!=5 || failed) return 1;}
 puts("006b8640_TEST PASS");return 0;
}