#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#endif
#include <new>
// CTCFireballSpell::FrameUpdate() -- per frame: count down the release delay, tick the will-button timer, spawn the build-up effect
// once (attached to the fireball helper), run the level-up effects, drive the held state and remove the spell when finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C93B0, 444 bytes (+ int3 padding), `ret` (vtable 0x0126E964 slot 9).
// Leaf: ?FrameUpdate@CTCFireballSpell
// Callees: CWillButtonHelperTimer::FrameUpdate 0x008EDDD0, GetSubDef<CSpecialEffectsDef> 0x00669580, GetEffectFromString 0x00669D30,
// GetPosition 0x004C73D0, CreateParticleEffect 0x0072AB70, CBaseIntelligentPointer 0x00A01B10/B90/B50/C10, LowerBound 0x0040F020,
// AttachToThing 0x006E0BE0, ProcessLevellingUpFX 0x007C8DE0, the held-state helper 0x007C9180, CThing::RemoveTC 0x004C9840.
// Shape: the Kraken/OnDie def lookup (out slot read once into a named pointer released by hand), an implicit-literal `&&` temp,
// the effect through an intelligent-pointer temporary + operator=, an uninitialised emitter local; first try.
// Names: catalog "FrameUpdate" sits on 0x007C9180, but this row is the vtable slot-9 override; 0x007C9180 is a non-virtual helper.
// Layout: retail CTCFireballSpell = Ego_r.pdb minus 4 after the timer; timer CurrentState +0x20, WillIncrementedFlag +0x38.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {          // the out slot GetSubDef fills
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
struct CTCBase_Retail;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20 UsingComponents
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void RemoveTC(long interfaceType);          // 0x004C9840
    __forceinline unsigned long HasTC(long kind) const { return m_presentBits[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase_Retail* FindTC(long kind)
    {
        long type = kind;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || kind < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    void MakeNoise(const CCharString& noise);   // 0x00662110
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& position);   // 0x0072AB70
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CTCSoundPlayer {                         // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
struct CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CWillButtonHelperTimer {                 // retail 0x34 bytes
    void FrameUpdate();                         // 0x008EDDD0
    long ButtonHeldDownTimer;                   // +0x00
    long WillAccumulatedLevel;                  // +0x04
    unsigned char PCreature[8];                 // +0x08 CIntelligentPointer<CThing>
    long MyAbility;                             // +0x10
    long CurrentState;                          // +0x14
    void* WillPowerUpAccumulatedTimings[3];     // +0x18
    float MaxNumberLevels;                      // +0x24
    float CurMaxLevels;                         // +0x28
    bool WillIncrementedFlag;                   // +0x2c
    bool UpdateAccLevel;
    bool IsAICasting;
    bool IsAIHoldingDown;
    long LevelToCast;                           // +0x30
};

class CTCFireballSpell {
public:
    virtual void v00();
    CThingCreatureBase* ParentThing;            // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CWillButtonHelperTimer Timer;               // +0x0c
    CDefClassBase* PDef;                        // +0x40 CDefPointer<CSpecialAbilitiesFireballSpellDef const>
    unsigned long InitLoopingSound;             // +0x44
    CIntelligentPointer FireballBuildUpEffect;  // +0x48
    bool ImmediateCastMode;                     // +0x50
    bool OneAnimCastMode;                       // +0x51
    unsigned char PTarget[8];                   // +0x52
    unsigned char PItemRemoved[8];              // +0x5a
    long WeaponState;                           // +0x64
    bool LeftHandAnim;                          // +0x68
    CCharString FireballAttachmentName;         // +0x6c
    bool StartEffect;                           // +0x70
    long MaxLevelAllowableByStamina;            // +0x74
    long ReleaseDelayFrameCount;                // +0x78
    bool SpellFinished;                         // +0x7c
    void ProcessLevellingUpFX();                // 0x007C8DE0
    void UpdateButtonHeld();                    // 0x007C9180 (catalog "FrameUpdate"; non-virtual, called only from here)
    void FrameUpdate();
};

void CTCFireballSpell::FrameUpdate()
{
    if (SpellFinished)
        --ReleaseDelayFrameCount;
    Timer.FrameUpdate();
    CThingCreatureBase* parent = ParentThing;
    if (StartEffect) {
        StartEffect = false;
        CDefPointer<CSpecialEffectsDef> effectsSlot;
        long effectType = 0;
        bool found = PDef->GetSubDef(effectsSlot) && effectsSlot.m_p->GetEffectFromString("BUILDUP_EFFECT", effectType);
        CSpecialEffectsDef* effects = effectsSlot.m_p;
        if (found) {
            FireballBuildUpEffect = CIntelligentPointer(CreateParticleEffect(effectType, parent->GetPosition()));
            if (IS_THING_ALIVE(FireballBuildUpEffect)) {
                CTCDParticleEmitter* emitter;
                CThing* effect = FireballBuildUpEffect.GetPItem();
                if (effect->HasTC(0x4b))
                    emitter = (CTCDParticleEmitter*)effect->FindTC(0x4b);
                emitter->AttachToThing(ParentThing, 10, FireballAttachmentName, 0, 0.0f);
            }
        }
        ReleaseDef(effects);
    }
    if (Timer.WillIncrementedFlag && Timer.WillAccumulatedLevel <= MaxLevelAllowableByStamina)
        ProcessLevellingUpFX();
    switch (Timer.CurrentState) {
    case 2:
        UpdateButtonHeld();
        break;
    case 3:
        if (SpellFinished && ReleaseDelayFrameCount <= 0)
            parent->RemoveTC(0x9d);
        break;
    }
}

#include <cstdio>
#include <cstring>
#include <cstdlib>
CCharString::CCharString(const char* text, long) : m_pData((void*)text) { }
CCharString::~CCharString() { }
static int g_destroyed = 0;
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() { g_destroyed++; }
static CSpecialEffectsDef g_fx; static int g_haveFx = 1;
void* Find_CDefClassBase_GetSubDef(const void*) { if (!g_haveFx) return 0; g_fx.m_refCount++; return &g_fx; }
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void*, const CCharString& name) { return std::strcmp((const char*)name.m_pData, "BUILDUP_EFFECT") == 0 ? 33 : -1; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static C3DVector g_pos;
const C3DVector& CThing::GetPosition() const { return g_pos; }
static long g_removed = -1;
void CThing::RemoveTC(long t) { g_removed = t; }
void CThingCreatureBase::MakeNoise(const CCharString&) { }
static CThing g_effect; static long g_createdType = -1;
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector&) { g_createdType = effect; return &g_effect; }
static CThing* g_attachThing = 0; static CTCDParticleEmitter* g_attachEmitter = 0;
void CTCDParticleEmitter::AttachToThing(CThing* thing, long, const CCharString&, long, float) { g_attachEmitter = this; g_attachThing = thing; }
unsigned long CTCSoundPlayer::PlaySound(const CCharString&, long, long, long, long) { return 0; }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = (CBaseObject*)item; }
static int g_timer = 0, g_levelFx = 0, g_held = 0;
void CWillButtonHelperTimer::FrameUpdate() { g_timer++; }
void CTCFireballSpell::ProcessLevellingUpFX() { g_levelFx++; }
void CTCFireballSpell::UpdateButtonHeld() { g_held++; }
void CTCFireballSpell::v00() { }

int main()
{
    static CThingCreatureBase parent; static CTCDParticleEmitter emitter; static CDefClassBase spellDef;
    CInterfaceMap::SEntry ee[1]; ee[0].type = 0x4b; ee[0].component = (CTCBase_Retail*)&emitter;
    g_effect.m_interfaces.first = ee; g_effect.m_interfaces.last = ee + 1; g_effect.m_presentBits[2] = 1UL << 11;
    void* mem = std::calloc(1, sizeof(CTCFireballSpell));
    CTCFireballSpell* f = (CTCFireballSpell*)mem;
    new (&f->FireballBuildUpEffect) CIntelligentPointer(0);
    f->ParentThing = &parent; f->PDef = &spellDef;
    int ok = 1;
    // first frame: effect spawned + attached, state 2 -> held helper, level fx when flag set and level <= stamina max
    g_fx.m_refCount = 1; f->StartEffect = true; f->Timer.CurrentState = 2; f->Timer.WillIncrementedFlag = true;
    f->Timer.WillAccumulatedLevel = 2; f->MaxLevelAllowableByStamina = 2; f->ReleaseDelayFrameCount = 5;
    f->FrameUpdate();
    if (f->StartEffect || g_createdType != 33 || f->FireballBuildUpEffect.GetPItem() != &g_effect || g_attachEmitter != &emitter || g_attachThing != &parent) ok = 0;
    if (g_timer != 1 || g_levelFx != 1 || g_held != 1 || g_fx.m_refCount != 1 || g_destroyed != 0 || f->ReleaseDelayFrameCount != 5) ok = 0;
    // finished, state 3: delay counts down 1 -> 0, component removed; level above stamina -> no level fx
    f->SpellFinished = true; f->ReleaseDelayFrameCount = 1; f->Timer.CurrentState = 3; f->Timer.WillAccumulatedLevel = 3;
    f->FrameUpdate();
    if (f->ReleaseDelayFrameCount != 0 || g_removed != 0x9d || g_levelFx != 1 || g_held != 1 || g_createdType != 33) ok = 0;
    // not yet: delay 3 -> 2, nothing removed
    g_removed = -1; f->ReleaseDelayFrameCount = 3;
    f->FrameUpdate();
    if (f->ReleaseDelayFrameCount != 2 || g_removed != -1) ok = 0;
    if (!ok) { std::printf("007c93b0_TEST FAIL\n"); return 1; }
    std::printf("007c93b0_TEST PASS\n");
    return 0;
}
