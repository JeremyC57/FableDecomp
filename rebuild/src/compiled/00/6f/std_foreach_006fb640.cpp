// std::for_each(const NAnimationEvents::CEvent* first, const NAnimationEvents::CEvent* last, NAnimationEvents::CGetEventsFilteredByTime f) -- appends every animation event whose time lies in [start, end) to a CSmallVector
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FB640, 117 bytes, `ret 10h`. Caller: CManager::GetExtraEventsBetweenTimes 0x006F961A.
// Callee: CSmallVector<NAnimationEvents::CEvent,32>::resize(long, const CEvent&) @ 0x006FADE0 (manifest).
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: `push_back(ev)` is `resize(size + 1)` + `back() = ev`; the never-written `CEvent()` default-argument temporary is homed
// over the LIVE functor slot (`lea eax,[esp+18h]`) only when resize has a same-TU noinline body that just READS the fill through
// its const reference (declared-only, VC7.1 gives the temporary its own 8-byte frame: `sub esp,8`). The stand-in is below.
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
template <class T, long N> class CSmallVector {
public:
    T* m_pData;                                   // +0x00
    long m_size;                                  // +0x04
    long m_capacity;                              // +0x08
    T m_inline[N];
    void resize(long newSize, const T& fill = T());   // 0x006FADE0 (same-TU stand-in below)
    T& back() { return m_pData[m_size - 1]; }
    void push_back(const T& value)
    {
        resize(m_size + 1);
        back() = value;
    }
};
#pragma auto_inline(off)
// Same-TU stand-in for CSmallVector::resize (0x006FADE0): it only READS the fill value through its const reference,
// so VC7.1 may home the never-written `T()` default-argument temporary over the caller's live functor slot.
template <class T, long N> __declspec(noinline) void CSmallVector<T, N>::resize(long newSize, const T& fill)
{
    for (long i = m_size; i < newSize; ++i)
        m_pData[i] = fill;
    m_size = newSize;
}
#pragma auto_inline(on)
namespace NAnimationEvents {
class CEvent {
public:
    long m_type;                                  // +0x00
    float m_time;                                 // +0x04
    CEvent() {}
};
class CGetEventsFilteredByTime {
public:
    float m_startTime;                            // +0x00
    float m_endTime;                              // +0x04
    CSmallVector<CEvent, 32>* m_pEvents;          // +0x08
    CGetEventsFilteredByTime(float start, float end, CSmallVector<CEvent, 32>* events)
        : m_startTime(start), m_endTime(end), m_pEvents(events) {}
    void operator()(const CEvent& ev)
    {
        if (ev.m_time >= m_startTime && ev.m_time < m_endTime)
            m_pEvents->push_back(ev);
    }
};
}
template NAnimationEvents::CGetEventsFilteredByTime __fastcall std::for_each(const NAnimationEvents::CEvent*, const NAnimationEvents::CEvent*, NAnimationEvents::CGetEventsFilteredByTime);