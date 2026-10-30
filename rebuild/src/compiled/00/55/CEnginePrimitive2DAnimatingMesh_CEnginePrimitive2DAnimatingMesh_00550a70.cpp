// CEnginePrimitive2DAnimatingMesh::CEnginePrimitive2DAnimatingMesh(unsigned __int64, unsigned long, const C3DAnimationBlendState&, const C2DBoxF&, const C3DBoundingSphere&, const C3DOrientationInterpolator&, EEngineInterpolationSet, unsigned char, CRGBColour) -- construct a 2D animating-mesh descriptor: primitive type 0x25, mesh index, clip box, alpha, orientation, an animation interpolator holding the given blend state at both ends, bounding sphere and tint
// Leaf: ??0CEnginePrimitive2DAnimatingMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00550A70, length 272 bytes (`ret 28h`: the unused 64-bit id + 8 arguments).
// Label correction: the manifest calls this "CEnginePrimitiveAnimation::CEnginePrimitiveAnimation" (4 parameters); the stores
// match the PDB CEnginePrimitive2DAnimatingMesh (0x80 bytes) exactly: ClipBox +0x0c, MeshIndex +0x1c, Alpha +0x20,
// Orientation +0x24, Anim +0x58, BoundingSphere +0x6c, TintColour +0x7c; type 0x25 (the 2D static mesh is 0x24).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (the animation interpolator's base). Vtables: 0x01239824 C3DOrientationInterpolator,
// 0x01239840 C3DAnimationInterpolator, 0x01239838 C3DAnimationBlendState.
// Model: the landed sibling CEnginePrimitive2DStaticMesh::CEnginePrimitive2DStaticMesh 0x00550940 (CEnginePrimitive base with the
// zeroing theme-channel ctor, member-wise copy ctors, CMeshTint::Set(r, g, b, a)). The animation interpolator is built from
// one blend state (`StartState(s), EndState(s)`, each an add-ref'ing copy of the counted animation pointer) after its
// out-of-line CBaseClass constructor; the orientation copy needs no base call (the base copy is implicit).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DVector { public: float X,Y,Z; C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){} C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} };
class CRightHandedSet { public: C3DVector Up,Forward; CRightHandedSet(const CRightHandedSet& o):Up(o.Up),Forward(o.Forward){} };
class C3DOrientationInterpolator : public CBaseClass { public:   // vtable 0x01239824
    CRightHandedSet StartOrientation,EndOrientation;
    C3DOrientationInterpolator(const C3DOrientationInterpolator& o):CBaseClass(o),StartOrientation(o.StartOrientation),EndOrientation(o.EndOrientation){}
};
struct CAnimationResource { void* vtable; long RefCount; };
class C3DAnimationBlendState : public CBaseClass { public:       // vtable 0x01239838
    CAnimationResource* Animation;
    C3DAnimationBlendState(const C3DAnimationBlendState& o):CBaseClass(o),Animation(o.Animation){ if (Animation) ++Animation->RefCount; }
};
class C3DAnimationInterpolator : public CBaseClass { public:     // vtable 0x01239840
    C3DAnimationBlendState StartState,EndState;
    C3DAnimationInterpolator(const C3DAnimationBlendState& state):StartState(state),EndState(state){}
};
class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF(const C2DBoxF& o):TLX(o.TLX),TLY(o.TLY),BRX(o.BRX),BRY(o.BRY){} };
class CRGBColour { public: unsigned char B,G,R,A; };
struct CMeshTint { unsigned char B,G,R,A; CMeshTint(const CRGBColour& o) { Set(o.R,o.G,o.B,o.A); } void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; } };
class CEnginePrimitiveThemeChannelDesc { public: unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad; CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){} };
class CEnginePrimitive { public: long Type,InterpolationSet; CEnginePrimitiveThemeChannelDesc ThemeChannelDesc; CEnginePrimitive(long type,long interpolation):Type(type),InterpolationSet(interpolation){} };
class C3DBoundingSphere { public: float X,Y,Z,Radius; C3DBoundingSphere(const C3DBoundingSphere& o):X(o.X),Y(o.Y),Z(o.Z),Radius(o.Radius){} };
struct CEnginePrimitive2DAnimatingMesh : CEnginePrimitive {   // PDB 0x80 bytes
    C2DBoxF ClipBox;                               // +0x0c
    unsigned long MeshIndex;                       // +0x1c
    unsigned char Alpha;                           // +0x20
    C3DOrientationInterpolator Orientation;        // +0x24
    C3DAnimationInterpolator Anim;                 // +0x58
    C3DBoundingSphere BoundingSphere;              // +0x6c
    CMeshTint TintColour;                          // +0x7c
    CEnginePrimitive2DAnimatingMesh(unsigned __int64 unused,unsigned long mesh,const C3DAnimationBlendState& anim,const C2DBoxF& box,
                                    const C3DBoundingSphere& sphere,const C3DOrientationInterpolator& orientation,long interpolation,
                                    unsigned char alpha,CRGBColour tint);
};
CEnginePrimitive2DAnimatingMesh::CEnginePrimitive2DAnimatingMesh(unsigned __int64 unused,unsigned long mesh,const C3DAnimationBlendState& anim,
    const C2DBoxF& box,const C3DBoundingSphere& sphere,const C3DOrientationInterpolator& orientation,long interpolation,
    unsigned char alpha,CRGBColour tint)
 : CEnginePrimitive(0x25,interpolation),ClipBox(box),MeshIndex(mesh),Alpha(alpha),Orientation(orientation),Anim(anim),
   BoundingSphere(sphere),TintColour(tint)
{ (void)unused; }