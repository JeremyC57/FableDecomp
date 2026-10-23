#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
#include <stdio.h>
#include <string.h>
// CTCTurncoat::ModifyTurncoatedness(float) -- while below its resistance, add to the creature's turncoatedness (clamped to [0, resistance], restarting the recovery delay); on reaching the resistance play the activate effect, attach the turncoated effect, kill the stage effects and play the turncoat chant
// Leaf: ?ModifyTurncoatedness@CTCTurncoat
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C7A00, 572 bytes (+ int3 padding), `ret 4`.
// Callees: CThing::GetPosition() const 0x004C73D0; CreateParticleEffect(long, const C3DVector*) 0x0072AB70 (`__fastcall`);
//   CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / ~CBaseIntelligentPointer 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem
//   0x00A01B90; IS_THING_ALIVE<CIntelligentPointer<...> > 0x004CC340 (`__fastcall`, out of line); LowerBound 0x0040F020
//   (interfaces 0x4B particle emitter, 0x10 sound player); CTCDParticleEmitter::AttachToThing 0x006E0BE0; CThing::Kill(bool)
//   0x004C9B80; vector<CIntelligentPointer<CThing> >::erase(first, last) 0x004AC0F0; CTCSoundPlayer::PlaySound 0x0066BD00;
//   CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0.
// Layout (retail CTCTurncoat = Ego_r.pdb, the stage-effect vector 12 bytes, as the landed ctor 0x007C78C0):
//   CurrentTurncoatedness +0x10, PDef +0x14, TurncoatedEffect +0x18, TimeDelayBeforeResistanceRecoveryFrames +0x30,
//   CurrentRecoveryTimer +0x34, TurncoatStageEffects +0x3c, CreatureTurncoatResistance +0x48; CTurncoatDef TurncoatedEffect
//   +0x28, TurncoatActivateEffect +0x2c.
#include <stdlib.h>
struct C3DVector { float X, Y, Z; };
struct CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
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
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector* pos);   // 0x0072AB70

