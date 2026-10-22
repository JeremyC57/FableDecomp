// Behaviour test for retail 0x008d0ba0: the candidate source below is embedded verbatim (from src/008d0ba0.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CGameScriptThing::IsAwareOfHero() -- true when the live thing's perception component (interface 0x34) has the player in
//   extended line of sight, or is aware of (timer non-zero) a live perceived thing of type 2
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D0BA0, 176 bytes.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0
//   (this TU's copy of the interface-map search; 0x0040F020 is the other one), CTCPerceiveThing::IsPlayerExtendedLineOfSight
//   0x00744830 (manifest), CTCPerceiveThing::GetPerceivedThing() const 0x007446A0 (`add ecx,0Ch ; jmp GetPItem`: returns
//   PerceivedThing.GetPItem(); the manifest label is a propagated _Cons_val), the float 0.0 at 0x0122DEDC.
// Layout: PDB (Ego_r) names: CGameScriptThing PThing +0x0c; CTCPerceiveThing PerceivedThing +0x0c, AwarenessTimer +0x18;
//   CThing UsingComponents bitset +0x20, interface map +0x44 (as the landed GetInterface users) and Dead = bit 0 of
//   retail +0x91 (PDB +0xa1, TLC CThing 0x10 shorter); the type byte at +0x10 is descriptive (no PDB name).
//   Local model: no engine/ header exists for CGameScriptThing/CTCPerceiveThing/CThing.
struct CThing;
struct CBaseObject;
struct CTCBase;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* PData;
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* Get() const { return (T*)GetPItem(); }           // CThing's CBaseObject base is at offset 0
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType,CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long*);                    // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char ThingType;                            // +0x10 (descriptive)
    unsigned char _pad11[0x0f];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad4c[0x45];
    unsigned char Dead : 1;                             // +0x91 bit 0
};
struct CTCPerceiveThing {
    unsigned char _base00[0x0c];
    CIntelligentPointer<const CThing> PerceivedThing;   // +0x0c
    float DistanceFromPerceivedThingSquared;            // +0x14
    float AwarenessTimer;                               // +0x18
    CThing* GetPerceivedThing() const;                  // 0x007446A0
    bool IsPlayerExtendedLineOfSight();                 // 0x00744830
};
struct CGameScriptThing {
    unsigned char _base00[0x0c];
    CIntelligentPointer<CThing> PThing;                 // +0x0c
    bool IsAwareOfHero();
};
bool CGameScriptThing::IsAwareOfHero()
{
    CIntelligentPointer<CThing>* p = &PThing;
    CThing* thing;
    if (p->Get() && !p->Get()->Dead && (thing = p->Get())->UsingComponents[0x34 >> 5] & (1UL << (0x34 & 31))) {
        long kind = 0x34;
        CInterfaceMap* map = &thing->Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&kind);
        if (it == map->last || it->type > 0x34) it = map->last;
        CTCPerceiveThing* perceive = (CTCPerceiveThing*)it->component;
        if (perceive->IsPlayerExtendedLineOfSight()) return true;
        if (perceive->AwarenessTimer == 0.0f) return false;
        CThing* target = perceive->GetPerceivedThing();
        if (target && !target->Dead && perceive->GetPerceivedThing()->ThingType == 2) return true;
    }
    return false;
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
static int g_los;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* k) { SEntry* e = first; while (e != last && e->type < *k) ++e; return e; }
bool CTCPerceiveThing::IsPlayerExtendedLineOfSight() { return g_los != 0; }
CThing* CTCPerceiveThing::GetPerceivedThing() const { return (CThing*)PerceivedThing.PData; }
static CThing g_thing, g_hero; static CTCPerceiveThing g_perc; static CInterfaceMap::SEntry g_e[2];
static int run(int los, float timer, CThing* perceived, unsigned char type, int heroDead, int thingDead, int hasIface) {
    memset(&g_thing, 0, sizeof g_thing); memset(&g_hero, 0, sizeof g_hero); memset(&g_perc, 0, sizeof g_perc);
    g_e[0].type = 0x34; g_e[0].component = (CTCBase*)&g_perc; g_e[1].type = 0x40; g_e[1].component = 0;
    g_thing.Interfaces.first = g_e; g_thing.Interfaces.last = g_e + 2;
    if (hasIface) g_thing.UsingComponents[1] |= 0x100000;
    g_thing.Dead = thingDead; g_hero.Dead = heroDead; g_hero.ThingType = type;
    g_los = los; g_perc.AwarenessTimer = timer; g_perc.PerceivedThing.PData = perceived;
    CGameScriptThing s; memset(&s, 0, sizeof s); s.PThing.PData = &g_thing;
    return s.IsAwareOfHero() ? 1 : 0;
}
int main() {
    int ok = offsetof(CThing, Interfaces) == 0x44 && offsetof(CTCPerceiveThing, AwarenessTimer) == 0x18 && offsetof(CGameScriptThing, PThing) == 0x0c;
    ok = ok && run(1, 0.0f, 0, 0, 0, 0, 1) == 1;              /* extended line of sight */
    ok = ok && run(0, 0.0f, &g_hero, 2, 0, 0, 1) == 0;        /* not aware */
    ok = ok && run(0, 1.5f, &g_hero, 2, 0, 0, 1) == 1;        /* aware of a live type-2 thing */
    ok = ok && run(0, 1.5f, &g_hero, 3, 0, 0, 1) == 0;        /* wrong type */
    ok = ok && run(0, 1.5f, &g_hero, 2, 1, 0, 1) == 0;        /* perceived thing dead */
    ok = ok && run(0, 1.5f, 0, 2, 0, 0, 1) == 0;              /* nothing perceived */
    ok = ok && run(1, 1.5f, &g_hero, 2, 0, 1, 1) == 0;        /* own thing dead */
    ok = ok && run(1, 1.5f, &g_hero, 2, 0, 0, 0) == 0;        /* no perception component */
    CGameScriptThing s; memset(&s, 0, sizeof s); ok = ok && !s.IsAwareOfHero();   /* no thing */
    printf(ok ? "008d0ba0_TEST PASS\n" : "008d0ba0_TEST FAIL\n"); return ok ? 0 : 1;
}