// NParticleEngine::CPSCRenderSprite::Initialise(CIEngine* engine, CParticleSystem* system, CParticleUpdateParameters* params, CEnginePrimitiveHandle* parent) -- create the sprite child primitive (type 0x1b) and the trail child primitive (type 0x1a) under the parent handle when their banks are set and they do not exist yet, then mark the component initialised; returns the initialised flag
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF9310, 250 bytes (+ int3 padding), `ret 10h`; CPSCRenderSprite vtable 0x0129F67C slot 28.
// Callees: the engine's virtual slot 26 (+0x68) CreateChildPrimitive(CEnginePrimitiveHandle*, const SSpriteCreateInfo*, long).
// Layout: Ego_r.pdb CPSCRenderSprite (retail offsets equal): CPSCBase flags +8 (UpdateStarted bit 0, Initialised bit 1),
//   SpriteBankIndex +0x0c, TrailBankIndex +0x10, the render bit-field word +0x20 (BlendMode:3, TrailBlendMode:3, BlendOp:3,
//   TrailBlendOp:3, Flags:10), ChildPrimIndex +0x44, ChildTrailPrimIndex +0x50. The 0x1c-byte create record is a local
//   model (type, a zero dword, three zero bytes, bank, 16-bit flags, blend mode, blend op).
// Shape: the trail blend mode / op are read into locals before the first create; the sprite create reuses the loaded
//   bit-field word, the trail create re-reads it after the call.
class CEnginePrimitiveHandle;
struct SSpriteCreateInfo {
    long           Type;                           // +0x00
    long           Reserved;                       // +0x04
    bool           FlagA;                          // +0x08
    bool           FlagB;                          // +0x09
    bool           FlagC;                          // +0x0a
    long           BankIndex;                      // +0x0c
    unsigned short Flags;                          // +0x10
    unsigned long  BlendMode;                      // +0x14
    unsigned long  BlendOp;                        // +0x18
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
    virtual long CreateChildPrimitive(CEnginePrimitiveHandle* parent, const SSpriteCreateInfo* info, long flags);   // slot 26 (+0x68)
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
class CPSCRenderSprite : public CPSCBase { public:               // vtable 0x0129F67C
    long          SpriteBankIndex;                 // +0x0c
    long          TrailBankIndex;                  // +0x10
    unsigned long StartColour;                     // +0x14
    unsigned long MidColour;                       // +0x18
    unsigned long EndColour;                       // +0x1c
    unsigned long BlendMode : 3;                   // +0x20
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long Flags : 10;
    unsigned long _bits20 : 10;
    unsigned char _pad24[0x44 - 0x24];
    long          ChildPrimIndex;                  // +0x44
    unsigned char _pad48[0x50 - 0x48];             // +0x48 TrailPrimHandle
    long          ChildTrailPrimIndex;             // +0x50
    bool Initialise(CIEngine* engine, CParticleSystem* system, CParticleUpdateParameters<unsigned long, unsigned long>* params, CEnginePrimitiveHandle* parent);
};

bool CPSCRenderSprite::Initialise(CIEngine* engine, CParticleSystem* system, CParticleUpdateParameters<unsigned long, unsigned long>* params, CEnginePrimitiveHandle* parent)
{
    if (engine) {
        unsigned long trailBlendOp = TrailBlendOp;
        unsigned long trailBlendMode = TrailBlendMode;
        SSpriteCreateInfo info;
        if (SpriteBankIndex && ChildPrimIndex == -1) {
            info.Type = 0x1b;
            info.Reserved = 0;
            info.FlagA = false;
            info.FlagB = false;
            info.FlagC = false;
            info.BankIndex = SpriteBankIndex;
            info.Flags = (unsigned short)Flags;
            info.BlendMode = BlendMode;
            info.BlendOp = BlendOp;
            ChildPrimIndex = engine->CreateChildPrimitive(parent, &info, 0);
        }
        if (TrailBankIndex && ChildTrailPrimIndex == -1) {
            info.Type = 0x1a;
            info.Reserved = 0;
            info.FlagA = false;
            info.FlagB = false;
            info.FlagC = false;
            info.BankIndex = TrailBankIndex;
            info.Flags = (unsigned short)Flags;
            info.BlendMode = trailBlendMode;
            info.BlendOp = trailBlendOp;
            ChildTrailPrimIndex = engine->CreateChildPrimitive(parent, &info, 0);
        }
        Initialised = true;
    }
    return Initialised;
}
}