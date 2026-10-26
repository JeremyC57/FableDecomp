// Behaviour test for 006e0be0: src/006e0be0.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCDParticleEmitter::AttachToThing(CThing& thing, long flags, const CCharString& posName, long posIndex, float offset)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCDParticleEmitter::AttachToThing(CThing& thing, long flags, const CCharString& posName, long posIndex, float offset) -- attach the emitter to a thing: clear the old attachment, record the flags/bone name/index/offset and the thing pointer, drop the emitter's thing from the thing's dependents, place the emitter, play the thing's "PARTICLE_<EMITTER NAME>" sound criteria when its entity sound def names one, and keep the emitter always updating when the thing ignores bullet time
// Leaf: ?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0BE0, 449 bytes, `ret 14h`.
// Callees: CTCDParticleEmitter::ClearAttachments 0x006E0AF0, CCharString::operator=(const CCharString&) 0x0099EFB0,
//   CBaseIntelligentPointer() 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ~CBaseIntelligentPointer 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020,
//   CTCDependentThings::RemoveDependentThing(CThing*) 0x0071A940 (interface 0xd8; descriptive, AddDependentThing is
//   0x0071A8F0), CTCDParticleEmitter::UpdateEmitterThingAttachment(bool, float) 0x006DFFB0, CDefClassBase::GetSubDef
//   0x0065F604 (the CEntitySoundDef instantiation, as the landed 0x0066B1D0), CCharString::operator!=(const char*)
//   0x0099E960 ("" 0x0122D70E), CCharString::ToUpper 0x0099EE20, operator+(const char*, const CCharString&) 0x0099F690
//   (__fastcall, "PARTICLE_" 0x0125FFA4), ~CCharString 0x0099EAE0, CTCSound::PlaySound 0x0066BD00 (interface 0x10),
//   CThing::IsAffectedByBulletTime 0x004C9920.
// Layout: Ego_r.pdb CTCDParticleEmitter (PEmitter +0x14, PAttachedToThing +0x1c, AttachmentFlags +0x24,
//   AttachmentPosName +0x28, AttachmentPosIndex +0x2c, FloatOffset +0x30, AttachedToThing/AlwaysUpdate +0x45 bits 0/1);
//   CParticleEmitter (Ego_r - 0xc): AlwaysUpdate +0x44 bit 4, Name +0x140; CEntitySoundDef SoundIdentifier +0x48.
// Shape: the attachment is assigned through a CIntelligentPointer<CThing> temporary and the inline operator=; the
//   dependents lookup is the if-form GetInterface (as the landed ClearAttachments 0x006E0AF0) and the sound lookup the
//   one-ternary form, both over a same-TU LowerBound body; GetSubDef has a same-TU out-only stand-in (the def is cached in
//   ebp and reloaded from its slot); the emitter name is reached through a named emitter pointer; SetAlwaysUpdate(true)
//   is inline with one GetPItem.
class CTCBase;
class CThing;

