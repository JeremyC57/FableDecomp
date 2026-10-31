// CAIStateGroup_MinionReactToPlayerInteraction::InteractionFailed() -- withdraw the newest pending interaction message (type 0x14) aimed at the creature and, if it has the interface-0x10 sound player, play the TAVERN_GAME_INCORRECT criteria
// Leaf: ?InteractionFailed@CAIStateGroup_MinionReactToPlayerInteraction@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092F240, length 193 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb ?InteractionFailed@CAIStateGroup_MinionReactToPlayerInteraction@@AAEXXZ (private); called from the landed
//   IsRunnable 0x00930A70.
// Callees: CMessageEventFilter_IsTargettedAtThing::CMessageEventFilter_IsTargettedAtThing(const CThing&) 0x006AF290 (Ego_r ctor
//   order IsTargettedAtThing 0x006C9980 / IsOriginatedByThing 0x006C99A0 = TLC 0x006AF290 / 0x006AF2B0), CAIStateGroupBase::PeekWorld
//   0x008FCFA0 / GetWorld 0x008FCFC0 (then the inline CWorld::GetMessageEventManager, FableWin.pdb), CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<CMessageEventFilter_IsOfType,
//   CMessageEventFilter_IsTargettedAtThing> > 0x008D46D0 (catalog; its body calls IsOfType::operator() 0x006AF1C0 and
//   IsTargettedAtThing::operator() 0x006AF1E0, which compares CMessageEvent::TargetUID +0x28), CMessageEventManager::RemoveMessage
//   0x00493B5B, CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound 0x0040F020 (inlined CThing::GetInterface, interface 0x10 =
//   bit 16 of +0x20), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("TAVERN_GAME_INCORRECT" 0x0126F6AC),
//   CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00.
// Layout: CWorld and CAIStateGroupBase come from their shared headers (the `#define X XLayout` include pattern); the
//   derived class header flattens its base (and types ReactionType as bytes), so it is the local model of the
//   landed CAIStateGroup_* sources (NextReactionFrame +0x14, ReactionType +0x28 from Ego_r.pdb).
// Shape: the landed message-filter chain `IsOfType(0x14) && IsTargettedAtThing(creature)` bound by pointers and passed by const
//   reference, with the FindMostRecentMessage template body compiled in this TU and not inlined (template bodies are emitted in
//   every TU that instantiates them): VC7.1 then knows the bound filter does not escape and stores its first pointer before the
//   second filter's ctor call, with the type word after (an external declaration stores both after the call: 189v193). The sound
//   criteria is a NAMED CCharString (built before the bool arguments are pushed).
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CMessageEvent;
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
    bool operator()(const CMessageEvent& message) const;             // 0x006AF1C0
};
class CMessageEventFilter_IsTargettedAtThing { public:
    unsigned __int64 UID;                       // the target's 64-bit UID (Ego_r.pdb)
    CMessageEventFilter_IsTargettedAtThing(const CThing& thing);     // 0x006AF290
    bool operator()(const CMessageEvent& message) const;             // 0x006AF1E0 (compares CMessageEvent::TargetUID +0x28)
};
class CMessageEvent { public:
    unsigned char _body[0x40];                  // opaque here (only its address is used)
};
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
    bool operator()(const CMessageEvent& message) const { return (*m_pFirst)(message) && (*m_pSecond)(message); }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
namespace std {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };
template <class T> class list { public:     // STLport 4.6: the sentinel node pointer (4 bytes)
    _List_node_base* _M_node;
};
}
class CMessageEventManager { public:
    unsigned char             _pad00[4];
    std::list<CMessageEvent>  Messages;         // +0x04 (newest first)
    // The template instantiation is compiled in every TU that uses it: same-TU, not inlined (retail calls the COMDAT
    // copy at 0x008D46D0, whose body this is: walk the list, return the first message the bound filter accepts).
    template<class F> __declspec(noinline) const CMessageEvent* FindMostRecentMessage(const F& filter) const
    {
        for (const std::_List_node_base* n = Messages._M_node->_M_next; n != Messages._M_node; n = n->_M_next) {
            const CMessageEvent& message = ((const std::_List_node<CMessageEvent>*)n)->_M_data;
            if (filter(message))
                return &message;
        }
        return 0;
    }
    void RemoveMessage(const CMessageEvent* message);                                       // 0x00493B5B
};
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
class CWorld : public CWorldLayout { public:          // PMessageEventManager (CCountedPointer) Data word at +0x60
    CMessageEventManager& GetMessageEventManager() { return *PMessageEventManager_Data; }               // FableWin.pdb QAE
    const CMessageEventManager& GetMessageEventManager() const { return *PMessageEventManager_Data; }   // FableWin.pdb QBE
};
class CTCSoundPlayer { public:
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d);   // 0x0066BD00
};
class CThingAICreature : public CThing { };
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18 (engine/CAIBrain.h)
    CThingAICreature& GetCreature() const { return *Creature; }
};
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
class CAIStateGroupBase : public CAIStateGroupBaseLayout { public:   // vptr, PBrain +0x04, ... (0x14 bytes)
protected:
    CThingAICreature& GetCreature() const { return PBrain->GetCreature(); }
    const CWorld& PeekWorld() const;            // 0x008FCFA0
    CWorld& GetWorld() const;                   // 0x008FCFC0
};
class CAIStateGroup_MinionReactToPlayerInteraction : public CAIStateGroupBase { public:
    unsigned long NextReactionFrame[5];         // +0x14
    long ReactionType;                          // +0x28
    bool WaitingForConfirmation;                // +0x2c
    bool DidTraderInteraction;                  // +0x2d
private:
    void InteractionFailed();
};

void CAIStateGroup_MinionReactToPlayerInteraction::InteractionFailed()
{
    const CMessageEvent* message = PeekWorld().GetMessageEventManager().FindMostRecentMessage(
        CMessageEventFilter_IsOfType(0x14) && CMessageEventFilter_IsTargettedAtThing(GetCreature()));
    if (message) {
        GetWorld().GetMessageEventManager().RemoveMessage(message);
        CTCSoundPlayer* soundPlayer;
        if (GetCreature().GetInterface(&soundPlayer, 0x10)) {
            CCharString criteria("TAVERN_GAME_INCORRECT");
            soundPlayer->PlayAttachedCriteria(criteria, true, false, false, -1);
        }
    }
}