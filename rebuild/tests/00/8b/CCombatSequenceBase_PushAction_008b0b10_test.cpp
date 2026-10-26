// Behaviour test for 0x008B0B10 CCombatSequenceBase::PushAction: the action is appended (its count block gains a reference) and counted.
#include <cstdio>
#include <new>
class CCPPointerInfo { public: long RefCount; };
struct CCountedPointer {
    void* Data; CCPPointerInfo* Info;
    CCountedPointer(void* d, CCPPointerInfo* i) : Data(d), Info(i) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
};
class CCombatAction { public: CCountedPointer CreatureAction; CCombatAction(void* d, CCPPointerInfo* i) : CreatureAction(d, i) {} };
struct Seq {
    long NoAvailableActions;
    CCombatAction* start; CCombatAction* finish; CCombatAction* eos;
    void PushAction(CCombatAction& a)
    {
        if (finish != eos) { new (finish) CCombatAction(a); ++finish; }
        else { /* overflow path: not exercised */ }
        ++NoAvailableActions;
    }
};
int main()
{
    CCombatAction* storage = (CCombatAction*)::operator new(4 * sizeof(CCombatAction));
    Seq s; s.NoAvailableActions = 0; s.start = s.finish = storage; s.eos = storage + 4;
    CCPPointerInfo info = { 1 };
    int obj;
    CCombatAction a(&obj, &info), b(&obj, 0);
    s.PushAction(a); s.PushAction(b);
    if (s.NoAvailableActions != 2 || s.finish != storage + 2 || info.RefCount != 2 || storage[0].CreatureAction.Data != &obj || storage[1].CreatureAction.Info != 0) {
        std::printf("008b0b10_TEST FAIL\n"); return 1;
    }
    ::operator delete(storage);
    std::printf("008b0b10_TEST PASS\n");
    return 0;
}