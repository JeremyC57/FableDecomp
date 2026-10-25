// Behaviour test (0x0082a1d0): CMiniMapDisplay::ClearPrimitiveHandles clears every map handle, then every marker handle
#include <cstdio>
static int g_order = 0;
struct CEnginePrimitiveHandle {
    int  Order;
    bool Cleared;
    void Clear() { Cleared = true; Order = ++g_order; }
};
template <class R, class T> class mem_fun_ref_t {
public:
    explicit mem_fun_ref_t(R (T::*pfn)()) : m_pfn(pfn) {}
    R operator()(T& object) const { return (object.*m_pfn)(); }
private:
    R (T::*m_pfn)();
};
template <class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pfn)()) { return mem_fun_ref_t<R, T>(pfn); }
template <class It, class F> F for_each_model(It first, It last, F f) { for (; first != last; ++first) f(*first); return f; }
struct CHandleVector {
    CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd;
    CEnginePrimitiveHandle* begin() { return m_pFirst; }
    CEnginePrimitiveHandle* end() { return m_pLast; }
};
struct CMiniMapDisplay {
    CHandleVector MapPrimitiveHandles;
    CHandleVector MarkerPrimitiveHandles;
    void ClearPrimitiveHandles()
    {
        for_each_model(MapPrimitiveHandles.begin(), MapPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        for_each_model(MarkerPrimitiveHandles.begin(), MarkerPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    }
};
int main()
{
    CEnginePrimitiveHandle maps[3], markers[2];
    for (int i = 0; i < 3; ++i) { maps[i].Cleared = false; maps[i].Order = 0; }
    for (int j = 0; j < 2; ++j) { markers[j].Cleared = false; markers[j].Order = 0; }
    CMiniMapDisplay d;
    d.MapPrimitiveHandles.m_pFirst = maps; d.MapPrimitiveHandles.m_pLast = maps + 3; d.MapPrimitiveHandles.m_pEnd = maps + 3;
    d.MarkerPrimitiveHandles.m_pFirst = markers; d.MarkerPrimitiveHandles.m_pLast = markers + 2; d.MarkerPrimitiveHandles.m_pEnd = markers + 2;
    d.ClearPrimitiveHandles();
    if (!maps[0].Cleared || !maps[2].Cleared || !markers[1].Cleared || maps[0].Order != 1 || maps[2].Order != 3 || markers[0].Order != 4 || markers[1].Order != 5)
        { std::printf("0082a1d0_TEST FAIL\n"); return 1; }
    std::printf("0082a1d0_TEST PASS\n");
    return 0;
}