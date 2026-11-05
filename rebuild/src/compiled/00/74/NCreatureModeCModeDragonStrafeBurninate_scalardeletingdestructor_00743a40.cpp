// NCreatureMode::CModeDragonStrafeBurninate::`scalar deleting destructor'(unsigned int) -- destroy the member at +0x98 out of line, the CModeIntangible member at +0 destroyed out of line, then the flag-tested operator delete
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00743A40.
// Callees: the member destructor @ 0x00A01C10 (CBaseIntelligentPointer::~CBaseIntelligentPointer), the base destructor @ 0x00743000 (CModeIntangible::~CModeIntangible), operator delete @ 0x00BFE9BC. Shape: no vtable store (the class is
// non-polymorphic: the polymorphic object at +0 is a member, so VC7.1 emits the non-virtual deleting destructor); the member at +0x98 is destroyed first. The chunk label
// "CEngineSubPrimitiveText `scalar deleting destructor'" is propagated; the class is named by address.
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CModeIntangible {                                  // polymorphic base of 0x98 bytes
    virtual void v00();
    unsigned char _pad[0x94];
    ~CModeIntangible();                                   // 0x00743000
};

struct CModeIntangiblePointer_00743a40 {                                   // non-polymorphic: the polymorphic CModeIntangible is its FIRST MEMBER, not a base (no vtable store, members destroyed in reverse order)
    CModeIntangible m_base;                                 // +0x00
    CBaseIntelligentPointer m_member;                            // +0x98
    ~CModeIntangiblePointer_00743a40() { }
};

// The delete-expression that makes the compiler emit the (non-virtual) scalar deleting destructor.
void Destroy_00743a40(CModeIntangiblePointer_00743a40* object)
{
    delete object;
}