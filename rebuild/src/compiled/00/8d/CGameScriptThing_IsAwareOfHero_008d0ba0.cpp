// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
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
class CThing;
class CBaseObject;
class CTCBase;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* PData;
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }           // CThing's CBaseObject base is at offset 0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                            // +0x10 (descriptive)
    unsigned char _pad11[0x0f];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                             // +0x91 bit 0
};
class CTCPerceiveThing { public:
    unsigned char _base00[0x0c];
    CIntelligentPointer<const CThing> PerceivedThing;   // +0x0c
    float DistanceFromPerceivedThingSquared;            // +0x14
    float AwarenessTimer;                               // +0x18
    const CThing* GetPerceivedThing() const;                  // 0x007446A0 Ego_r/FableWin ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ (agentNF16)
public:
    bool IsPlayerExtendedLineOfSight() const;   // 0x00744830
};
class CGameScriptThing { public:
    unsigned char _base00[0x0c];
    CIntelligentPointer<CThing> PThing;                 // +0x0c
    bool IsAwareOfHero();
};
bool CGameScriptThing::IsAwareOfHero()
{
    CIntelligentPointer<CThing>* p = &PThing;
    CThing* thing;
    if (p->Get() && !p->Get()->Dead && (thing = p->Get())->UsingComponents[0x34 >> 5] & (1UL << (0x34 & 31))) {
        ETCInterfaceType kind = (ETCInterfaceType)(0x34);
        const CInterfaceMap* map = &thing->Interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(kind);
        if (it == map->end() || it->first > 0x34) it = map->end();
        CTCPerceiveThing* perceive = (CTCPerceiveThing*)it->second;
        if (perceive->IsPlayerExtendedLineOfSight()) return true;
        if (perceive->AwarenessTimer == 0.0f) return false;
        const CThing* target = perceive->GetPerceivedThing();
        if (target && !target->Dead && perceive->GetPerceivedThing()->ThingType == 2) return true;
    }
    return false;
}