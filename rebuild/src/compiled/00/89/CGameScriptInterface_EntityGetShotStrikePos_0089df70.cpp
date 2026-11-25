// CGameScriptInterface::EntityGetShotStrikePos(const CScriptThing& entity, C3DVector& position) const -- where the last projectile struck the entity
// Retail 0x0089DF70, 128 bytes (Ego_r.pdb ?EntityGetShotStrikePos@CGameScriptInterface@@UBE_NABVCScriptThing@@AAVC3DVector@@@Z).
// Body: the entity's thing (CScriptThing slot 11) must exist, be alive (+0x91 bit 0 clear) and carry the
// entity-events component (interface 0x42: `test byte [eax+28h],4` on the presence bitset); the component is
// looked up in the thing's interface map (+0x44, CVectorMap LowerBound 0x0040F020, one-ternary end/hit pick) and
// asked for its last CEventShotStruck (CTCEntityEvents::GetEvent<NEntityEvents::CEventShotStruck,
// NEntityEvents::CEventFilter_None> 0x008AC7A0, event type 0x27); on success the event's StrikePos (+0x08,
// Ego_r.pdb CEventShotStruck) is copied out and the result is true.
// Shape: the component comes out of an inline out-parameter lookup (GetTCOut), which leaves it in eax until the
// pushes (`mov eax,[eax+4] ... mov ecx,eax`; a returned pointer loads straight into ecx). The lookup key, the event out pointer and the empty filter temporary all live in the dead `entity`
// argument slot. That needs the two out-of-line callees as same-TU noinline stand-ins (LowerBound only reads its
// key; GetEvent only writes its out pointer and never touches the filter, whose parameter is unnamed), as the
// landed CAIStateGroup_ChickenPanic::IsRunnable 0x00911C20 does for the same pair; declared only, both
// addresses escape and get frame words.
// Leaf: ?EntityGetShotStrikePos@CGameScriptInterface@@UBE_NABVCScriptThing@@AAVC3DVector@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).

#include "fable_compiler.h"
#include "engine/C3DVector.h"

class CTCBase;
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     m_kind;
    CTCBase* m_pTC;
};
// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in CTCMap (?LowerBound@CTCMap@@QAEPAUSTCEntry@@ABK@Z) replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): CThing::Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QAE spelling at 0x0040F020.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
// (the row-local inline Find(kind) of the stand-in is CVectorMap::Find (engine/CVectorMap.h) on the key temporary, then ->second)

// Same-TU stand-in for CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (0x0040F020, STLport lower_bound over
// the 8-byte entries): the key is only read, so its address does not escape.

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                          // +0x44
    unsigned char  _pad54[0x3D];                   // +0x54 (CVectorMap is 0x10)
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    FABLE_FORCEINLINE unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};

template <class T> FABLE_FORCEINLINE bool GetTCOut(CThing* thing, long kind, T** out)   // out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        *out = (T*)(CTCBase*)thing->m_tcs.Find((ETCInterfaceType)kind)->second;
        return true;
    }
    return false;
}

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace NEntityEvents {
    class CEventShotStruck { public:                      // Ego_r.pdb, 0x14 bytes
        void* _vftable;                            // +0x00
        long  _04;                                 // +0x04
        C3DVector StrikePos;                       // +0x08
    };
    class CEventFilter_None { public: CEventFilter_None() {} };
}

class CTCEntityEvents { public:                           // interface 0x42
    unsigned char _00[0x0c];
    NEntityEvents::CEventShotStruck* LastShotStruck;   // stand-in view of the event list
    template <class TEvent, class TFilter> bool GetEvent(TEvent** out, const TFilter&) const;
};

#pragma auto_inline(off)
// Same-TU stand-in for CTCEntityEvents::GetEvent<CEventShotStruck, CEventFilter_None> (0x008AC7A0): it writes only
// the out pointer and never reads the filter.
template <class TEvent, class TFilter>
__declspec(noinline) bool CTCEntityEvents::GetEvent(TEvent** out, const TFilter&) const
{
    if (LastShotStruck) {
        *out = LastShotStruck;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::EntityGetShotStrikePos(const CScriptThing& entity, C3DVector& position) const
{
    CThing* thing = entity.GetPThing();
    CTCEntityEvents* events;
    if (thing && !thing->m_dead && GetTCOut(thing, 0x42, &events))
    {
        NEntityEvents::CEventShotStruck* strike;
        if (events->GetEvent(&strike, NEntityEvents::CEventFilter_None()))
        {
            position = strike->StrikePos;
            return true;
        }
    }
    return false;
}
