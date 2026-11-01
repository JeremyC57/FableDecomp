// CTCDegradableBase::~CTCDegradableBase() -- compiler-generated destructor: release the PDefinition def pointer (count at +4, virtual Delete at zero, cleared), then ~CTCBase by tail jump
// Leaf: ??1CTCDegradableBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007710A0, 35 bytes: `push esi ; mov esi,ecx ; mov ecx,[esi+10h] ; test ecx,ecx ; je T ;
//   dec dword ptr [ecx+4] ; jne N ; mov eax,[ecx] ; call [eax+4] ; N: mov dword ptr [esi+10h],0 ; T: mov ecx,esi ; pop esi ;
//   jmp 0x00686830`.
// Identity (proven): its two callers are the scalar deleting destructors of CTCDegradableBarrier (0x00771120, vtable 0x0126A6A4
//   slot 0, call at 0x00771123) and of CTCDegradableBase itself (0x008ECE80, vtable 0x01287F2C slot 0, call at 0x008ECE83);
//   the base's own `??_G` can only call the base's destructor, so this is ~CTCDegradableBase, reached from the barrier through
//   its (empty, inlined) implicit destructor. RTTI: CTCDegradableBarrier : CTCDegradableBase : CTCBase; Ego_r.pdb gives
//   CTCDegradableBarrier no members and CTCDegradableBase LastWorldFrameHit +0x0C, PDefinition (CDefPointer<const
//   CDegradableDef>) +0x10, PCurrentDegradableInfo +0x14, PParentThingPhysical +0x18 (no containers: retail offsets equal the
//   PDB's; the landed constructor 0x008EC850 zeroes +0x10). Neither PDB has ??1CTCDegradableBase (compiler-generated: no
//   vtable store). The catalog label `~C3DAnimationBlendState` is propagated.
// Callees: the def's virtual Delete (CIVCountedPointeeBase slot 1, `call [eax+4]`), CTCBase::~CTCBase 0x00686830.
// Shape: the PDefinition member's inline CDefPointer<T>::~CDefPointer -> Release() from engine/CDefPointer.h (null test,
//   `--IVRefCount`, Delete at zero, Object = 0).
// Emission: as in retail's CTCDegradableBarrier TU (0x007710A0 sits before the barrier constructor 0x007710D0 and its deleting
//   destructor 0x00771120): the barrier constructor emits the vftable, whose deleting destructor calls this one.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                 // CDefPointer<T> over CDefPointeeBase : CResource : CIVCountedPointeeBase
#include "engine/CBaseClassNonCopyable.h"       // CBaseClass (polymorphic root) -> CBaseClassNonCopyable

class CThing;
class CThingPhysical;
class CDegradableDef;
class CDegradableInfo;

class CTCBase : public CBaseClassNonCopyable {              // retail 0x0C bytes
public:
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCDegradableBase : public CTCBase {                  // vtable 0x01287F2C
protected:
    CTCDegradableBase(CThing& thing);                       // 0x008EC850
public:
    long                              LastWorldFrameHit;      // +0x0C
    CDefPointer<const CDegradableDef> PDefinition;            // +0x10
    CDegradableInfo*                  PCurrentDegradableInfo; // +0x14
    CThingPhysical*                   PParentThingPhysical;   // +0x18
};

class CTCDegradableBarrier : public CTCDegradableBase {     // vtable 0x0126A6A4
public:
    CTCDegradableBarrier(CThing& thing);                    // 0x007710D0
};

CTCDegradableBarrier::CTCDegradableBarrier(CThing& thing)
    : CTCDegradableBase(thing)
{
}