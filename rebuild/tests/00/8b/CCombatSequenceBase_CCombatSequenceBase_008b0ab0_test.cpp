// behaviour test for 0x008B0AB0 CCombatSequenceBase::CCombatSequenceBase: the references become the Creature/Target pointers, the action vector starts empty and the state is cleared
#include <cstdio>
struct Creature { int id; };
struct Action { int* refs; };
struct Sequence {
    bool Running; long NoAvailableActions; long CurrentActionIndex;
    Action* start; Action* finish; Action* eos;
    Creature* PCreature; Creature* PTarget; bool CreateCalled;
    Sequence(Creature& creature, Creature& target)
        : start(0), finish(0), eos(0), PCreature(&creature), PTarget(&target)
    {
        // clear(): erase(begin(), end()) -- copy [end, end) down to begin (nothing), destroy the tail, new finish
        Action* i = start;
        for (Action* p = finish; p != finish; ++p) *i++ = *p;
        for (Action* p = i; p != finish; ++p) if (p->refs) --*p->refs;
        finish = i;
        NoAvailableActions = 0;
        CurrentActionIndex = 0;
        Running = false;
        CreateCalled = false;
    }
};
int main()
{
    Creature c = { 1 }, t = { 2 };
    Sequence s(c, t);
    if (s.PCreature != &c || s.PTarget != &t || s.start != 0 || s.finish != 0 || s.eos != 0 || s.Running || s.CreateCalled
        || s.NoAvailableActions != 0 || s.CurrentActionIndex != 0) {
        std::printf("008b0ab0_TEST FAIL\n"); return 1;
    }
    std::printf("008b0ab0_TEST PASS\n");
    return 0;
}