#include <cstdio>
#include <cstring>
#include <new>

struct CGameEvent { int id; };
struct CInputTypeBase { virtual bool IsButtonEvent(const CGameEvent& ev) { return ev.id == m_match; } int m_match; };
template <class T> struct CCountedPointer { T* m_pObject; void* m_pCount; };
template <class T> struct CRetailVector { T* m_pBegin; T* m_pEnd; T* m_pEnd2;
    unsigned long Size() const { return (unsigned long)(m_pEnd - m_pBegin); } T& operator[](unsigned long i) { return m_pBegin[i]; } };
struct CGamePlayerInterface {
    CRetailVector< CCountedPointer<CInputTypeBase> > ContextSensitiveItemButtons;
    bool IsEventContextSensitiveItemButton(const CGameEvent& event, unsigned long* outIndex) {
        for (unsigned long i = 0; i < ContextSensitiveItemButtons.Size(); ++i)
            if (ContextSensitiveItemButtons[i].m_pObject->IsButtonEvent(event)) { *outIndex = i; return true; }
        return false; }
};
int main()
{
    CInputTypeBase t0, t1, t2; t0.m_match = 10; t1.m_match = 20; t2.m_match = 30;
    CCountedPointer<CInputTypeBase> v[3]; v[0].m_pObject = &t0; v[1].m_pObject = &t1; v[2].m_pObject = &t2;
    CGamePlayerInterface pi; pi.ContextSensitiveItemButtons.m_pBegin = v; pi.ContextSensitiveItemButtons.m_pEnd = v + 3;
    CGameEvent ev; unsigned long idx = 99;
    ev.id = 20; if (!pi.IsEventContextSensitiveItemButton(ev, &idx) || idx != 1) { std::printf("00446110_TEST FAIL\n"); return 1; }
    ev.id = 30; if (!pi.IsEventContextSensitiveItemButton(ev, &idx) || idx != 2) { std::printf("00446110_TEST FAIL\n"); return 1; }
    ev.id = 5; idx = 99; if (pi.IsEventContextSensitiveItemButton(ev, &idx) || idx != 99) { std::printf("00446110_TEST FAIL\n"); return 1; }
    pi.ContextSensitiveItemButtons.m_pEnd = v; ev.id = 10; if (pi.IsEventContextSensitiveItemButton(ev, &idx)) { std::printf("00446110_TEST FAIL\n"); return 1; }
    std::printf("00446110_TEST PASS\n");
    return 0;
}