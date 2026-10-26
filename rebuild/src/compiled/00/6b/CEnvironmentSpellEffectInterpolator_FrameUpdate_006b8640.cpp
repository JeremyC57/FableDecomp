// CEnvironmentSpellEffectInterpolator::FrameUpdate() -- update priority stacks and publish the four current screen effects.
// Retail 006b8640,233 bytes. Calls CScreenEffectPriorityStack::FrameUpdate 00880e50 and interface setters 00884e90/00884f60/00884f90/00884fe0.
// No generated headers for these classes. Native stack uses smaller STL containers than donor: current radial +1c, position +4c, displacement +84, filter +ac.
class CRGBFloatColour { public: float R,G,B,A; };
struct CDisplacementParams { CRGBFloatColour MonochromeEffectColour; };
class C3DVector { public: float X,Y,Z; C3DVector() {} C3DVector(const C3DVector& x):X(x.X),Y(x.Y),Z(x.Z) {} };
struct CRadialBlurParams { CRGBFloatColour ThresholdColour,IntensityColour; float Length,Fade,Range,Twist; };
struct CRadialBlurPositionParams { C3DVector AreaCenter,BlurCenter; long AreaTrackPos,BlurTrackPos; };
struct CScreenFilterParams { float Alpha,Saturation,Contrast,Brightness; CRGBFloatColour Tint; };
class CEnvironmentSpellEffectInterface { public:
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
class CEnvironmentSpellEffectInterpolator { public:
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