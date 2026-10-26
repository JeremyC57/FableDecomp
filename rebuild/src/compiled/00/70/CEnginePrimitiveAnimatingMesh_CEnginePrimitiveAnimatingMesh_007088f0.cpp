// CEnginePrimitiveAnimatingMesh::CEnginePrimitiveAnimatingMesh(unsigned __int64, unsigned long, const C3DAnimationInterpolator&, const C3DPositionInterpolator&, const C3DOrientationInterpolator&, float, bool, unsigned char, CRGBColour, float, EEngineInterpolationSet, const CEnginePrimitiveThemeChannelDesc&, bool) -- construct an animating-mesh descriptor: primitive type 9, mesh index, size, fade start, alpha, lit flag, position/orientation/animation interpolators (copied), no mesh effect, theme channel, tint and the bullet-time cloth flag
// Leaf: ??0CEnginePrimitiveAnimatingMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007088F0, length 304 bytes (`ret 38h`: the unused 64-bit id + 12 arguments).
// Callees: none (every copy constructor is inline). Vtables: 0x0123981C C3DPositionInterpolator, 0x01239824
// C3DOrientationInterpolator, 0x01239840 C3DAnimationInterpolator, 0x01239838 C3DAnimationBlendState.
// Layout: PDB CEnginePrimitiveAnimatingMesh (0x90 bytes, retail identical): MeshIndex +0x0c, Size +0x10, FadeStart +0x14,
// Alpha +0x18, Lit bit 0 of +0x19, WorldPos +0x1c, Orientation +0x38, Anim +0x6c, MeshEffect +0x80, ThemeChannelDesc +0x84,
// TintColour +0x88, BulletTimeAffectsCloth +0x8c. Model = the landed CEnginePrimitive2DAnimatingMesh ctor 0x00550A70 /
// CEnginePrimitive2DStaticMesh ctor 0x00550940 (CEnginePrimitive base, member-wise copy ctors, CMeshTint::Set).
// Shape: the animation interpolator is COPIED (both blend states add-ref their animation; no CBaseClass call: the base copy
// is implicit); the lit flag is a one-bit init-list member (`xor ; and 1 ; xor` merge); one `xor ebx,ebx` zero register
// serves the theme-channel bytes, the null tests and the empty MeshEffect.
class CBaseClass { public:
    virtual ~CBaseClass();
    CBaseClass();                                  // 0x0099A2D0
};
class C3DVector { public: float X,Y,Z; C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){} };
class CRightHandedSet { public: C3DVector Up,Forward; CRightHandedSet(const CRightHandedSet& o):Up(o.Up),Forward(o.Forward){} };
class C3DPositionInterpolator : public CBaseClass { public:      // vtable 0x0123981C
    C3DVector StartPosition,EndPosition;
    C3DPositionInterpolator(const C3DPositionInterpolator& o):CBaseClass(o),StartPosition(o.StartPosition),EndPosition(o.EndPosition){}
};
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
    C3DAnimationInterpolator(const C3DAnimationInterpolator& o):CBaseClass(o),StartState(o.StartState),EndState(o.EndState){}
};
class CEngineMeshEffect;
template <class T> class CIVCountedPointer { public: T* m_p; CIVCountedPointer():m_p(0){} };
class CRGBColour { public: unsigned char B,G,R,A; };
struct CMeshTint { unsigned char B,G,R,A; CMeshTint(const CRGBColour& o) { Set(o.R,o.G,o.B,o.A); } void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; } };
class CEnginePrimitiveThemeChannelDesc { public: unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad; CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){} };
class CEnginePrimitive { public: long Type,InterpolationSet; CEnginePrimitiveThemeChannelDesc ThemeChannelDesc; CEnginePrimitive(long type,long interpolation):Type(type),InterpolationSet(interpolation){} };
struct CEnginePrimitiveAnimatingMesh : CEnginePrimitive {   // PDB 0x90 bytes
    unsigned long MeshIndex;                       // +0x0c
    float Size;                                    // +0x10
    float FadeStart;                               // +0x14
    unsigned char Alpha;                           // +0x18
    unsigned char Lit : 1;                         // +0x19 bit 0
    C3DPositionInterpolator WorldPos;              // +0x1c
    C3DOrientationInterpolator Orientation;        // +0x38
    C3DAnimationInterpolator Anim;                 // +0x6c
    CIVCountedPointer<CEngineMeshEffect> MeshEffect;   // +0x80
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc; // +0x84
    CMeshTint TintColour;                          // +0x88
    bool BulletTimeAffectsCloth;                   // +0x8c
    CEnginePrimitiveAnimatingMesh(unsigned __int64 unused,unsigned long mesh,const C3DAnimationInterpolator& anim,
        const C3DPositionInterpolator& position,const C3DOrientationInterpolator& orientation,float size,bool lit,
        unsigned char alpha,CRGBColour tint,float fadeStart,long interpolation,const CEnginePrimitiveThemeChannelDesc& theme,
        bool bulletTimeAffectsCloth);
};
CEnginePrimitiveAnimatingMesh::CEnginePrimitiveAnimatingMesh(unsigned __int64 unused,unsigned long mesh,const C3DAnimationInterpolator& anim,
    const C3DPositionInterpolator& position,const C3DOrientationInterpolator& orientation,float size,bool lit,
    unsigned char alpha,CRGBColour tint,float fadeStart,long interpolation,const CEnginePrimitiveThemeChannelDesc& theme,
    bool bulletTimeAffectsCloth)
 : CEnginePrimitive(9,interpolation),MeshIndex(mesh),Size(size),FadeStart(fadeStart),Alpha(alpha),Lit(lit),WorldPos(position),
   Orientation(orientation),Anim(anim),MeshEffect(),ThemeChannelDesc(theme),TintColour(tint),BulletTimeAffectsCloth(bulletTimeAffectsCloth)
{ (void)unused; }