#include <cstdio>
struct CIndexedObject { virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual long GetIndex() { return 42; } };
struct CHandleOwner_00c628f0 { void* __vftable; CIndexedObject* m_pObject; long GetIndex(); };
long CHandleOwner_00c628f0::GetIndex() { if (m_pObject == 0) return -1; return m_pObject->GetIndex(); }
int main()
{
    CIndexedObject obj; CHandleOwner_00c628f0 h = { 0, 0 };
    if (h.GetIndex() != -1) { std::printf("00c628f0_TEST FAIL\n"); return 1; }
    h.m_pObject = &obj; if (h.GetIndex() != 42) { std::printf("00c628f0_TEST FAIL\n"); return 1; }
    std::printf("00c628f0_TEST PASS\n");
    return 0;
}
