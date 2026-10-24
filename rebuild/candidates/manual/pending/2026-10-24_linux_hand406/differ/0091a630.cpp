// CAIStateGroup_WorkBase::HasSeller() const -- true when the creature is in a creature interaction of type 8 (the buy/sell exchange) that has exactly two participants
// Leaf: ?HasSeller@?$CAIStateGroup_WorkBase@
// Grade: DIFFER(50v51) -- retail reaches the interaction object by address (`mov eax,[eax+18h] ;
//   add eax,12Ch ; mov eax,[eax]`), ours folds it (`mov ecx,[eax+18h] ; mov eax,[ecx+12Ch]`); the rest is identical.
//   Tried: getter chain, `T* const&` GetPObject, a named const-ref local, __forceinline getters, /Os /Oa /Og /G7.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091A630, 51 bytes (64 with padding), __thiscall, returns bool.
// Identity: descriptive name (no Ego_r.pdb symbol: the debug build inlines it); the name follows the landed caller
//   CAIStateGroup_BuyFromSeller::IsRunnable 0x0091A390, which calls it twice around AcquireSeller 0x0091A9B0. It sits with
//   the CAIStateGroup_WorkBase<CAIStateGroupBase> members (IsInWorkHours 0x0091A6F0). The catalog label
//   "Inventory_CountItems" is an autoname.
// Callees: none (STLport list::size() inlined).
// Layout: CAIStateGroupBase::PBrain +4, CAIBrain::PCreature +0x18; the creature's CCountedPointer<CCreatureInteraction> at
//   +0x12C (object first; landed CThingCreatureBase::RemoveFromCreatureInteraction 0x006629B0); retail CCreatureInteraction:
//   Participants (STLport list<CThingCreatureBase*>, one sentinel pointer) +0, Type +4 (Ego_r.pdb +0/+0xC).
// Shape: the counted pointer is reached by address in retail (`add eax,12Ch ; mov eax,[eax]`, not reproduced here: the
//   forms below fold the offset); list::size() is STLport's distance(begin(), end())
//   counting loop; `if (a && b && c) return true; return false;` gives the byte `mov al,1` / `xor al,al` exits.
struct CThingCreatureBase;

namespace _STL {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> class list {               // STLport list: one sentinel pointer in retail
public:
    _List_node_base* _M_node;
    unsigned int size() const
    {
        unsigned int __result = 0;
        for (_List_node_base* __first = _M_node->_M_next; __first != _M_node; __first = __first->_M_next)
            ++__result;
        return __result;
    }
};

}

enum ECreatureInteractionType { CREATURE_INTERACTION_TRADE = 8 };   // descriptive: the value the seller code tests

class CCreatureInteraction {
public:
    _STL::list<CThingCreatureBase*> Participants;   // +0x00
    ECreatureInteractionType Type;                  // +0x04
    ECreatureInteractionType GetType() const { return Type; }
};

template <class T> class CCountedPointer {
public:
    T* m_pObject;                               // +0x00
    void* m_pCount;                             // +0x04
    T* GetPObject() const { return m_pObject; }
};

class CThingCreatureBase {
public:
    unsigned char _pad00[0x12c];
    CCountedPointer<CCreatureInteraction> PCreatureInteraction;     // +0x12C
    const CCountedPointer<CCreatureInteraction>& GetPCreatureInteraction() const { return PCreatureInteraction; }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
};

template <class TBase> class CAIStateGroup_WorkBase : public TBase {
public:
    bool HasSeller() const;
};

template <class TBase> bool CAIStateGroup_WorkBase<TBase>::HasSeller() const
{
    CCreatureInteraction* interaction = this->GetCreature()->GetPCreatureInteraction().GetPObject();
    if (interaction && interaction->GetType() == CREATURE_INTERACTION_TRADE && interaction->Participants.size() == 2)
        return true;
    return false;
}

template bool CAIStateGroup_WorkBase<CAIStateGroupBase>::HasSeller() const;
