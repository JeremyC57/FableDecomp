// Behaviour test for 0x00836260 (copy of the model + stand-in callee bodies).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __thiscall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingDeadCreature::WorldUpdate() -- game-object update and body reorientation; then, for a creature def that persists after death, count the body's life down and, two seconds before the end, spawn the decay effect at the body and fade it out once; when the fade has finished kill the body
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00836260, length 539 bytes, `ret`, bool in al.
// Callees: CThingGameObject::WorldUpdate @ 0x00838BA0, CThingBodyReorienter::IsUpdateNeeded @ 0x00863250 /
//   UpdateBodyReorienation @ 0x00863E10 / the one-byte flag setter @ 0x00863260, CThing::UpdateGraphicAppearance(long)
//   @ 0x004C7590, CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::GetDef<CThingCreatureDef>(long,
//   CDefPointer&) @ 0x00513160, CThingDeadCreature::PeekDefinition @ 0x00835B90, CThing::GetPosition @ 0x004C73D0,
//   CreateSpecialEffect @ 0x006E0880 (__fastcall), CThing::GetTCDriver<CTCDParticleEmitter> @ 0x004ACB50,
//   CTCDParticleEmitter::AttachToThing @ 0x006E0BE0, CCharString(const char*, long) @ 0x0099EBF0 / dtor @ 0x0099EAE0,
//   LowerBound @ 0x0040F020 (non-const) / @ 0x004365B0 (const), CThing::AddTC @ 0x004C9D60, CTCFadeOutAndIn::Fade
//   @ 0x006C3B70, CThing::Kill @ 0x004C9B80.
// Globals/strings: g_framesPerSecond @ 0x01375550, 1e-4f @ 0x0129BA3C, 1.0f @ 0x0122DED8, "" @ 0x0122D70E,
//   "CTCFadeOutAndIn" @ 0x01238FE4.
// Layout (retail; Ego_r.pdb in brackets): CThing interface bitset +0x20, map +0x44, def index +0x8c, Dead +0x91 bit 0;
//   CThingPhysical Health +0xb4 [+0xcc]; CThingDeadCreature BodyReorienter +0xe4 [+0xfc], StartedDecay +0xe8 [+0x100];
//   CThingCreatureDef NumSecondsToPersistOnDeath +0x88, DeadBodyDecayEffect +0x144; CTCFadeOutAndIn fading flags
//   +0xc / +0xd.
// Shape: the first def read is an inline PeekDefinition (the CDefPointer temporary released before the float load);
//   GetDef and GetTCDriver get same-TU noinline stand-ins that only write their out slot.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v000() { }
    virtual void DestroyDef();                          // slot 1 (+4)
};
struct CRefCounted {
    long m_refCount;
};
struct CDefPointeeBase : CDefVirtual, CRefCounted {
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> struct CDefPointer {
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject)
            m_pObject->ReleaseDef();
    }
    T& operator*() const { return *m_pObject; }
};
struct CThingCreatureDef : CDefPointeeBase {
    unsigned char _pad08[0x88 - 0x08];
    float NumSecondsToPersistOnDeath;                   // +0x88
    unsigned char _pad8c[0x144 - 0x8c];
    long DeadBodyDecayEffect;                           // +0x144
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingCreatureDef>& out);   // 0x00513160 (GetDef<CThingCreatureDef>)
};
void* FindDef_CDefinitionManager_GetDef_6260();
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CThingCreatureDef>& out)
{
    void* def = FindDef_CDefinitionManager_GetDef_6260();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CTCFadeOutAndIn {
    unsigned char _pad00[0x0c];
    bool FadingA;                                       // +0x0c
    bool FadingB;                                       // +0x0d
    void Fade(float time, bool fadeIn);                 // 0x006C3B70
};
class CThing;
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CThingBodyReorienter {
    bool IsUpdateNeeded();                              // 0x00863250
    void UpdateBodyReorienation();                      // 0x00863E10
    void SetFlag_00863260(bool value);                  // 0x00863260
};
template<class T> struct scoped_ptr {
    T* px;
    T* operator->() const { return px; }
};

extern long g_framesPerSecond;                          // 0x01375550

class CThing {
public:
    void* __vftable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short m_defGlobalIndex;                    // +0x8c
    unsigned char _pad8e[0x91 - 0x8e];
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void UpdateGraphicAppearance(long reason);          // 0x004C7590
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    CTCBase* AddTC(const CCharString& name, long a, long b);   // 0x004C9D60
    void Kill(bool immediately);                        // 0x004C9B80
    bool GetTCDriver(CTCDParticleEmitter** out);        // 0x004ACB50 (GetTCDriver<CTCDParticleEmitter>)
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline const CTCBase* GetConstTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
void* Find_CThing_GetTCDriver_6260(CThing* thing);
#pragma auto_inline(off)
__declspec(noinline) bool CThing::GetTCDriver(CTCDParticleEmitter** out)
{
    void* driver = Find_CThing_GetTCDriver_6260(this);
    if (!driver)
        return false;
    *out = (CTCDParticleEmitter*)driver;
    return true;
}
#pragma auto_inline(on)

CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

class CThingGameObject : public CThing {
public:
    unsigned char _pad94[0xb4 - 0x94];
    float Health;                                       // +0xb4
    bool WorldUpdate();                                 // 0x00838BA0
};
class CThingDeadCreature : public CThingGameObject {
public:
    unsigned char _padb8[0xe4 - 0xb8];
    scoped_ptr<CThingBodyReorienter> BodyReorienter;    // +0xe4
    bool StartedDecay;                                  // +0xe8
    const CThingCreatureDef& PeekDefinition() const;    // 0x00835B90
    const CThingCreatureDef& PeekDefinitionInline() const
    {
        CDefPointer<CThingCreatureDef> def;
        GetDefinitionManager()->GetDef(m_defGlobalIndex, def);
        return *def;
    }
    bool WorldUpdate();
};

bool CThingDeadCreature::WorldUpdate()
{
    CThingGameObject::WorldUpdate();
    if (BodyReorienter->IsUpdateNeeded()) {
        BodyReorienter->UpdateBodyReorienation();
        BodyReorienter->SetFlag_00863260(false);
    }
    if (HasTC(0x83))
        UpdateGraphicAppearance(2);
    if (PeekDefinitionInline().NumSecondsToPersistOnDeath > 0.0001f) {
        Health -= 1.0f;
        if (Health < g_framesPerSecond * 2.0f) {
            if (!StartedDecay) {
                StartedDecay = true;
                if (PeekDefinition().DeadBodyDecayEffect > 0) {
                    CThing* effect = CreateSpecialEffect(PeekDefinition().DeadBodyDecayEffect, GetPosition(), false);
                    if (effect && !effect->Dead) {
                        CTCDParticleEmitter* emitter = 0;
                        effect->GetTCDriver(&emitter);
                        emitter->AttachToThing(this, 0x30, "", 0, 0.0f);
                    }
                }
                CTCFadeOutAndIn* fade;
                if (HasTC(0xf9))
                    fade = (CTCFadeOutAndIn*)GetTC(0xf9);
                else
                    fade = (CTCFadeOutAndIn*)AddTC("CTCFadeOutAndIn", 0, 0);
                fade->Fade(2.0f, true);
            } else {
                const CTCFadeOutAndIn* fade = 0;
                if (HasTC(0xf9))
                    fade = (const CTCFadeOutAndIn*)GetConstTC(0xf9);
                if (!fade->FadingB && !fade->FadingA) {
                    Kill(true);
                    return true;
                }
            }
        }
    }
    return false;
}

CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; (void)length; }
CCharString::~CCharString() { }
static int g_destroyed = 0;
void CDefVirtual::DestroyDef() { ++g_destroyed; }
static CThingCreatureDef g_def;
void* FindDef_CDefinitionManager_GetDef_6260() { return &g_def; }
static CDefinitionManager g_mgr;
CDefinitionManager* CThing::GetDefinitionManager() const { return &g_mgr; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}
static float g_fadeTime = 0.0f; static int g_fadeIn = -1; static int g_fadeCalls = 0;
void CTCFadeOutAndIn::Fade(float time, bool fadeIn) { g_fadeTime = time; g_fadeIn = fadeIn ? 1 : 0; ++g_fadeCalls; }
static int g_attach = 0; static long g_attachType = 0; static CThing* g_attachThing = 0;
void CTCDParticleEmitter::AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset)
{
    g_attach++; g_attachType = attachType; g_attachThing = thing;
    if (std::strcmp((const char*)helperName.m_pData, "") != 0 || helperIndex != 0 || offset != 0.0f) g_attach += 100;
}
static int g_needUpdate = 0; static int g_reorient = 0; static int g_flagSet = -1;
bool CThingBodyReorienter::IsUpdateNeeded() { return g_needUpdate != 0; }
void CThingBodyReorienter::UpdateBodyReorienation() { ++g_reorient; }
void CThingBodyReorienter::SetFlag_00863260(bool value) { g_flagSet = value ? 1 : 0; }
long g_framesPerSecond = 30;
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
static int g_appearance = -1;
void CThing::UpdateGraphicAppearance(long reason) { g_appearance = (int)reason; }
static CTCFadeOutAndIn g_newFade;
static int g_addTC = 0;
CTCBase* CThing::AddTC(const CCharString& name, long a, long b)
{
    if (std::strcmp((const char*)name.m_pData, "CTCFadeOutAndIn") == 0 && a == 0 && b == 0) ++g_addTC;
    return (CTCBase*)&g_newFade;
}
static int g_kill = -1;
void CThing::Kill(bool immediately) { g_kill = immediately ? 1 : 0; }
static CTCDParticleEmitter g_particleSource;
void* Find_CThing_GetTCDriver_6260(CThing* thing) { (void)thing; return &g_particleSource; }
static CThing g_effect;
static long g_effectId = 0; static int g_effectAttached = -1;
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached)
{
    g_effectId = effect; g_effectAttached = attached ? 1 : 0;
    if (position.Y != 2.0f) return 0;
    return &g_effect;
}
static int g_baseUpdate = 0;
bool CThingGameObject::WorldUpdate() { ++g_baseUpdate; return true; }
const CThingCreatureDef& CThingDeadCreature::PeekDefinition() const { return g_def; }
static CThingBodyReorienter g_reor;
static CTCFadeOutAndIn g_oldFade;

