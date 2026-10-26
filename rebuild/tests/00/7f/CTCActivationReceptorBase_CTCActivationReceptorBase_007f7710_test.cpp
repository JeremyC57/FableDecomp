#include <cstdio>
#include <cstring>

class CThing { public: int id; };
static unsigned long g_defaultFramesToDeactivate = 150;
class CTCBase { public: CThing* ParentThing; bool ToBeRemoved, IsInGlobalUpdateTCs; CTCBase(CThing& o) : ParentThing(&o), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { } };
struct CThingPtrVector { CThing** m_first; CThing** m_last; CThing** m_end; CThingPtrVector() : m_first(0), m_last(0), m_end(0) { } };
class CTCActivationReceptorBase : public CTCBase { public:
    bool Activated; CThingPtrVector Triggers; long LogicType; bool DeactivateAfterSetTime;
    unsigned long FramesAfterActivationToDeactivate, FrameAtLastActivation;
    CTCActivationReceptorBase(CThing& owner)
        : CTCBase(owner), Activated(false), LogicType(1), DeactivateAfterSetTime(true),
          FramesAfterActivationToDeactivate(g_defaultFramesToDeactivate), FrameAtLastActivation(0) { }
};
int main()
{
    CThing thing; thing.id = 1;
    CTCActivationReceptorBase r(thing);
    if (r.ParentThing != &thing || r.Activated || r.Triggers.m_first || r.Triggers.m_last || r.Triggers.m_end) { std::printf("007f7710_TEST FAIL\n"); return 1; }
    if (r.LogicType != 1 || !r.DeactivateAfterSetTime || r.FramesAfterActivationToDeactivate != 150 || r.FrameAtLastActivation != 0) { std::printf("007f7710_TEST FAIL\n"); return 1; }
    std::printf("007f7710_TEST PASS\n");
    return 0;
}