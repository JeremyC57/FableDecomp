// CThingBuilding::Alloc() -- the class-registry factory for CThingBuilding: allocate and construct a building (thing type 3): the physical/game-object base chain, ForceOpenCount 0 and OverrideAutomaticLocking cleared; null when the thing manager refuses the allocation
// Leaf: ?Alloc@CThingBuilding
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052AC10, 86 bytes, `ret` (Ego_r.pdb `?Alloc@CThingBuilding@@SIPAVCClass@@XZ`: static __fastcall,
//   no arguments, returns CClass*).
// Callees: CThing::operator new(unsigned int, bool) 0x004C7380 (landed; size 0xd8, flag false), CThingPhysical::
//   CThingPhysical(long type) 0x005296B0 (type 3), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10.
// Vtables: CThingGameObject 0x01244F7C, CIntelligentPointer<CThing> 0x01238C6C, CThingBuilding 0x0124509C (rtti-vtables.tsv).
// Callers: a call at 0x0082D92B (the CThingBuilding TU) and the class-registry table entry referenced at 0x00522C8C
//   (CThingManager TU).
// Layout: Ego_r.pdb CThingBuilding (0xf0) has ForceOpenCount +0xe8 and the OverrideAutomaticLocking bit +0xec; retail
//   is 0x18 bytes narrower (CThingGameObject 0xd0, see the landed CThingDeadCreature ctor 0x00835C20): +0xd0 / +0xd4,
//   0xd8 bytes (`mov ecx,0D8h`). engine/CThingGameObject.h is the PDB layout (PThingStandingOn +0xd8), so a local
//   retail model is used.
// Shape: `new (false) T` through CThing's protected static __fastcall operator new(size, bool) (no matching placement
//   delete, so no EH cleanup frame); the CThingGameObject ctor is inline (vtable store after the base call, then the
//   intelligent pointer's base ctor and vtable); the bitfield clear in the ctor body is scheduled before the derived
//   vtable store, ForceOpenCount's init-list zero after it.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_OPERATORNEW
#include "engine/CThing.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThingPhysical : public CThing {
public:
    unsigned char _pad04[0x24];   // own members from +0x98
    CThingPhysical(long type);                  // 0x005296B0
};

class CThingGameObject : public CThingPhysical {           // vtable 0x01244F7C
public:
    CIntelligentPointer<CThing> PThingStandingOn;             // +0xc0
    unsigned char m_gameObjectFlags;            // +0xc8 (bitfield group, not initialised here)
    unsigned char _padc9[7];                    // +0xc9 tail padding (the class is 8-aligned through its 64-bit UID)
    virtual void v00();
    CThingGameObject(long type) : CThingPhysical(type) { }
};

class CThingBuilding : public CThingGameObject {          // vtable 0x0124509C
public:
    long ForceOpenCount;                        // +0xd0
    bool OverrideAutomaticLocking : 1;          // +0xd4
    virtual void v00();
    CThingBuilding() : CThingGameObject(3), ForceOpenCount(0) { OverrideAutomaticLocking = false; }
    static CClass* __fastcall Alloc();
};

CClass* __fastcall CThingBuilding::Alloc()
{
    return new (false) CThingBuilding;
}