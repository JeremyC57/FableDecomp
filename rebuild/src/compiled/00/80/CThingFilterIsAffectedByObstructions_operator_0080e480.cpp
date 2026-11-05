// CThingFilter_IsAffectedByObstructions::operator()(CThing* thing) -- false only when the thing's interface-0x31 component reports an object of type 0x5F (obstruction-immune); true otherwise (also when the component is absent)
// Leaf: ??RCThingFilter_IsAffectedByObstructions
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080E480, length 82 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, the component's object accessor @ 0x0073A6E0
// (manifest "CPixelShader::CountReferences" is a propagated label; it returns a polymorphic object), then that object's
// virtual slot 1 (`call [edx+4]`, a type id).
// Layout: interface 0x31 -> bitset word 1 (+0x24) bit 17 (`test dword ptr [eax+24h],20000h`).
// Shape: `if (GetInterface && obj->Type() == 0x5F) return false; return true;` -- both exits are byte constants.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTypedObject {
    virtual void v00();
    virtual long GetType();                         // slot 1
};

struct CTCInterface31 {                             // interface 0x31 component (class not established)
    CTypedObject* GetObject() const;                      // 0x0073A6E0
};

struct CThingFilter_IsAffectedByObstructions {
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsAffectedByObstructions::operator()(CThing* thing) const
{
    const CTCInterface31* component;
    if (thing->PeekTC(TCI_ENTITY_MODE_MANAGER, &component) && component->GetObject()->GetType() == 0x5F)
        return false;
    return true;
}