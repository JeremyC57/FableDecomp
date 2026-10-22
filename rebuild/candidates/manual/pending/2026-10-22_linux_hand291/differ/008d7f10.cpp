// Grade: DIFFER(527v523) -- the flash sub-def's frame word is 18h (retail reuses 14h for the effects sub-def) and the created effect lands in edi instead of ebx (retail keeps edi = 0 for the final effects release); the leading flag-home store, globals, effect block and release order match
// NCreatureMode::CModeJackOfBladesFlash::OnActivate() -- read the flash timings from the mode's sub-def into the shared flash globals, activate the base mode, arm the timer, spawn the FLASH_SWORD_POWER_UP effect at the creature's WEAPON_FOCUS_01 dummy and keep it as the emitter, then fade in the Jack of Blades environment theme
// Leaf: ?OnActivate@CModeJackOfBladesFlash
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7F10, 523 bytes (`ret`).
// Callees: CDefClassBase::GetSubDef<flash def> @ 0x008C33E0 (manifest "GetSubDef<CAICreatureWillPowerIndicatorDef>", propagated),
//   CModeBase::OnActivate @ 0x0073AAC0, CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580, CCRC::CalcAndLookup @ 0x00669D30,
//   CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::GetPosition @ 0x004C73D0,
//   CTCGraphicAppearance::GetDummyObject @ 0x004C08C0, the special-effect Create @ 0x006E0880, CBaseIntelligentPointer
//   @ 0x00A01B10 / 0x00A01B50 / 0x00A01B90 / 0x00A01C10, CGameDefinitionManager::Get @ 0x0044C6B0,
//   CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CModeBase::GetWorld @ 0x0073A680,
//   CEnvironment::SetThemeWeightAllChannels(long, long, float) @ 0x006B38D0, __ftol2.
// Data: "FLASH_SWORD_POWER_UP" 0x01285810, "WEAPON_FOCUS_01" 0x0126515C, "ENVIRONMENT_JACK_OF_BLADES_FADE" 0x012857F0,
//   game frames per second (long) @ 0x01375550, 1/30 @ 0x012856DC; the flash timing globals 0x01388A7C / 0x01388A80 /
//   0x01388A84 / 0x013BAFD8 / 0x013BAFDC / 0x013BAFE0.
// Layout: as the landed ctor 0x008D7E60 / OnDeactivate 0x008DF310: Timer +0x68, BurnCounter +0x6c, PCreature +0x7c,
//   PEmitter +0x88; CThing graphic appearance +0x64, def +0x70; the flash sub-def's two durations (seconds) +0x6c / +0x70;
//   the world's environment +0x18.
// Shape: both GetSubDef instantiations have the same-TU out-slot-only stand-in (landed 0x008D9870 idiom), so the flash def
//   stays in ebp, its frame word is reused by the effects def, and it is released by hand at the end (no null test); the
//   effect lookup is the `&&` with a conditional CCharString temporary (flag in ebx).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool CalcAndLookup(const CCharString& name, long* type);   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail): it writes the out type only, so VC7.1 knows the effect id's address does not
// escape the call (the landed 0x008D9870 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* type)
{
    long found = Lookup_CSpecialEffectsDef_CalcAndLookup(this, name);
    if (found < 0)
        return false;
    *type = found;
    return true;
}
#pragma auto_inline(on)
struct CJackOfBladesFlashDef : CDefBase {
    unsigned char _pad08[0x6c - 8];
    float         PowerUpTime;                  // +0x6c (seconds)
    float         FlashTime;                    // +0x70 (seconds)
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) { if (DecRefToZero(m_p->m_refCount)) m_p->Destroy(); } }
};
template <class T> struct CRawDefPointer {
    T* m_p;
    CRawDefPointer() : m_p(0) { }
};
struct CDefClassBase : CDefBase {
    template <class P> bool GetSubDef(P* out);  // 0x008C33E0 (flash def, stand-in body below)
    bool GetSubDef(CRawDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (out of line in retail): it writes the out slot only, so VC7.1 knows the def pointer's address does not
// escape the call (the landed 0x008D9870 idiom).
#pragma auto_inline(off)
template <class P> __declspec(noinline) bool CDefClassBase::GetSubDef(P* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position, CRightHandedSet* axes, float scale);   // 0x004C08C0
};
struct CThing {
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    unsigned char         _pad68[0x70 - 0x68];
    CDefClassBase*        PBaseDef;             // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

struct CCharString;
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();            // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};
struct CEnvironment {
    void SetThemeWeightAllChannels(long channel, long theme, float weight);   // 0x006B38D0
};
struct CWorld {
    unsigned char _pad00[0x18];
    CEnvironment* Environment;                  // +0x18
};

extern long g_GameFramesPerSecond;              // 0x01375550
extern const float g_OneThirtieth;              // 0x012856DC (1/30)
extern long g_FlashPowerUpFrames;               // 0x01388A7C
extern long g_FlashFrames;                      // 0x01388A80
extern long g_FlashPhaseCount;                  // 0x01388A84
extern long g_FlashTotalFrames;                 // 0x013BAFD8
extern long g_FlashStartFrame;                  // 0x013BAFDC
extern long g_FlashFadeFrames;                  // 0x013BAFE0

namespace NCreatureMode {
struct CModeBase {                              // vtable 0x012673CC
    virtual ~CModeBase();
    CThing*       m_pThing;                     // +0x04
    unsigned char _pad08[0x14];
    void OnActivate();                          // 0x0073AAC0
    CWorld* GetWorld() const;                   // 0x0073A680
};
struct CModeStandBase : CModeBase {
    unsigned char _pad1c[0x4c];                 // +0x1c .. +0x68
};
struct CModeJackOfBladesFlash : CModeStandBase {
    long                     Timer;             // +0x68
    long                     BurnCounter;       // +0x6c
    unsigned char            _filter[0xc];      // +0x70 CAnimFilterOfGroup Filter
    CThing*                  PCreature;         // +0x7c
    CBaseIntelligentPointer  PTarget;           // +0x80
    CIntelligentPointerThing PEmitter;          // +0x88
    CBaseIntelligentPointer  PSpotLight;        // +0x90
    virtual void OnActivate();
};

void CModeJackOfBladesFlash::OnActivate()
{
    CRawDefPointer<CJackOfBladesFlashDef> flashSlot;
    m_pThing->PBaseDef->GetSubDef(&flashSlot);
    CJackOfBladesFlashDef* flash = flashSlot.m_p;
    g_FlashPowerUpFrames = (long)(g_GameFramesPerSecond * flash->PowerUpTime);
    g_FlashFrames = (long)(g_GameFramesPerSecond * flash->FlashTime);
    g_FlashTotalFrames = g_FlashPowerUpFrames + g_FlashFrames;
    g_FlashStartFrame = g_FlashTotalFrames - g_FlashFrames;
    g_FlashFadeFrames = g_FlashFrames;
    g_FlashPhaseCount = 10;
    CModeBase::OnActivate();
    Timer = g_FlashPowerUpFrames + g_FlashFrames;
    BurnCounter = 0;
    CRawDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    {
        bool found = m_pThing->PBaseDef->GetSubDef(&effects) && effects.m_p->CalcAndLookup("FLASH_SWORD_POWER_UP", &effect);
        if (found) {
            C3DVector position = PCreature->GetPosition();
            CRightHandedSet axes;
            PCreature->PGraphicAppearance->GetDummyObject("WEAPON_FOCUS_01", 0, 0, &position, &axes, 1.0f);
            CThing* emitter = CreateSpecialEffect(effect, position, false);
            if (emitter)
                PEmitter = CIntelligentPointerThing(emitter);
        }
        long fadeTheme = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("ENVIRONMENT_JACK_OF_BLADES_FADE");
        CWorld* world = GetWorld();
        world->Environment->SetThemeWeightAllChannels(3, fadeTheme, g_FlashFadeFrames * g_OneThirtieth);
    }
    if (effects.m_p) {
        if (DecRefToZero(effects.m_p->m_refCount))
            effects.m_p->Destroy();
    }
    if (DecRefToZero(flash->m_refCount))
        flash->Destroy();
}
}
