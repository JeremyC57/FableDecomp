#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
int g_released = 0;
int g_deleted = 0;
struct SCountedPointerInfo {
    long m_count;
    void (__fastcall *m_pfnRelease)(void* owner);
    void* m_pOwner;
};
void __fastcall ReleaseOwner(void* owner) { ++g_released; (void)owner; }
void FreeInfo(SCountedPointerInfo* info) { ++g_deleted; (void)info; }
template <class T> class CCountedPointer {
public:
    T* m_pObject;
    SCountedPointerInfo* m_pInfo;
    CCountedPointer(T* object, SCountedPointerInfo* info) : m_pObject(object), m_pInfo(info) {}
    CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject), m_pInfo(o.m_pInfo)
    {
        if (m_pInfo)
            ++m_pInfo->m_count;
    }
    ~CCountedPointer()
    {
        if (m_pInfo && --m_pInfo->m_count == 0) {
            m_pInfo->m_pfnRelease(m_pInfo->m_pOwner);
            FreeInfo(m_pInfo);
        }
    }
    T* operator->() const { return m_pObject; }
};
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
namespace NShape {
class CShape {
public:
    int m_calls;
    CShape() : m_calls(0) {}
    virtual ~CShape() {}
    virtual bool IsTrack() const { ++const_cast<CShape*>(this)->m_calls; return true; }
};
struct CCallShapeSlot1 {
    bool operator()(CCountedPointer<CShape> shape) const { return shape->IsTrack(); }
};
}
int main()
{
    using namespace NShape;
    CShape s1, s2;
    SCountedPointerInfo i1 = { 1, ReleaseOwner, 0 };
    SCountedPointerInfo i2 = { 1, ReleaseOwner, 0 };
    CCountedPointer<CShape> p[3] = { CCountedPointer<CShape>(&s1, &i1), CCountedPointer<CShape>(&s2, &i2), CCountedPointer<CShape>(&s1, 0) };
    std::for_each((const CCountedPointer<CShape>*)p, (const CCountedPointer<CShape>*)p + 3, CCallShapeSlot1());
    if (s1.m_calls != 2 || s2.m_calls != 1 || i1.m_count != 1 || i2.m_count != 1 || g_released != 0 || g_deleted != 0) { std::printf("0084f280_TEST FAIL\n"); return 1; }
    p[2].m_pInfo = 0;
    std::printf("0084f280_TEST PASS\n");
    return 0;
}