// Behaviour test for 0x005FC510 CTCInventoryMap::ResetPrimitiveHandles(): for each of the four map-screen primitive
// handle vectors, Clear() every handle (for_each + mem_fun_ref, 0x0043F360 / 0x0041BE70) and then clear() the vector
// (erase(begin, end), 0x00440D70) -- all modelled here.
#include <cstdio>

static int g_clears = 0;

class CEnginePrimitiveHandle { public:
    int  m_id;
    bool m_cleared;
    void Clear() { m_cleared = true; ++g_clears; }
};

template <class R, class T> class mem_fun_ref_t {
public:
    explicit mem_fun_ref_t(R (T::*pfn)()) : m_pfn(pfn) {}
    R operator()(T& object) const { return (object.*m_pfn)(); }
private:
    R (T::*m_pfn)();
};
template <class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pfn)()) { return mem_fun_ref_t<R, T>(pfn); }

template <class It, class F> F for_each(It first, It last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

namespace stl {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last)
    {
        T* out = first;
        for (T* in = last; in != _M_finish; ++in, ++out) *out = *in;
        _M_finish = out;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
}

typedef stl::vector<CEnginePrimitiveHandle> CPrimitiveHandleVector;

class CTCInventoryMap { public:
    CPrimitiveHandleVector MapScreenPrimitiveHandles;
    CPrimitiveHandleVector MapScreenQuestPrimitiveHandles;
    CPrimitiveHandleVector MapScreenTeleportPrimitiveHandles;
    CPrimitiveHandleVector MapScreenHousePrimitiveHandles;
    void ResetPrimitiveHandles()
    {
        for_each(MapScreenPrimitiveHandles.begin(), MapScreenPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        MapScreenPrimitiveHandles.clear();
        for_each(MapScreenQuestPrimitiveHandles.begin(), MapScreenQuestPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        MapScreenQuestPrimitiveHandles.clear();
        for_each(MapScreenTeleportPrimitiveHandles.begin(), MapScreenTeleportPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        MapScreenTeleportPrimitiveHandles.clear();
        for_each(MapScreenHousePrimitiveHandles.begin(), MapScreenHousePrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        MapScreenHousePrimitiveHandles.clear();
    }
};

static void Init(CPrimitiveHandleVector& v, CEnginePrimitiveHandle* store, int n, int cap)
{
    for (int i = 0; i < cap; ++i) { store[i].m_id = i; store[i].m_cleared = false; }
    v._M_start = store;
    v._M_finish = store + n;
    v._M_end_of_storage = store + cap;
}

int main()
{
    CEnginePrimitiveHandle a[3], b[1], c[4], d[2];
    CTCInventoryMap map;
    Init(map.MapScreenPrimitiveHandles, a, 3, 3);
    Init(map.MapScreenQuestPrimitiveHandles, b, 0, 1);
    Init(map.MapScreenTeleportPrimitiveHandles, c, 4, 4);
    Init(map.MapScreenHousePrimitiveHandles, d, 2, 2);
    map.ResetPrimitiveHandles();
    int ok = g_clears == 9;
    if (map.MapScreenPrimitiveHandles._M_finish != a || map.MapScreenQuestPrimitiveHandles._M_finish != b ||
        map.MapScreenTeleportPrimitiveHandles._M_finish != c || map.MapScreenHousePrimitiveHandles._M_finish != d) ok = 0;
    if (map.MapScreenTeleportPrimitiveHandles._M_end_of_storage != c + 4) ok = 0;   // capacity kept
    if (!a[2].m_cleared || !c[3].m_cleared || !d[0].m_cleared || b[0].m_cleared) ok = 0;
    if (!ok) { std::printf("005fc510_TEST FAIL\n"); return 1; }
    std::printf("005fc510_TEST PASS\n");
    return 0;
}