// Behaviour test for CAIStateGroup_MinionReactToPlayerInteraction::InteractionFailed (0x0092F240): find the newest message of
// type 0x14 aimed at the creature (IsOfType && IsTargettedAtThing over the newest-first list), remove it, and when the creature
// has the interface-0x10 sound player play "TAVERN_GAME_INCORRECT" (true, false, false, -1).
#include <cstdio>
#include <cstring>

class CMessageEvent { public: long Type; unsigned long TargetUID; bool removed; };
class CMessageEventManager { public:
    CMessageEvent* Messages[8]; int Count;       // newest first
    template<class F> CMessageEvent* FindMostRecentMessage(const F& f) const
    {
        for (int i = 0; i < Count; ++i)
            if (!Messages[i]->removed && f(*Messages[i])) return Messages[i];
        return 0;
    }
    void RemoveMessage(CMessageEvent* m) { m->removed = true; }
};
class CMessageEventFilter_IsOfType { public:
    long m_type; CMessageEventFilter_IsOfType(long t) : m_type(t) { }
    bool operator()(const CMessageEvent& m) const { return m.Type == m_type; }
};
class CThing { public: unsigned long UID; bool hasSoundPlayer; int played; char criteria[32]; };
class CMessageEventFilter_IsTargettedAtThing { public:
    unsigned long UID; CMessageEventFilter_IsTargettedAtThing(const CThing& t) : UID(t.UID) { }
    bool operator()(const CMessageEvent& m) const { return m.TargetUID == UID; }
};
template<class A, class B> class CFilter_Bind2 { public:
    const A* a; const B* b;
    CFilter_Bind2(const A& x, const B& y) : a(&x), b(&y) { }
    bool operator()(const CMessageEvent& m) const { return (*a)(m) && (*b)(m); }
};
template<class A, class B> CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }

struct CState {
    CMessageEventManager* World; CThing* Creature;
    void InteractionFailed()
    {
        CMessageEvent* message = World->FindMostRecentMessage(
            CMessageEventFilter_IsOfType(0x14) && CMessageEventFilter_IsTargettedAtThing(*Creature));
        if (message) {
            World->RemoveMessage(message);
            if (Creature->hasSoundPlayer) {
                ++Creature->played;
                std::strcpy(Creature->criteria, "TAVERN_GAME_INCORRECT");
            }
        }
    }
};

int main()
{
    CMessageEvent a = { 0x14, 7, false }, b = { 0x14, 9, false }, c = { 0x13, 9, false };
    CMessageEventManager mgr; mgr.Count = 3; mgr.Messages[0] = &c; mgr.Messages[1] = &b; mgr.Messages[2] = &a;
    CThing creature; creature.UID = 9; creature.hasSoundPlayer = true; creature.played = 0; creature.criteria[0] = 0;
    CState s; s.World = &mgr; s.Creature = &creature;
    s.InteractionFailed();
    if (!b.removed || a.removed || c.removed || creature.played != 1 || std::strcmp(creature.criteria, "TAVERN_GAME_INCORRECT"))
    { std::printf("0092f240_TEST FAIL\n"); return 1; }
    s.InteractionFailed();                     // no more matching message: nothing happens
    if (creature.played != 1 || a.removed) { std::printf("0092f240_TEST FAIL\n"); return 1; }
    CThing other; other.UID = 7; other.hasSoundPlayer = false; other.played = 0;
    s.Creature = &other; s.InteractionFailed();
    if (!a.removed || other.played != 0) { std::printf("0092f240_TEST FAIL\n"); return 1; }
    std::printf("0092f240_TEST PASS\n");
    return 0;
}