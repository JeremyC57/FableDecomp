// CTCDrunkenness::CTCDrunkenness(CThing*) -- CTCBase(thing), never sobered, seconds per frame from the frame rate, and a per-creature drunkenness threshold drawn from the drunkenness ability def (0.5 .. 1.5 times its base threshold)
// Leaf: ??0CTCDrunkenness
//
// Retail 0x007BB800, 147 bytes (`ret 4`). Ego_r.pdb `??0CTCDrunkenness@@QAE@AAVCThing@@@Z`. Callees:
// CTCBase::CTCBase(CThing*) @ 0x00686800, CDefClassBase::GetSubDef<CSpecialAbilitiesDrunkennessDef>
// @ 0x007BB920 (manifest name is a byte-twin template), CTCBase::GetPLocalSeed() @ 0x00686DA0
// (manifest "CViewBase::GetDisplayEngine", propagated), GFFloatRandom(float, unsigned long*) @
// 0x004978A0 (`__fastcall`: the seed in ecx, the float on the stack). Globals: 0x01375550 the integer
// frame rate, 0x0126DEA4 1.0f, 0x0122F59C 0.5f. Vtable 0x0126DEB4 = CTCDrunkenness.
// Layout = Ego_r.pdb CTCDrunkenness (0x20): FrameLastUpdatedSobering +0x14, SecondsPerFrame +0x18,
// ThisDrunkennessThreshold +0x1c (Drunk / Drunkenness are left to OnInitialActivate); retail CThing's
// base def pointer object at +0x70; the ability def's base threshold at +0x28.
// Shape: the thing's def comes from an inline by-value getter (null-tested add-ref into edi, GetSubDef,
// null-tested release); the ability def pointer is a named local zeroed in the dead argument slot and
// released by its destructor with no null test (load/dec/store); `1.0f / frameRate` is `fdivr`.
class CDefClassBase;
class CSpecialAbilitiesDrunkennessDef;

template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object != 0) Object->AddRef(); }
    ~CDefPointer() { if (Object != 0) Object->Release(); }
    T* operator->() const { return Object; }
};

class CDefClassBase { public:
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long RefCount;                                          // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DestroySelf(); }
    bool GetSubDef(struct CDrunkennessDefPointer* out);     // 0x007BB920
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(struct CDrunkennessDefPointer* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CSpecialAbilitiesDrunkennessDef { public:
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long          RefCount;                                 // +0x04
    unsigned char _pad08[0x28 - 0x08];
    float         DrunkennessThreshold;                     // +0x28
    void Release() { if (--RefCount == 0) DestroySelf(); }
};

struct CDrunkennessDefPointer {                             // CDefPointer<const CSpecialAbilitiesDrunkennessDef>
    CSpecialAbilitiesDrunkennessDef* Object;
    CDrunkennessDefPointer() : Object(0) { }
    ~CDrunkennessDefPointer() { Object->Release(); }
    CSpecialAbilitiesDrunkennessDef* operator->() const { return Object; }
};

class CThing { public:
    unsigned char             _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                        // +0x70
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
};

extern long g_FrameRate;                                    // 0x01375550

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                                 // 0x00686800
    unsigned long* GetPLocalSeed() const;                   // 0x00686DA0
};

class CTCDrunkenness : public CTCBase { public:                           // vtable 0x0126DEB4
    bool  Drunk;                                            // +0x0c
    float Drunkenness;                                      // +0x10
    long  FrameLastUpdatedSobering;                         // +0x14
    float SecondsPerFrame;                                  // +0x18
    float ThisDrunkennessThreshold;                         // +0x1c
    virtual void v00();
    CTCDrunkenness(CThing* thing);
};

CTCDrunkenness::CTCDrunkenness(CThing* thing)
    : CTCBase(thing)
    , FrameLastUpdatedSobering(0)
    , SecondsPerFrame(1.0f / g_FrameRate)
{
    CDrunkennessDefPointer def;
    ParentThing->GetDef()->GetSubDef(&def);
    float factor = GFFloatRandom(1.0f, GetPLocalSeed()) + 0.5f;
    ThisDrunkennessThreshold = factor * def->DrunkennessThreshold;
}