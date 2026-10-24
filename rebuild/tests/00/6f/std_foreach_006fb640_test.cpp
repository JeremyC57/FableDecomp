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
template <class T, long N> class CSmallVector {
public:
    T* m_pData;
    long m_size;
    long m_capacity;
    T m_inline[N];
    CSmallVector() : m_pData(m_inline), m_size(0), m_capacity(N) {}
    void resize(long newSize, const T& fill = T())
    {
        for (long i = m_size; i < newSize; ++i)
            m_pData[i] = fill;
        m_size = newSize;
    }
    T& back() { return m_pData[m_size - 1]; }
    void push_back(const T& value)
    {
        resize(m_size + 1);
        back() = value;
    }
};
namespace NAnimationEvents {
class CEvent {
public:
    long m_type;
    float m_time;
    CEvent() {}
};
class CGetEventsFilteredByTime {
public:
    float m_startTime;
    float m_endTime;
    CSmallVector<CEvent, 32>* m_pEvents;
    CGetEventsFilteredByTime(float start, float end, CSmallVector<CEvent, 32>* events)
        : m_startTime(start), m_endTime(end), m_pEvents(events) {}
    void operator()(const CEvent& ev)
    {
        if (ev.m_time >= m_startTime && ev.m_time < m_endTime)
            m_pEvents->push_back(ev);
    }
};
}
int main()
{
    using namespace NAnimationEvents;
    CSmallVector<CEvent, 32> out;
    CEvent e[5]; float t[5] = { 0.5f, 1.0f, 1.5f, 2.0f, 3.0f };
    for (int i = 0; i < 5; ++i) { e[i].m_type = i; e[i].m_time = t[i]; }
    CGetEventsFilteredByTime f = std::for_each(e, e + 5, CGetEventsFilteredByTime(1.0f, 2.0f, &out));
    if (out.m_size != 2 || out.m_pData[0].m_type != 1 || out.m_pData[1].m_type != 2) { std::printf("006fb640_TEST FAIL\n"); return 1; }
    if (f.m_startTime != 1.0f || f.m_endTime != 2.0f || f.m_pEvents != &out) { std::printf("006fb640_TEST FAIL\n"); return 1; }
    std::printf("006fb640_TEST PASS\n");
    return 0;
}