struct CBaseIntelligentPointer {                        // 8 bytes: vptr + item
    virtual void v00();
    void* m_pData;
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CThing* item);                  // 0x004A96F0
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer* p);   // 0x004CC340
inline bool IsThingAlive(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CEffectVector {                                  // vector<CIntelligentPointer<CThing> > (12 bytes)
    CIntelligentPointer* _M_start;
    CIntelligentPointer* _M_finish;
    CIntelligentPointer* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    CIntelligentPointer* erase(CIntelligentPointer* first, CIntelligentPointer* last);   // 0x004AC0F0
    void clear() { erase(_M_start, _M_finish); }
};

struct CTCDParticleEmitter {                            // interface 0x4B
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CTCSoundPlayer {                                 // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};

struct CTurncoatDef {
    unsigned char _pad0[0x28];
    long TurncoatedEffect;                              // +0x28
    long TurncoatActivateEffect;                        // +0x2c
};
struct CDefPointer {                                    // CDefPointer<const CTurncoatDef>
    CTurncoatDef* Object;
    CTurncoatDef* operator->() const { return Object; }
};

struct CTCTurncoat {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
    long WillTurncoatTimer;                             // +0x0c
    float CurrentTurncoatedness;                        // +0x10
    CDefPointer PDef;                                   // +0x14
    CIntelligentPointer TurncoatedEffect;               // +0x18
    long TurncoatTimeLast;                              // +0x20
    CIntelligentPointer TurncoatOriginator;             // +0x24
    bool TurncoatOverride;                              // +0x2c
    long TimeDelayBeforeResistanceRecoveryFrames;       // +0x30
    long CurrentRecoveryTimer;                          // +0x34
    float ResistanceRecoveryPerFrame;                   // +0x38
    CEffectVector TurncoatStageEffects;                 // +0x3c
    float CreatureTurncoatResistance;                   // +0x48
    void ModifyTurncoatedness(float amount);
};

void CTCTurncoat::ModifyTurncoatedness(float amount)
{
    if (CurrentTurncoatedness >= CreatureTurncoatResistance)
        return;
    CurrentRecoveryTimer = TimeDelayBeforeResistanceRecoveryFrames;
    CurrentTurncoatedness += amount;
    if (CurrentTurncoatedness > CreatureTurncoatResistance)
        CurrentTurncoatedness = CreatureTurncoatResistance;
    else if (CurrentTurncoatedness < 0.0f)
        CurrentTurncoatedness = 0.0f;
    if (CurrentTurncoatedness >= CreatureTurncoatResistance) {
        if (PDef->TurncoatActivateEffect)
            CreateParticleEffect(PDef->TurncoatActivateEffect, &ParentThing->GetPosition());
        if (!IsThingAlive(TurncoatedEffect)) {
            long effect = PDef->TurncoatedEffect;
            if (effect) {
                {
                    CIntelligentPointer created(CreateParticleEffect(effect, &ParentThing->GetPosition()));
                    TurncoatedEffect.SetPItem(created.GetPItem());
                }
                if (IS_THING_ALIVE(&TurncoatedEffect)) {
                    CTCDParticleEmitter* emitter;
                    TurncoatedEffect.GetPItem()->GetInterface(&emitter, 0x4b);
                    emitter->AttachToThing(ParentThing, 8, "", 0, 0.0f);
                }
            }
        }
        for (long i = 0; i < TurncoatStageEffects.size(); ++i) {
            if (IsThingAlive(TurncoatStageEffects._M_start[i])) {
                CIntelligentPointer* start = TurncoatStageEffects._M_start;
                start[i].GetPItem()->Kill(true);
            }
        }
        TurncoatStageEffects.clear();
        CTCSoundPlayer* sound;
        if (ParentThing->GetInterface(&sound, 0x10)) {
            CCharString chant("HERO_ABILITY_TURNCOAT_CHANT");
            sound->PlaySound(chant, 1, 0, 0, -1);
        }
    }
}

// ---- test stubs ----
static int g_kills, g_creates, g_attach, g_sound, g_erase;
static CThing g_things[8];
CCharString::CCharString(const char*, long) { PStringData = 0; }
CCharString::~CCharString() {}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { for (SEntry* e = first; e != last; ++e) if (e->type >= *type) return e; return last; }
const C3DVector& CThing::GetPosition() const { static C3DVector v; return v; }
void CThing::Kill(bool) { ++g_kills; m_flags |= 1; }
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector*) { ++g_creates; return &g_things[effect & 7]; }
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pData; }
void CBaseIntelligentPointer::SetPItem(CThing* t) { m_pData = t; }
CIntelligentPointer::CIntelligentPointer(CThing* t) { m_pData = t; }
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer* p) { return IsThingAlive(*p); }
CIntelligentPointer* CEffectVector::erase(CIntelligentPointer* f, CIntelligentPointer* l) { ++g_erase; _M_finish = f; return f; }
void CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) { ++g_attach; }
unsigned long CTCSoundPlayer::PlaySound(const CCharString&, long, long, long, long) { ++g_sound; return 0; }
void CTCTurncoat::v00() {}
int main()
{
    CTCTurncoat& t = *(CTCTurncoat*)calloc(1, sizeof(CTCTurncoat));
    static CTurncoatDef def; memset(&def, 0, sizeof(def));
    def.TurncoatActivateEffect = 1; def.TurncoatedEffect = 2;
    t.PDef.Object = &def;
    static CThing parent; memset(&parent, 0, sizeof(parent));
    t.ParentThing = &parent;
    t.CreatureTurncoatResistance = 10.0f; t.TimeDelayBeforeResistanceRecoveryFrames = 30;
    static CIntelligentPointer* stage = (CIntelligentPointer*)calloc(3, sizeof(CIntelligentPointer));
    memset(g_things, 0, sizeof(g_things));
    stage[0].m_pData = &g_things[4]; stage[1].m_pData = 0; stage[2].m_pData = &g_things[5]; g_things[5].m_flags = 1;
    static CInterfaceMap::SEntry ent[1]; static char emitterObj[4]; ent[0].type = 0x4b; ent[0].component = (CTCBase*)emitterObj;
    g_things[2].m_presentBits[2] = 1UL << 11; g_things[2].m_interfaces.first = ent; g_things[2].m_interfaces.last = ent + 1;
    t.TurncoatStageEffects._M_start = stage; t.TurncoatStageEffects._M_finish = stage + 3;
    int ok = 1;
    t.ModifyTurncoatedness(-5.0f);
    if (t.CurrentTurncoatedness != 0.0f || t.CurrentRecoveryTimer != 30 || g_creates) ok = 0;
    t.ModifyTurncoatedness(4.0f);
    if (t.CurrentTurncoatedness != 4.0f || g_creates) ok = 0;
    t.ModifyTurncoatedness(100.0f);
    if (t.CurrentTurncoatedness != 10.0f) ok = 0;
    if (g_creates != 2 || g_kills != 1 || !(g_things[4].m_flags & 1) || g_erase != 1 || g_sound != 0 || g_attach != 1) ok = 0;
    if (t.TurncoatedEffect.m_pData != &g_things[2]) ok = 0;
    t.ModifyTurncoatedness(1.0f);
    if (g_creates != 2) ok = 0;
    if (ok) printf("007c7a00_TEST PASS\n"); else printf("fail c%d k%d e%d s%d a%d\n", g_creates, g_kills, g_erase, g_sound, g_attach);
    return ok ? 0 : 1;
}