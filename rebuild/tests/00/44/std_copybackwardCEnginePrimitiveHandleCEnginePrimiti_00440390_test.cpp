// Behaviour test for std::__copy_backward<CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, int> (0x00440390): last - first
// handles are assigned backward so that the copy ends at result (overlap-safe for an insert shift), and result - n is returned.
#include <cstdio>
static int g_notified = 0, g_destroyed = 0;
class CEnginePrimitiveHandle;
struct Prim {
    long m_handleCount;
    int  id;
    void Destroy() { ++g_destroyed; }
    void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_notified; }
    void NotifyIfLastHandle(CEnginePrimitiveHandle* h) { if (m_handleCount == 1) OnLastHandleReleased(h); }
};
struct CIVCountedPointer {
    Prim* Object;
    void Release() { Prim* o = Object; if (o) { if (--o->m_handleCount == 0) o->Destroy(); Object = 0; } }
    CIVCountedPointer& operator=(Prim* o) { Release(); Object = o; if (o) ++o->m_handleCount; return *this; }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) { Release(); Object = other.Object; if (Object) ++Object->m_handleCount; }
        return *this;
    }
};
class CEnginePrimitiveHandle {
public:
    CIVCountedPointer Data;
    void Clear() { if (Data.Object) Data.Object->NotifyIfLastHandle(this); Data = 0; }
    CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other) { Clear(); Data = other.Data; return *this; }
};
static void Bind(CEnginePrimitiveHandle& h, Prim* p) { h.Data.Object = p; if (p) ++p->m_handleCount; }

static CEnginePrimitiveHandle* copy_backward_model(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CEnginePrimitiveHandle* result)
{
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
int main()
{
    int bad = 0;
    Prim p[5] = { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 } };
    CEnginePrimitiveHandle h[5];
    for (int i = 0; i < 4; ++i) Bind(h[i], &p[i]);
    Bind(h[4], &p[4]);
    // insert-style overlap: shift h[1..4) up by one onto h[2..5)
    CEnginePrimitiveHandle* r = copy_backward_model(h + 1, h + 4, h + 5);
    if (r != h + 2) bad = 1;
    if (h[0].Data.Object != &p[0] || h[1].Data.Object != &p[1] || h[2].Data.Object != &p[1] || h[3].Data.Object != &p[2] ||
        h[4].Data.Object != &p[3]) bad = 1;
    // p[4] lost its only handle (notify + destroy); p[3], p[2] each lost one count when overwritten (no notify: still shared)
    if (g_destroyed != 1 || p[4].m_handleCount != 0 || g_notified != 1) bad = 1;
    if (p[1].m_handleCount != 2 || p[2].m_handleCount != 1 || p[3].m_handleCount != 1) bad = 1;
    if (copy_backward_model(h, h, h + 3) != h + 3) bad = 1;
    if (bad) { std::printf("00440390_TEST FAIL\n"); return 1; }
    std::printf("00440390_TEST PASS\n");
    return 0;
}