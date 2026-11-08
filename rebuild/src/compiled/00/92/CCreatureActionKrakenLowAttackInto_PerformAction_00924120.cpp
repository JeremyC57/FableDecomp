// CCreatureAction_KrakenLowAttackInto::PerformAction() -- start the kraken's low attack beam and switch on its head tracking
// Leaf: ?PerformAction@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924120, length 91 bytes (+ int3 padding), vtable slot 3 of 0x01290B0C
// (RTTI `?A0x29922355::CCreatureAction_KrakenLowAttackInto`, anonymous namespace of the kraken TU; FableWin.pdb names
// ?PerformAction@CCreatureAction_KrakenLowAttackInto@?A0xe2035276@@UAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined
// GetInterface for type 0xe2 = CTCKraken, bit 2 of the bitset byte at +0x3c), CTCKraken::StartLowAttackBeam @ 0x007FC7A0
// (manifest says StartHighAttackBeam: that is 0x007FC7D0; this one scales CKrakenDef::DelayBeforeLowBeam +0x4c and sets mode 4).
// Shape: the UNCHECKED out-parameter lookup in an inline accessor (the absent arm reads the uninitialised out pointer = the
// shared key word); the component stays in esi for both the call and the HeadTrackTarget store, so both arms duplicate the tail.
// Layout: CTCKraken retail HeadTrackTarget +0x51 (Ego_r.pdb +0x61: the two std::list members are 4 bytes smaller each in retail).
#include "fable_compiler.h"
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCKraken { public:                              // interface type 0xe2 component
    unsigned char _pad00[0x51];
    bool          HeadTrackTarget;              // +0x51 (PDB +0x61)
    void StartLowAttackBeam();                  // 0x007FC7A0
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void PerformAction();               // slot 3 (+0x0c)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    FABLE_FORCEINLINE CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.Get()->GetTC(TCI_KRAKEN, &kraken);
        return kraken;
    }
};
struct CCreatureAction_KrakenLowAttackInto : CCombatActionBase {   // vtable 0x01290B0C, sizeof 0x114
    virtual void PerformAction();
};

void CCreatureAction_KrakenLowAttackInto::PerformAction()
{
    CTCKraken* kraken = GetKraken();
    kraken->StartLowAttackBeam();
    kraken->HeadTrackTarget = true;
}