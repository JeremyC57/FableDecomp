// CCreatureAction_ThrowGameObject::PerformAction() -- take the thing in the creature's primary carry slot (interface type 0x46), release it and accelerate it towards the stored throw vector
// Leaf: ?PerformAction@CCreatureAction_ThrowGameObject@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00846090, length 107 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for
// type 0x46 = bit 6 of the bitset byte at +0x28), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCCarrying::RemoveThing(CThing*) @ 0x006A19B0,
// CTCPhysicsBase::ApplyAcceleration(const C3DVector&) @ 0x006B0210 (the thrown thing's physics at +0x60).
// Shape: the carrying component is an UNINITIALISED out-parameter used unconditionally; the two paths join with it in edi (the miss path reads the
// never-written frame word). The thrown thing rides in esi; `this` in ebx becomes &Target (`add ebx,0A8h`) for the last call.
// No engine header layout used (the PDB flattens the base); modelled locally (retail CCreatureActionBase is 0xa8 bytes, Target at +0xa8).
class CTCBase;
#include "engine/C3DVector.h"
class CTCPhysicsBase { public:
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCCarrying { public:                        // interface type 0x46 component
    void RemoveThing(CThing* thing);        // 0x006A19B0
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_ThrowGameObject : public CCreatureActionBase { public:
    C3DVector Target;                       // +0xa8
    virtual void PerformAction();
};

void CCreatureAction_ThrowGameObject::PerformAction()
{
    CTCCarrying* pCarrying;
    PCreature.GetPItem()->GetTC(TCI_CARRYING, &pCarrying);
    CThing* thrown = pCarrying->GetThingInPrimarySlot();
    if (thrown) {
        pCarrying->RemoveThing(thrown);
        thrown->GetPhysicsTC()->ApplyAcceleration(Target);
    }
}