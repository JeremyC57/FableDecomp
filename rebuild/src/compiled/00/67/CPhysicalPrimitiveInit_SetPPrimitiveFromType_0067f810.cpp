// CPhysicalPrimitiveInit::SetPPrimitiveFromType(long) -- finds the registered prototype primitive whose type matches, stores a clone of it in PPrimitive and returns true (false when none matches)
// Leaf: SetPPrimitiveFromType
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067F810, 90 bytes (+6 int3 padding).
// Callees: prototype vtable slot 3 (`call [eax+0Ch]`, the type) and slot 1 (`call [edx+4]`, a
// clone), 0x0067FC10 CCountedPointer<CPhysicalPrimitiveBase>::Reset(CPhysicalPrimitiveBase*).
//
// PDB: CPhysicalPrimitiveInit (8 bytes) = { CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>
// PPrimitive; }. The prototypes live in a global three-pointer vector of 8-byte counted pointers
// (first at 0x013BA888, last at 0x013BA88C); the loop dereferences each element's object pointer
// once (kept in esi for both virtual calls), re-reads `last` after every virtual call (the global
// may change), and exits through two separate epilogues (`xor al,al` / `mov al,1`). The loop head is
// aligned (`lea esp,[esp]`): a speed TU.
struct CPhysicalPrimitiveBase {
    virtual void Slot00();
    virtual CPhysicalPrimitiveBase* Clone() const;    // slot 1
    virtual void Slot08();
    virtual long GetType() const;                     // slot 3
};

#include "engine/CCountedPointer.h"

struct CPrototypeVector {
    CCountedPointer<CPhysicalPrimitiveBase>* m_pFirst;
    CCountedPointer<CPhysicalPrimitiveBase>* m_pLast;
    CCountedPointer<CPhysicalPrimitiveBase>* m_pEnd;
};

extern CPrototypeVector g_PrimitivePrototypes;   // 0x013BA888

class CPhysicalPrimitiveInit { public:
    CCountedPointer<CPhysicalPrimitiveBase> PPrimitive;   // +0x0
    bool SetPPrimitiveFromType(long type);
};

bool CPhysicalPrimitiveInit::SetPPrimitiveFromType(long type)
{
    for (CCountedPointer<CPhysicalPrimitiveBase>* it = g_PrimitivePrototypes.m_pFirst;
         it != g_PrimitivePrototypes.m_pLast; ++it) {
        CPhysicalPrimitiveBase* prototype = it->Data;
        if (prototype->GetType() == type) {
            PPrimitive.Reset(prototype->Clone());
            return true;
        }
    }
    return false;
}