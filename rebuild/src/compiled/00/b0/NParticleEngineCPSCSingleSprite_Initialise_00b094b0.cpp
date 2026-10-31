// NParticleEngine::CPSCSingleSprite::Initialise(CIEngine* engine, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& parent) -- vtable slot 28: initialise the spline base, then (with an engine) create the sprite child primitive -- one per crossed sprite when CrossedSprites is set -- and the trail child primitive under the parent handle unless they already exist, recording their ids in PrimIDList, mark the component initialised and pass StayWithEmitter to the particle system; returns the initialised flag
// Leaf: ?Initialise@CPSCSingleSprite@
// Grade: RELOCATION_MATCH -- the crossed-sprite loop as i = 0, a count guard, then the zero C3DVector built once in the
//   guarded scope and a do/while (retail's preheader temporary; the parked 'lane gap: loop-invariant hoisting' was
//   source shape)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B094B0, 729 bytes (+ int3 padding), `ret 10h`. Ego_r.pdb
//   ?Initialise@CPSCSingleSprite@NParticleEngine@@UAE_NPAVCIEngine@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@
//   AAVCEnginePrimitiveHandle@@@Z (the signature of the landed CPSCRenderSprite::Initialise 0x00AF9310, slot 28).
// Callees: CPSCSplineBase::Initialise @ 0x00B215A0 (landed; qualified), CEnginePrimitiveSprite::CEnginePrimitiveSprite
//   @ 0x006558CE (landed; 15 stack words), the engine's virtual slot 26 (+0x68) CreateChildPrimitive(parent, primitive, 0),
//   vector<long>::resize(size_type, const long&) @ 0x004428F0.
// Layout: CPSCSingleSprite = Ego_r.pdb less 4 (the retail CArray inside the spline base is 12 bytes): SpriteBankIndex
//   +0x34, NoCrossedSprites +0x54, BlendMode +0x70, BlendOp +0x74, TrailBlendMode +0x78, TrailBlendOp +0x7c,
//   TrailBankIndex +0x80, TrailWidth +0x84, MaxTrailLength +0x88, the flag byte +0x93 (CrossedSprites bit 5),
//   StayWithEmitter +0x94 bit 0, TrailUpdatePrimitive +0xac (CEnginePrimitiveSpriteTrail = Ego_r.pdb less 8, the retail
//   STLport list is 4 bytes: MaxLength +0x14, GraphicIndex +0x18, GraphicFrame +0x1c, SkipPoints +0x20, Width +0x24,
//   Flags +0x44, BlendMode +0x48, BlendOp +0x4c), PrimIDList +0x12c; CParticleSystem StayWithEmitter = +0x50 bit 6 (as the
//   landed CParticleSystem::Initialise 0x00AF4010).
// Shape: "does this index still need a primitive" is an inline NeedsPrim(index) predicate (`size() <= i || list[i] == -1`;
//   written inline VC7.1 lays the already-created arm out of line differently); the sprite blend mode and op are read into
//   locals before the crossed-sprite test; each created id is stored after growing the list (inline one-argument resize
//   over the 0x004428F0 fill overload); the trail record's members are set in the order GraphicFrame, GraphicIndex,
//   SkipPoints, MaxLength, Flags, Width, BlendMode, BlendOp (found by a compile-only order search).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class CEnginePrimitive { public:
    long Type;                                      // +0x00
    long InterpolationSet;                          // +0x04
    unsigned char ThemeChannelDesc[4];              // +0x08
};
struct CEnginePrimitiveSprite : CEnginePrimitive {  // 0x48 bytes
    unsigned char _body[0x48 - 0x0c];
    CEnginePrimitiveSprite(const C3DVector& worldPos, unsigned long bankIndex, unsigned long frame, unsigned short flags,
                           float angle, float sizeX, float sizeY, long blendMode, unsigned char a, unsigned char r,
                           unsigned char g, unsigned char b, float fadeStart, long interpolationSet, long blendOp);   // 0x006558CE
};
struct CEnginePrimitiveSpriteTrail : CEnginePrimitive {   // retail 0x80 bytes
    void*          Trail;                           // +0x0c (STLport list head)
    bool           UpdateExisting;                  // +0x10
    unsigned long  MaxLength;                       // +0x14
    unsigned long  GraphicIndex;                    // +0x18
    unsigned long  GraphicFrame;                    // +0x1c
    float          SkipPoints;                      // +0x20
    float          Width;                           // +0x24
    unsigned long  HeadColour;                      // +0x28
    unsigned long  MidColour;                       // +0x2c
    unsigned long  TailColour;                      // +0x30
    float          FadeStart;                       // +0x34
    C3DVector      OffsetVector;                    // +0x38
    unsigned short Flags;                           // +0x44
    long           BlendMode;                       // +0x48
    long           BlendOp;                         // +0x4c
    unsigned char  WorldTransform[0x30];            // +0x50
};
class CEnginePrimitiveHandle;
class CIEngine { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25();
    virtual long CreateChildPrimitive(CEnginePrimitiveHandle& parent, const CEnginePrimitive& primitive, long flags);   // slot 26 (+0x68)
};
struct CLongVector {                                // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    long& operator[](unsigned int n) { return _M_start[n]; }
    void resize(unsigned int newSize, const long& value);    // 0x004428F0
    __forceinline void resize(unsigned int newSize) { resize(newSize, long()); }
};

