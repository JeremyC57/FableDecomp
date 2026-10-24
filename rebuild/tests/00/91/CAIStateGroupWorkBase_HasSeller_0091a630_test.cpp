// Behaviour test for 0x0091A630 CAIStateGroup_WorkBase<CAIStateGroupBase>::HasSeller: true only when the creature has
// an interaction, it is of type 8, and its participant list holds exactly two entries.
#include <stdio.h>
#include <string.h>
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
    T* operator->() const { return m_pObject; }
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
    const CCountedPointer<CCreatureInteraction>& interaction = this->GetCreature()->GetPCreatureInteraction();
    if (interaction.GetPObject() && interaction->GetType() == CREATURE_INTERACTION_TRADE && interaction->Participants.size() == 2)
        return true;
    return false;
}

template bool CAIStateGroup_WorkBase<CAIStateGroupBase>::HasSeller() const;


CAIStateGroupBase::~CAIStateGroupBase() { }

static void Link(_STL::_List_node_base* sentinel, _STL::_List_node_base* nodes, unsigned int count)
{
    _STL::_List_node_base* prev = sentinel;
    for (unsigned int i = 0; i < count; ++i) {
        prev->_M_next = &nodes[i];
        nodes[i]._M_prev = prev;
        prev = &nodes[i];
    }
    prev->_M_next = sentinel;
    sentinel->_M_prev = prev;
}

int main()
{
    static CThingCreatureBase creature;
    memset(&creature, 0, sizeof(creature));
    CAIBrain brain;
    memset(&brain, 0, sizeof(brain));
    brain.PCreature = &creature;
    CAIStateGroup_WorkBase<CAIStateGroupBase>* group =
        (CAIStateGroup_WorkBase<CAIStateGroupBase>*)new CAIStateGroupBase();
    group->PBrain = &brain;

    _STL::_List_node_base sentinel, nodes[3];
    CCreatureInteraction interaction;
    interaction.Participants._M_node = &sentinel;
    interaction.Type = CREATURE_INTERACTION_TRADE;

    int ok = 1;
    if (group->HasSeller()) ok = 0;                     // no interaction
    creature.PCreatureInteraction.m_pObject = &interaction;
    Link(&sentinel, nodes, 2);
    if (!group->HasSeller()) ok = 0;                    // trade with two participants
    Link(&sentinel, nodes, 3);
    if (group->HasSeller()) ok = 0;                     // three participants
    Link(&sentinel, nodes, 0);
    if (group->HasSeller()) ok = 0;                     // empty
    Link(&sentinel, nodes, 2);
    interaction.Type = (ECreatureInteractionType)7;
    if (group->HasSeller()) ok = 0;                     // wrong type

    delete (CAIStateGroupBase*)group;
    printf(ok ? "0091a630_TEST PASS\n" : "0091a630_TEST FAIL\n");
    return ok ? 0 : 1;
}