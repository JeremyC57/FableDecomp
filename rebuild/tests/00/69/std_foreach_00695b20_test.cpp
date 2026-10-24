#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CThing { public: int m_received; };
template <class T> class CCountedPointer {
public:
    T* m_pObject;
    long* m_pRefCount;
    T* operator->() const { return m_pObject; }
};
namespace NCreatureAction {
class CActionEventBase {
public:
    virtual ~CActionEventBase() {}
    virtual void Dispatch(CThing* creature) { ++creature->m_received; }
    bool m_dispatched;
    long m_activationFrame;
};
class CDispatchActionEventIfActivationFrame {
public:
    long m_frame;
    CThing* m_pCreature;
    CDispatchActionEventIfActivationFrame(long frame, CThing* creature) : m_frame(frame), m_pCreature(creature) {}
    void operator()(const CCountedPointer<CActionEventBase>& ev) const
    {
        if (m_frame > ev->m_activationFrame) {
            ev->Dispatch(m_pCreature);
            ev->m_dispatched = true;
        }
    }
};
}
int main()
{
    using namespace NCreatureAction;
    CThing creature; creature.m_received = 0;
    CActionEventBase e[3];
    CCountedPointer<CActionEventBase> p[3];
    long frames[3] = { 3, 10, 5 };
    for (int i = 0; i < 3; ++i) { e[i].m_dispatched = false; e[i].m_activationFrame = frames[i]; p[i].m_pObject = &e[i]; p[i].m_pRefCount = 0; }
    CDispatchActionEventIfActivationFrame f = std::for_each(p, p + 3, CDispatchActionEventIfActivationFrame(6, &creature));
    if (creature.m_received != 2 || !e[0].m_dispatched || e[1].m_dispatched || !e[2].m_dispatched) { std::printf("00695b20_TEST FAIL\n"); return 1; }
    if (f.m_frame != 6 || f.m_pCreature != &creature) { std::printf("00695b20_TEST FAIL\n"); return 1; }
    std::printf("00695b20_TEST PASS\n");
    return 0;
}