// Behaviour test for 0x00A6B000 _STL::fill(CIVCountedPointer<CLocalDetailPlacementGrid>*, ..., const CIVCountedPointer&)
#include <cstdio>

static int g_destroyed = 0;

class CIVCountedPointeeBase {
public:
    long m_count;
    CIVCountedPointeeBase() : m_count(0) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Destroy() { ++g_destroyed; }           // slot 1 in retail
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};
class CLocalDetailPlacementGrid : public CIVCountedPointeeBase {
};

namespace _STL {
template <class _ForwardIter, class _Tp>
void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
}   // namespace _STL

typedef CIVCountedPointer<CLocalDetailPlacementGrid> CGridPtr;

int main()
{
    CLocalDetailPlacementGrid a, b, c;
    CGridPtr slots[4];
    slots[0].Object = &a; slots[1].Object = &a; slots[2].Object = &b; slots[3].Object = 0;
    a.m_count = 2; b.m_count = 1;
    CGridPtr val;
    val.Object = &c;
    c.m_count = 1;
    _STL::fill(slots, slots + 4, val);
    bool ok = a.m_count == 0 && b.m_count == 0 && c.m_count == 5 && g_destroyed == 2;
    for (int i = 0; ok && i < 4; ++i)
        ok = slots[i].Object == &c;
    _STL::fill(slots, slots + 4, val);                  // same pointer: no count change
    ok = ok && c.m_count == 5 && g_destroyed == 2;
    if (!ok) { std::printf("00a6b000_TEST FAIL\n"); return 1; }
    std::printf("00a6b000_TEST PASS\n");
    return 0;
}