int main()
{
    static CThingDeadCreature dc;
    std::memset(&dc, 0, sizeof(dc));
    dc.BodyReorienter.px = &g_reor;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x50; entries[0].component = 0;
    entries[1].type = 0xf9; entries[1].component = (CTCBase*)&g_oldFade;
    dc.m_interfaces.first = entries; dc.m_interfaces.last = entries + 1;   // 0xf9 not present yet
    g_def.m_refCount = 100;
    g_def.NumSecondsToPersistOnDeath = 0.0f;
    g_def.DeadBodyDecayEffect = 7;
    dc.Health = 100.0f;
    // 1: no persistence -> nothing beyond base/reorient
    g_needUpdate = 1;
    if (dc.WorldUpdate()) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (g_baseUpdate != 1 || g_reorient != 1 || g_flagSet != 0) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (dc.Health != 100.0f || g_appearance != -1) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (g_def.m_refCount != 99 || g_destroyed != 0) { std::printf("00836260_TEST FAIL\n"); return 1; }
    // 2: persists, Health 100 -> 99, not below 60
    g_needUpdate = 0; g_flagSet = -1;
    g_def.NumSecondsToPersistOnDeath = 5.0f;
    dc.m_presentBits[0x83 >> 5] |= 1UL << (0x83 & 31);
    if (dc.WorldUpdate()) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (g_reorient != 1 || g_flagSet != -1 || g_appearance != 2) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (dc.Health != 99.0f || dc.StartedDecay) { std::printf("00836260_TEST FAIL\n"); return 1; }
    // 3: Health 50 -> 49 < 60: start decay, effect attached, AddTC fade
    dc.Health = 50.0f;
    if (dc.WorldUpdate()) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (dc.Health != 49.0f || !dc.StartedDecay) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (g_effectId != 7 || g_effectAttached != 0 || g_attach != 1 || g_attachType != 0x30 || g_attachThing != &dc) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (g_addTC != 1 || g_fadeCalls != 1 || g_fadeTime != 2.0f || g_fadeIn != 1) { std::printf("00836260_TEST FAIL\n"); return 1; }
    // 4: started, fade TC present and still fading -> false
    dc.m_interfaces.last = entries + 2;
    dc.m_presentBits[0xf9 >> 5] |= 1UL << (0xf9 & 31);
    g_oldFade.FadingA = true;
    if (dc.WorldUpdate()) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (g_kill != -1 || g_fadeCalls != 1) { std::printf("00836260_TEST FAIL\n"); return 1; }
    g_oldFade.FadingA = false; g_oldFade.FadingB = true;
    if (dc.WorldUpdate()) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (g_kill != -1) { std::printf("00836260_TEST FAIL\n"); return 1; }
    // 5: fade finished -> Kill(true), true
    g_oldFade.FadingB = false;
    if (!dc.WorldUpdate()) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (g_kill != 1 || dc.Health != 46.0f) { std::printf("00836260_TEST FAIL\n"); return 1; }
    // 6: no decay effect and effect dead path
    dc.StartedDecay = false; g_def.DeadBodyDecayEffect = 0; g_attach = 0;
    if (dc.WorldUpdate()) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (g_attach != 0 || g_fadeCalls != 2 || g_addTC != 1) { std::printf("00836260_TEST FAIL\n"); return 1; }
    dc.StartedDecay = false; g_def.DeadBodyDecayEffect = 3; g_effect.Dead = 1;
    if (dc.WorldUpdate()) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (g_attach != 0 || g_effectId != 3 || g_fadeCalls != 3) { std::printf("00836260_TEST FAIL\n"); return 1; }
    if (g_def.m_refCount != 92 || g_destroyed != 0) { std::printf("00836260_TEST FAIL\n"); return 1; }
    std::printf("00836260_TEST PASS\n");
    return 0;
}
