#include <cstdio>
struct CThing {
    unsigned long ThingTypeFlags;
    long X;
    bool Gossipee;
    bool Visible;
};
struct CThingAICreature : CThing {
    enum { ThingTypeMask = 0x1f };
};
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return !(~thing->ThingTypeFlags & mask);
}
struct CBaseObject;
struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseObject* GetPItem() const { return PData; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    template <class U> U* GetTypedPItem() const
    {
        return GetPItem() && IsThingOfType<U>((U*)GetPItem()) ? (U*)GetPItem() : 0;
    }
};
struct CMessageEvent {
    long Type;
    CIntelligentPointer<CThing> POriginator;
};
struct CMessageEventFilter_IsOfType {
    long Type;
    bool operator()(const CMessageEvent& message) const { return message.Type == Type; }
};
struct CThingFilter_IsValidGossipee {
    bool operator()(CThingAICreature* creature) const { return creature->Gossipee; }
};
struct CThingFilter_In2DRange {
    long Range;
    bool operator()(CThing* thing) const { return thing->X < Range; }
};
struct CThingFilter_IsNavLineOfSightToThing {
    bool operator()(CThing* thing) const { return thing->Visible; }
};
template <class F> struct CMessageEventFilter_ThingSearchFilter {
    F* Filter;
    bool operator()(const CMessageEvent& message) const
    {
        CThingAICreature* creature = message.POriginator.GetTypedPItem<CThingAICreature>();
        if (!creature)
            return false;
        return (*Filter)(creature);
    }
};
template <> struct CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing> {
    CThingFilter_IsNavLineOfSightToThing* Filter;
    bool operator()(const CMessageEvent& message) const
    {
        CThingAICreature* creature = message.POriginator.GetTypedPItem<CThingAICreature>();
        if (!creature)
            return false;
        return (*Filter)(creature);
    }
};
template <class F1, class F2, class F3, class F4> struct CFilter_Bind4 {
    F1* PF1;
    F2* PF2;
    F3* PF3;
    F4* PF4;
    bool operator()(const CMessageEvent& message) const
    {
        return (*PF1)(message) && (*PF2)(message) && (*PF3)(message) && (*PF4)(message);
    }
};
struct CMessageList {                            // retail one-pointer list
    struct node { node* next; node* prev; CMessageEvent value; };
    node* head;
};
struct CWorld;
struct CMessageEventManager {
    CWorld*      World;                         // +0x00
    CMessageList Messages;                      // +0x04
    template <class Filter> const CMessageEvent* FindMostRecentMessage(const Filter& filter) const;
};

template <class Filter> const CMessageEvent* CMessageEventManager::FindMostRecentMessage(const Filter& filter) const
{
    for (CMessageList::node* n = Messages.head->next; n != Messages.head; n = n->next) {
        const CMessageEvent* message = &n->value;
        if (filter(*message))
            return message;
    }
    return 0;
}
typedef CFilter_Bind4<CMessageEventFilter_IsOfType, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing> > TFilter;
template <class A, class B> struct SPick;
template <class A> struct SPick<A, A> { static A* Get(A* a) { return a; } };
inline void Assign(CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>*& p, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>& g, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>&) { p = &g; }
inline void Assign(CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>*& p, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>&, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>& r) { p = &r; }
inline void SetPointers(TFilter& f, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>& g, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>& r, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>& l)
{ Assign(f.PF2, g, r); Assign(f.PF3, g, r); f.PF4 = &l; }
static int fail() { std::printf("0090e4a0_TEST FAIL\n"); return 1; }
int main()
{
    CThingAICreature things[4];
    for (int i = 0; i < 4; ++i) { things[i].ThingTypeFlags = 0x1f; things[i].X = i * 10; things[i].Gossipee = true; things[i].Visible = true; }
    things[1].ThingTypeFlags = 0x0f;            // not a creature
    things[2].Gossipee = false;
    CMessageList::node nodes[6];
    CMessageList list; list.head = &nodes[0];
    for (int i = 0; i < 6; ++i) { nodes[i].next = &nodes[(i + 1) % 6]; nodes[i].value.Type = 5; nodes[i].value.POriginator.PData = 0; }
    nodes[1].value.Type = 4; nodes[1].value.POriginator.PData = (CBaseObject*)&things[0];
    nodes[2].value.POriginator.PData = 0;
    nodes[3].value.POriginator.PData = (CBaseObject*)&things[1];
    nodes[4].value.POriginator.PData = (CBaseObject*)&things[2];
    nodes[5].value.POriginator.PData = (CBaseObject*)&things[3];
    CMessageEventManager mgr; mgr.World = 0; mgr.Messages = list;
    CMessageEventFilter_IsOfType f1; f1.Type = 5;
    CThingFilter_IsValidGossipee g; CThingFilter_In2DRange r; r.Range = 100; CThingFilter_IsNavLineOfSightToThing l;
    CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee> tg; tg.Filter = &g;
    CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> tr; tr.Filter = &r;
    CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing> tl; tl.Filter = &l;
    TFilter f; f.PF1 = &f1; SetPointers(f, tg, tr, tl);
    if (mgr.FindMostRecentMessage(f) != &nodes[5].value) return fail();
    r.Range = 25; if (mgr.FindMostRecentMessage(f) != 0) return fail();
    r.Range = 100; things[3].Visible = false; if (mgr.FindMostRecentMessage(f) != 0) return fail();
    things[3].Visible = true; things[2].Gossipee = true; if (mgr.FindMostRecentMessage(f) != &nodes[4].value) return fail();
    f1.Type = 4; if (mgr.FindMostRecentMessage(f) != &nodes[1].value) return fail();
    f1.Type = 9; if (mgr.FindMostRecentMessage(f) != 0) return fail();
    std::printf("0090e4a0_TEST PASS\n");
    return 0;
}