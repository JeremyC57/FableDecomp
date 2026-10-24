// Behaviour test for std::fill<CEnginePrimitiveHandle*, CEnginePrimitiveHandle> (0x00440100): every handle in the range is assigned the
// value: a target holding the last handle of its primitive notifies it (slot 3) and destroys it (slot 1), the value's primitive
// gains one count per element, a target already sharing the value's primitive is left alone.
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

static void fill_model(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, const CEnginePrimitiveHandle& val)
{
    for (; first != last; ++first)
        *first = val;
}
int main()
{
    int bad = 0;
    Prim a = { 0, 1 }, b = { 0, 2 }, v = { 0, 3 };
    CEnginePrimitiveHandle h[4];
    Bind(h[0], &a); Bind(h[1], &a); Bind(h[2], &b); Bind(h[3], 0);
    CEnginePrimitiveHandle val; Bind(val, &v);
    fill_model(h, h + 4, val);
    // a: h[0] dropped it (count 2 -> 1, no notify), h[1] held the last one (notify + destroy); b: notify + destroy
    if (g_notified != 2 || g_destroyed != 2 || a.m_handleCount != 0 || b.m_handleCount != 0) bad = 1;
    if (v.m_handleCount != 5) bad = 1;
    for (int i = 0; i < 4; ++i) if (h[i].Data.Object != &v) bad = 1;
    fill_model(h, h + 2, val);                     // same primitive: no release, no count change
    if (v.m_handleCount != 5 || g_notified != 2 || g_destroyed != 2) bad = 1;
    fill_model(h, h, val);                         // empty range
    if (v.m_handleCount != 5) bad = 1;
    if (bad) { std::printf("00440100_TEST FAIL\n"); return 1; }
    std::printf("00440100_TEST PASS\n");
    return 0;
}