class CCharString { public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    bool operator!=(const char* text) const;                // 0x0099E960
    CCharString ToUpper() const;                            // 0x0099EE20
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
class CEntitySoundDef : public CDefBase { public:
    unsigned char _pad08[0x48 - 0x08];
    CCharString SoundIdentifier;                            // +0x48
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CEntitySoundDef>* out);      // 0x0065F604
};
CEntitySoundDef* FindRef_CDefClassBase_GetSubDef(CDefClassBase* defClass);   // stand-in callee of the stand-in body below
// Stand-in body (the real 0x0065F604 is an out-of-line template instantiation): it only writes the def through the out
// parameter, so VC7.1 knows the def pointer's address does not escape the call.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CEntitySoundDef>* out)
{
    CEntitySoundDef* def = FindRef_CDefClassBase_GetSubDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation: binary search for the first
// entry whose type is not below the key.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                                    // +0x70
    bool IsAffectedByBulletTime() const;                    // 0x004C9920
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    // The same lookup with the one-ternary entry pick (retail loads the component straight into ecx at the sound call).
    template <class T> __forceinline bool GetTC(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCDependentThings {                                 // interface 0xd8
    void RemoveDependentThing(CThing* thing);               // 0x0071A940
};
struct CTCSound {                                           // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                          // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    void* GetPItem() const;                                 // 0x00A01B50
    void SetPItem(void* item);                              // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThing> vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

namespace NParticleEngine {
class CParticleEmitter { public:
    unsigned char _pad00[0x44];
    bool PreWaterEffect : 1;                                // +0x44 bit 0
    bool WaterEffect : 1;
    bool ZBufferWriteable : 1;
    bool Visible : 1;
    bool AlwaysUpdate : 1;                                  // +0x44 bit 4
    bool DieIfOffsceen : 1;
    bool OffscreenUpdate : 1;
    bool ClipEffectToWeatherMask : 1;
    unsigned char _pad45[0x140 - 0x45];
    CCharString Name;                                       // +0x140
};
}

class CTCDParticleEmitter { public:
    void*                                         __vftable;               // +0x00
    CThing*                                       ParentThing;             // +0x04 (CTCBase)
    unsigned char                                 _pad08[0x14 - 0x08];
    CIntelligentPointer<NParticleEngine::CParticleEmitter> PEmitter;      // +0x14
    CIntelligentPointer<CThing>                   PAttachedToThing;        // +0x1c
    unsigned long                                 AttachmentFlags;         // +0x24
    CCharString                                   AttachmentPosName;       // +0x28
    long                                          AttachmentPosIndex;      // +0x2c
    float                                         FloatOffset;             // +0x30
    unsigned char                                 _pad34[0x44 - 0x34];
    bool WaitingForParticleToLoad : 1;                      // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    bool AttachedToThing : 1;                               // +0x45 bit 0
    bool AlwaysUpdate : 1;                                  // +0x45 bit 1
    bool Asleep : 1;
    void ClearAttachments();                                // 0x006E0AF0
    void UpdateEmitterThingAttachment(bool force, float blend);   // 0x006DFFB0
    void SetAlwaysUpdate(bool update)
    {
        AlwaysUpdate = update;
        NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
        if (emitter)
            emitter->AlwaysUpdate = update;
    }
    void AttachToThing(CThing& thing, long flags, const CCharString& posName, long posIndex, float offset);
};

void CTCDParticleEmitter::AttachToThing(CThing& thing, long flags, const CCharString& posName, long posIndex, float offset)
{
    ClearAttachments();
    AttachedToThing = true;
    AttachmentFlags = flags;
    AttachmentPosName = posName;
    AttachmentPosIndex = posIndex;
    PAttachedToThing = CIntelligentPointer<CThing>(&thing);
    FloatOffset = offset;
    CTCDependentThings* dependents;
    if (thing.GetInterface(&dependents, 0xd8))
        dependents->RemoveDependentThing(ParentThing);
    UpdateEmitterThingAttachment(true, 1.0f);
    CDefPointer<CEntitySoundDef> soundDef;
    if (thing.PDef->GetSubDef(&soundDef) && soundDef->SoundIdentifier != "") {
        if (PEmitter.GetPItem()) {
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            CCharString criteria = "PARTICLE_" + emitter->Name.ToUpper();
            CTCSound* sound;
            if (thing.GetTC(&sound, 0x10))
                sound->PlaySound(criteria, 1, 0, 0, -1);
        }
    }
    if (!thing.IsAffectedByBulletTime())
        SetAlwaysUpdate(true);
}

// ---- stubs ----
static int g_live = 0;               // live CCharString buffers
static char g_log[512];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
// String buffers are reference counted ([long refs][chars]): VC7.1 has no NRVO, so a stub returning a local by value
// takes a compensating reference before the local's destructor runs.
static void SetStr(CCharString& s, const char* t) { char* b = (char*)malloc(sizeof(long) + strlen(t) + 1); *(long*)b = 1; strcpy(b + sizeof(long), t); s.PStringData = b; ++g_live; }
static const char* Str(const CCharString& s) { return s.PStringData ? (const char*)s.PStringData + sizeof(long) : ""; }
static void Release(CCharString& s) { if (s.PStringData && --*(long*)s.PStringData == 0) { free(s.PStringData); --g_live; } s.PStringData = 0; }
CCharString::~CCharString() { Release(*this); }
CCharString& CCharString::operator=(const CCharString& o) { Log("assignName"); char t[128]; strcpy(t, Str(o)); Release(*this); SetStr(*this, t); return *this; }
bool CCharString::operator!=(const char* t) const { return strcmp(Str(*this), t) != 0; }
CCharString CCharString::ToUpper() const
{ CCharString r; SetStr(r, Str(*this)); for (char* q = (char*)r.PStringData + sizeof(long); *q; ++q) *q = (char)toupper(*q); ++*(long*)r.PStringData; return r; }
CCharString __fastcall operator+(const char* left, const CCharString& right)
{ char t[256]; strcpy(t, left); strcat(t, Str(right)); CCharString r; SetStr(r, t); ++*(long*)r.PStringData; return r; }
void CDefBase::v00() {}
static int g_destroyed = 0;
void CDefBase::Destroy() { ++g_destroyed; }
static CEntitySoundDef* g_subDef = 0;
static CDefClassBase* g_expectDefClass = 0;
CEntitySoundDef* FindRef_CDefClassBase_GetSubDef(CDefClassBase* defClass)
{ Log("GetSubDef"); CHECK(defClass == g_expectDefClass); if (g_subDef) ++g_subDef->RefCount; return g_subDef; }
static bool g_bulletTime = false;
bool CThing::IsAffectedByBulletTime() const { Log("BulletTime"); return g_bulletTime; }
static CThing* g_removed = 0; static CTCDependentThings* g_removedFrom = 0;
void CTCDependentThings::RemoveDependentThing(CThing* t) { Log("RemoveDependent"); g_removed = t; g_removedFrom = this; }
static char g_sound[64]; static CTCSound* g_soundOn = 0; static long g_sargs[4];
unsigned long CTCSound::PlaySound(const CCharString& n, long a, long b, long c, long d)
{ Log("PlaySound"); strcpy(g_sound, Str(n)); g_soundOn = this; g_sargs[0] = a; g_sargs[1] = b; g_sargs[2] = c; g_sargs[3] = d; return 0; }
void CBaseIntelligentPointer::v00() {}
template <class T> void CIntelligentPointer<T>::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() { m_pItem = 0; }
void* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(void* item) { m_pItem = item; }
static int g_cleared = 0;
void CTCDParticleEmitter::ClearAttachments() { Log("Clear"); ++g_cleared; }
static bool g_uForce = false; static float g_uBlend = 0;
void CTCDParticleEmitter::UpdateEmitterThingAttachment(bool force, float blend) { Log("Update"); g_uForce = force; g_uBlend = blend; }

static unsigned char g_emBuf[sizeof(CTCDParticleEmitter)];
static unsigned char g_thBuf[sizeof(CThing)];
static unsigned char g_peBuf[sizeof(NParticleEngine::CParticleEmitter)];
static CInterfaceMap::SEntry g_entries[3];
static CTCDependentThings g_deps; static CTCSound g_snd;
static CDefClassBase g_defClass;
static CThing* g_owner = (CThing*)0x1234;

static CTCDParticleEmitter* MakeEmitter(bool withParticle, const char* name)
{
    memset(g_emBuf, 0, sizeof g_emBuf);
    CTCDParticleEmitter* e = (CTCDParticleEmitter*)g_emBuf;
    e->ParentThing = g_owner;
    SetStr(e->AttachmentPosName, "old");
    memset(g_peBuf, 0, sizeof g_peBuf);
    NParticleEngine::CParticleEmitter* pe = (NParticleEngine::CParticleEmitter*)g_peBuf;
    SetStr(pe->Name, name);
    if (withParticle) e->PEmitter.SetPItem(pe);
    return e;
}
static CThing* MakeThing(bool deps, bool snd)
{
    memset(g_thBuf, 0, sizeof g_thBuf);
    CThing* t = (CThing*)g_thBuf;
    int n = 0;
    g_entries[0].type = 0x05; g_entries[0].component = 0; ++n;
    if (snd) { g_entries[n].type = 0x10; g_entries[n].component = (CTCBase*)&g_snd; ++n; t->m_presentBits[0] |= 1UL << 16; }
    if (deps) { g_entries[n].type = 0xd8; g_entries[n].component = (CTCBase*)&g_deps; ++n; t->m_presentBits[6] |= 1UL << 24; }
    t->m_interfaces.first = g_entries; t->m_interfaces.last = g_entries + n;
    t->PDef = &g_defClass; g_expectDefClass = &g_defClass;
    return t;
}
static void Reset() { g_log[0] = 0; g_step = 0; g_removed = 0; g_removedFrom = 0; g_sound[0] = 0; g_soundOn = 0; g_destroyed = 0; g_cleared = 0; g_uForce = false; g_uBlend = 0; }
static void Free(CTCDParticleEmitter* e)
{
    e->AttachmentPosName.~CCharString();
    ((NParticleEngine::CParticleEmitter*)g_peBuf)->Name.~CCharString();
}

int main()
{
    CEntitySoundDef* def = new CEntitySoundDef;
    def->RefCount = 1; SetStr(def->SoundIdentifier, "creature");
    CCharString bone; SetStr(bone, "hand_r");
    // 1: everything present: dependents + sound interfaces, a named sound def, a live particle, not in bullet time
    {
        Reset(); g_subDef = def; g_bulletTime = false;
        CTCDParticleEmitter* e = MakeEmitter(true, "Spark_big");
        CThing* t = MakeThing(true, true);
        e->AttachToThing(*t, 7, bone, 3, 2.5f);
        CHECK(strcmp(g_log, "1:Clear;2:assignName;3:RemoveDependent;4:Update;5:GetSubDef;6:PlaySound;7:BulletTime;") == 0);
        CHECK(e->AttachedToThing && e->AttachmentFlags == 7 && strcmp(Str(e->AttachmentPosName), "hand_r") == 0);
        CHECK(e->AttachmentPosIndex == 3 && e->FloatOffset == 2.5f && e->PAttachedToThing.GetPItem() == t);
        CHECK(g_removed == g_owner && g_removedFrom == &g_deps);
        CHECK(g_uForce && g_uBlend == 1.0f);
        CHECK(strcmp(g_sound, "PARTICLE_SPARK_BIG") == 0 && g_soundOn == &g_snd);
        CHECK(g_sargs[0] == 1 && g_sargs[1] == 0 && g_sargs[2] == 0 && g_sargs[3] == -1);
        CHECK(e->AlwaysUpdate && ((NParticleEngine::CParticleEmitter*)g_peBuf)->AlwaysUpdate);
        CHECK(def->RefCount == 1 && g_destroyed == 0);
        Free(e);
    }
    // 2: no interfaces, no sound def, in bullet time: no removal, no sound, no always-update
    {
        Reset(); g_subDef = 0; g_bulletTime = true;
        CTCDParticleEmitter* e = MakeEmitter(true, "x");
        CThing* t = MakeThing(false, false);
        e->AttachToThing(*t, 1, bone, -1, 0.0f);
        CHECK(strcmp(g_log, "1:Clear;2:assignName;3:Update;4:GetSubDef;5:BulletTime;") == 0);
        CHECK(!e->AlwaysUpdate && !((NParticleEngine::CParticleEmitter*)g_peBuf)->AlwaysUpdate);
        CHECK(e->PAttachedToThing.GetPItem() == t && e->AttachmentPosIndex == -1);
        Free(e);
    }
    // 3: empty sound identifier: def released (destroyed at the last reference), no sound
    {
        Reset(); CEntitySoundDef* d2 = new CEntitySoundDef; d2->RefCount = 0; SetStr(d2->SoundIdentifier, "");
        g_subDef = d2; g_bulletTime = false;
        CTCDParticleEmitter* e = MakeEmitter(true, "y");
        CThing* t = MakeThing(true, true);
        e->AttachToThing(*t, 2, bone, 0, 1.0f);
        CHECK(strcmp(g_log, "1:Clear;2:assignName;3:RemoveDependent;4:Update;5:GetSubDef;6:BulletTime;") == 0);
        CHECK(g_sound[0] == 0 && g_destroyed == 1 && d2->RefCount == 0);
        d2->SoundIdentifier.~CCharString();
        Free(e);
    }
    // 4: sound def but no particle: no sound; only the component's own always-update bit; sound interface absent too
    {
        Reset(); g_subDef = def; g_bulletTime = false;
        CTCDParticleEmitter* e = MakeEmitter(false, "z");
        CThing* t = MakeThing(false, true);
        e->AttachToThing(*t, 4, bone, 1, 1.0f);
        CHECK(strcmp(g_log, "1:Clear;2:assignName;3:Update;4:GetSubDef;5:BulletTime;") == 0);
        CHECK(e->AlwaysUpdate && !((NParticleEngine::CParticleEmitter*)g_peBuf)->AlwaysUpdate);
        CHECK(def->RefCount == 1);
        Free(e);
    }
    // 5: named def and a particle but no sound interface: name built and freed, nothing played
    {
        Reset(); g_subDef = def; g_bulletTime = true;
        CTCDParticleEmitter* e = MakeEmitter(true, "w");
        CThing* t = MakeThing(true, false);
        e->AttachToThing(*t, 4, bone, 1, 1.0f);
        CHECK(strcmp(g_log, "1:Clear;2:assignName;3:RemoveDependent;4:Update;5:GetSubDef;6:BulletTime;") == 0);
        CHECK(g_sound[0] == 0);
        Free(e);
    }
    bone.~CCharString(); bone.PStringData = 0;
    def->SoundIdentifier.~CCharString();
    CHECK(g_live == 0);
    if (fails == 0) printf("006e0be0_TEST PASS\n");
    return fails;
}