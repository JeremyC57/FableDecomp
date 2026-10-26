// NParticleEngine::CPSCRenderMesh::Initialise(CIEngine* engine, CParticleSystem* system, CParticleUpdateParameters* params, CEnginePrimitiveHandle* parent) -- create the mesh child primitive (type 0x1c, identity transform) and the trail child primitive (type 0x1a) under the parent handle when their banks are set and they do not exist yet, then mark the component initialised; returns the initialised flag
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFF0A0, 266 bytes (+ int3 padding), `ret 10h`; CPSCRenderMesh vtable 0x0129F96C slot 28.
// Label: the catalog's "CPSCRenderSprite::Initialise" is a propagated name; the vtable slot and the offsets are
//   CPSCRenderMesh's (the sprite one is 0x00AF9310).
// Callees: SMeshCreateInfo(long bank, const CMatrix3x4& transform, unsigned long blendMode, unsigned long blendOp) 0x00AFEAD0
//   (label "C3DMeshFileBoneChunk::SetLocalizationMatrix" is propagated), the engine's virtual slot 26 (+0x68)
//   CreateChildPrimitive(CEnginePrimitiveHandle*, const SPrimitiveCreateInfo*, long).
// Layout: Ego_r.pdb CPSCRenderMesh (retail offsets equal): CPSCBase flags +8 (Initialised bit 1), BankIndex +0x0c,
//   TrailBankIndex +0x10, the render bit-field word +0x2c (BlendMode:3, TrailBlendMode:3, BlendOp:3, TrailBlendOp:3),
//   ChildPrimIndex +0x54, ChildTrailPrimIndex +0x60.
// Shape: the mesh record is a temporary built by its out-of-line ctor inside the create call (engine vtable fetched first),
//   from an identity matrix in its own block (top-down zero fill, then the diagonal); the trail record is a block local.
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public:
    C3DVector Right, Up, Look, Pos;
    void SetIdentity()
    {
        Right.Y = Right.Z = Up.X = Up.Z = Look.X = Look.Y = Pos.X = Pos.Y = Pos.Z = 0.0f;
        Right.X = Up.Y = Look.Z = 1.0f;
    }
};
class CEnginePrimitiveHandle;
struct SPrimitiveCreateInfo {
    long           Type;                           // +0x00
    long           Reserved;                       // +0x04
    bool           FlagA;                          // +0x08
    bool           FlagB;                          // +0x09
    bool           FlagC;                          // +0x0a
    long           BankIndex;                      // +0x0c
};
struct STrailCreateInfo : SPrimitiveCreateInfo {   // 0x1c
    unsigned short Flags;                          // +0x10
    unsigned long  BlendMode;                      // +0x14
    unsigned long  BlendOp;                        // +0x18
};
struct SMeshCreateInfo : SPrimitiveCreateInfo {    // 0x48
    CMatrix3x4    Transform;                       // +0x10
    unsigned long BlendMode;                       // +0x40
    unsigned long BlendOp;                         // +0x44
    SMeshCreateInfo(long bank, const CMatrix3x4& transform, unsigned long blendMode, unsigned long blendOp);   // 0x00AFEAD0
};
class CIEngine { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual long CreateChildPrimitive(CEnginePrimitiveHandle* parent, const SPrimitiveCreateInfo* info, long flags);   // slot 26 (+0x68)
};
namespace NParticleEngine {
class CParticleSystem;
template <class A, class B> class CParticleUpdateParameters;
class CPSCBase { public:
    virtual void v00();
    CParticleSystem* ParticleSystem;               // +0x04
    bool UpdateStarted : 1;                        // +0x08
    bool Initialised : 1;
};
class CPSCRenderMesh : public CPSCBase { public:                 // vtable 0x0129F96C
    long          BankIndex;                       // +0x0c
    long          TrailBankIndex;                  // +0x10
    unsigned long Colours[6];                      // +0x14
    unsigned long BlendMode : 3;                   // +0x2c
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long _bits2c : 20;
    unsigned char _pad30[0x54 - 0x30];
    long          ChildPrimIndex;                  // +0x54
    unsigned char _pad58[0x60 - 0x58];             // +0x58 TrailPrimHandle
    long          ChildTrailPrimIndex;             // +0x60
    bool Initialise(CIEngine* engine, CParticleSystem* system, CParticleUpdateParameters<unsigned long, unsigned long>* params, CEnginePrimitiveHandle* parent);
};

bool CPSCRenderMesh::Initialise(CIEngine* engine, CParticleSystem* system, CParticleUpdateParameters<unsigned long, unsigned long>* params, CEnginePrimitiveHandle* parent)
{
    if (engine) {
        if (BankIndex && ChildPrimIndex == -1) {
            CMatrix3x4 identity;
            identity.SetIdentity();
            unsigned long blendMode = BlendMode;
            unsigned long blendOp = BlendOp;
            ChildPrimIndex = engine->CreateChildPrimitive(parent, &SMeshCreateInfo(BankIndex, identity, blendMode, blendOp), 0);
        }
        if (TrailBankIndex && ChildTrailPrimIndex == -1) {
            STrailCreateInfo info;
            info.Type = 0x1a;
            info.Reserved = 0;
            info.FlagA = false;
            info.FlagB = false;
            info.FlagC = false;
            info.BankIndex = TrailBankIndex;
            info.Flags = 0;
            info.BlendMode = TrailBlendMode;
            info.BlendOp = TrailBlendOp;
            ChildTrailPrimIndex = engine->CreateChildPrimitive(parent, &info, 0);
        }
        Initialised = true;
    }
    return Initialised;
}
}