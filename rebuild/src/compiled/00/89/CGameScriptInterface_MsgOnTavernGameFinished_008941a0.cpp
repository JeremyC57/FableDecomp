// CGameScriptInterface::MsgOnTavernGameFinished(const CScriptThing& winner) const -- whether a "tavern game finished"
// message (type 0x40) seen or heard this frame names the given (alive) thing as its target
// Leaf: ?MsgOnTavernGameFinished@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008941A0, length 176 bytes, `ret 4`.
// Callees: GetMinWorldFrameForMessages @ 0x006E7510, GetMaxWorldFrameForMessages @ 0x006E7530,
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>> @ 0x008ABED0 (CWorld+0x60),
// CScriptThing virtual slot 11 GetThing (+0x2C), CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: CMessageEvent PTarget +0x48 (CIntelligentPointer<CThing>, PDB +0x50); CThing dead bit +0x91 bit 0.
// Layout: CThing ThingTypeFlags +0x6c (retail, as the landed 0x008D04E0 model).
// Shape: the Msg-family filter frame (landed 0x00893FF0 model); the message target is the landed typed-item test of
// 0x008D04E0 (MsgIsKilledBy): `GetPItem() != 0 && IsThingOfType<CThing>(GetPItem())` with a zero type mask copied to a
// local, which keeps the dead store of the second GetPItem result in the dead `winner` slot and the constant
// `xor cl,cl ; jne`, then the third GetPItem as the target; `return a && b && c;` gives the full-register 0/1 exits.
class CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    CBaseObject* Item;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CThing { public:
    enum { ThingTypeMask = 0 };
    unsigned char _pad[0x6c];
    unsigned long ThingTypeFlags;                   // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char m_dead : 1;                       // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
template <class T> inline bool IsThingOfType(const CThing* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}

inline bool IS_THING_DEAD(CThing* const& thing) { return thing->m_dead; }
template<class T> inline bool IS_THING_ALIVE(T* const& thing) { return thing != 0 && !IS_THING_DEAD(thing); }

class CMessageEvent { public:
    unsigned char _pad[0x48];
    CIntelligentPointer<CThing> PTarget;            // +0x48
    CThing* GetTarget() const
    {
        CThing* target = 0;
        if (PTarget.GetPItem() != 0 && IsThingOfType<CThing>(PTarget.GetPItem()))
            target = PTarget.GetPItem();
        return target;
    }
};
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
class CMessageEventFilter_CanBeSeenOrHeard { public:
    long m_frame;
    long m_maxFrame;
    CMessageEventFilter_CanBeSeenOrHeard(long frame, long maxFrame) : m_frame(frame), m_maxFrame(maxFrame) { }
};
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CSeenOrHeardFilter;
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CSeenOrHeardFilter& filter);   // 0x008ABED0
};
class CWorld { public:
    virtual void w000();
    unsigned char _pad04[0x5c];
    CMessageEventManager* PMessageEventManager_Data;   // +0x60
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::MsgOnTavernGameFinished(const CScriptThing& winner) const
{
    long frame = GetMinWorldFrameForMessages();
    long maxFrame = GetMaxWorldFrameForMessages();
    CMessageEventManager* messages = World->PMessageEventManager_Data;
    CMessageEvent* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x40) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame));
    return message && IS_THING_ALIVE(winner.GetPThing()) && winner.GetPThing() == message->GetTarget();
}