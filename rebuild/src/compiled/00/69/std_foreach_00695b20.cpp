// std::for_each(CCountedPointer<NCreatureAction::CActionEventBase>* first, CCountedPointer<NCreatureAction::CActionEventBase>* last, NCreatureAction::CDispatchActionEventIfActivationFrame f) -- dispatches every action event whose activation frame has passed and marks it dispatched
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x00695B20, 69 bytes, `ret 0Ch`. Caller: CCreatureActionBase::FrameUpdate 0x00694077.
// Callees: the event's virtual slot 1 (+0x04, `Dispatch(creature)`).
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: the functor's creature pointer is cached in edi, its frame re-read from the slot each iteration (no free register);
// `*it` is re-read after the virtual call for the `m_dispatched = true` store; `m_frame > ev->m_activationFrame` is `cmp eax,[ecx+8] ; jle`.
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CThing;
template <class T> class CCountedPointer {
public:
    T* m_pObject;
    long* m_pRefCount;
    T* operator->() const { return m_pObject; }
};
namespace NCreatureAction {
class CActionEventBase {
public:
    virtual ~CActionEventBase();
    virtual void Dispatch(CThing* creature);      // slot 1 (+0x04)
    bool m_dispatched;                            // +0x04
    long m_activationFrame;                       // +0x08
};
class CDispatchActionEventIfActivationFrame {
public:
    long m_frame;                                 // +0x00
    CThing* m_pCreature;                          // +0x04
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
template NCreatureAction::CDispatchActionEventIfActivationFrame __fastcall std::for_each(CCountedPointer<NCreatureAction::CActionEventBase>*, CCountedPointer<NCreatureAction::CActionEventBase>*, NCreatureAction::CDispatchActionEventIfActivationFrame);