namespace NParticleEngine {
class CParticleSystem { public:
    unsigned char _pad00[0x50];
    bool Initialised : 1;                           // +0x50 bit 0
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;
    bool Visible : 1;
    bool StayWithEmitter : 1;                       // bit 6
    bool ScaleParticles : 1;
};
template <class A, class B> class CParticleUpdateParameters;

class CPSCBase { public:
    virtual void v00();
    CParticleSystem* ParticleSystem;                // +0x04
    bool UpdateStarted : 1;                         // +0x08 bit 0
    bool Initialised : 1;                           // +0x08 bit 1
};
class CPSCSplineBase : public CPSCBase { public:
    unsigned char _spline[0x34 - 0x0c];             // SplineAnimSpeed, Spline, the two counted data pointers
    virtual bool Initialise(CIEngine* engine, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& parent);   // 0x00B215A0
};
class CPSCSingleSprite : public CPSCSplineBase { public:
    long          SpriteBankIndex;                  // +0x34
    unsigned long StartColour;                      // +0x38
    unsigned long MidColour;                        // +0x3c
    unsigned long EndColour;                        // +0x40
    float         AnimationTimeSecs;                // +0x44
    float         StartRenderSize;                  // +0x48
    float         EndRenderSize;                    // +0x4c
    long          SpriteAlignment;                  // +0x50
    long          NoCrossedSprites;                 // +0x54
    float         InitialAngle;                     // +0x58
    long          FadeInEndInteger;                 // +0x5c
    long          FadeOutBeginInteger;              // +0x60
    float         AlphaFadeMinimum;                 // +0x64
    float         SizeFadeMinimum;                  // +0x68
    unsigned long PositionParam;                    // +0x6c
    long          BlendMode;                        // +0x70
    long          BlendOp;                          // +0x74
    long          TrailBlendMode;                   // +0x78
    long          TrailBlendOp;                     // +0x7c
    long          TrailBankIndex;                   // +0x80
    float         TrailWidth;                       // +0x84
    long          MaxTrailLength;                   // +0x88
    long          StayWithEmitterIntFactor;         // +0x8c
    bool          UseStartColour;                   // +0x90
    bool          UseMidColour;                     // +0x91
    bool          UseEndColour;                     // +0x92
    bool          SelfIlluminating : 1;             // +0x93
    bool          AlphaFadeEnable : 1;
    bool          RotateAroundCentre : 1;
    bool          FaceMe2D : 1;
    bool          FaceMe3D : 1;
    bool          CrossedSprites : 1;               // bit 5
    bool          SizeFadeEnable : 1;
    bool          ForceAnimationTime : 1;
    bool          StayWithEmitter : 1;              // +0x94 bit 0
    unsigned char _pad95[0xac - 0x95];
    CEnginePrimitiveSpriteTrail TrailUpdatePrimitive;   // +0xac
    CLongVector   PrimIDList;                       // +0x12c
    bool NeedsPrim(unsigned int i) { return PrimIDList.size() <= i || PrimIDList[i] == -1; }
    virtual bool Initialise(CIEngine* engine, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& parent);
};

bool CPSCSingleSprite::Initialise(CIEngine* engine, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& parent)
{
    CPSCSplineBase::Initialise(engine, system, params, parent);
    if (engine) {
        unsigned int index = 0;
        if (SpriteBankIndex) {
            if (NeedsPrim(index)) {
                long blendMode = BlendMode;
                long blendOp = BlendOp;
                if (CrossedSprites) {
                    long i = 0;
                    if (NoCrossedSprites > 0) {
                        C3DVector zero(0.0f, 0.0f, 0.0f);
                        do {
                            long id = engine->CreateChildPrimitive(parent, CEnginePrimitiveSprite(zero, SpriteBankIndex, 0, 0, 0.0f, 1.0f, 1.0f, blendMode, 0, 0, blendOp, 0xff, 0.0f, 0, 0), 0);
                            if (PrimIDList.size() <= (unsigned int)i)
                                PrimIDList.resize(i + 1);
                            PrimIDList[i] = id;
                        } while (++i < NoCrossedSprites);
                    }
                    index = NoCrossedSprites;
                } else {
                    long id = engine->CreateChildPrimitive(parent, CEnginePrimitiveSprite(C3DVector(0.0f, 0.0f, 0.0f), SpriteBankIndex, 0, 0, 0.0f, 1.0f, 1.0f, blendMode, 0, 0, blendOp, 0xff, 0.0f, 0, 0), 0);
                    if (PrimIDList.size() <= 0)
                        PrimIDList.resize(1);
                    PrimIDList[0] = id;
                    index = 1;
                }
            } else
                index = CrossedSprites ? NoCrossedSprites : 1;
        }
        if (TrailBankIndex) {
            if (NeedsPrim(index)) {
                TrailUpdatePrimitive.GraphicFrame = 0;
                TrailUpdatePrimitive.GraphicIndex = TrailBankIndex;
                TrailUpdatePrimitive.SkipPoints = 0;
                TrailUpdatePrimitive.MaxLength = MaxTrailLength;
                TrailUpdatePrimitive.Flags = 0;
                TrailUpdatePrimitive.Width = TrailWidth;
                TrailUpdatePrimitive.BlendMode = TrailBlendMode;
                TrailUpdatePrimitive.BlendOp = TrailBlendOp;
                long id = engine->CreateChildPrimitive(parent, TrailUpdatePrimitive, 0);
                if (PrimIDList.size() <= index)
                    PrimIDList.resize(index + 1);
                PrimIDList[index] = id;
            }
        }
        Initialised = true;
    }
    system.StayWithEmitter = StayWithEmitter;
    return Initialised;
}
} // namespace NParticleEngine