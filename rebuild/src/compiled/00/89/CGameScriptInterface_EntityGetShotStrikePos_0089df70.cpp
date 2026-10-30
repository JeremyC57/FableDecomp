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
// Leaf: ?EntityGetShotStrikePos@CGameScriptInterface

class C3DVector { public: float X, Y, Z; };

class CTCBase;
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     m_kind;
    CTCBase* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44 (sorted vector of pairs)
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020 (same-TU stand-in below)
    __forceinline CTCBase* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};

#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (0x0040F020, STLport lower_bound over
// the 8-byte entries): the key is only read, so its address does not escape.
__declspec(noinline) STCEntry* CTCMap::LowerBound(const unsigned long& kind)
{
    STCEntry* it = m_pFirst;
    int len = int(m_pLast - m_pFirst);
    while (len > 0) {
        int half = len / 2;
        STCEntry* middle = it + half;
        if ((unsigned long)middle->m_kind < kind) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};

template <class T> __forceinline bool GetTCOut(CThing* thing, long kind, T** out)   // out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        *out = (T*)thing->m_tcs.Find(kind);
        return true;
    }
    return false;
}

class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;              // slot 11 (+0x2c)
};

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
    CThing* thing = entity.GetThing();
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
