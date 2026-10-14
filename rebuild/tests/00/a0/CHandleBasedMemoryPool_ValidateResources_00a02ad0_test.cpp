#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00A02AD0: exercises the same logic through a copy of the model (compiled separately from the source)
struct CHeader { long Handle; long _unused04; long Size; short Padding; char Type; char Flags; };
template <class T> struct CArray { T* m_pFirst; T* m_pLast; T* m_pEnd; unsigned long GetCount() const { return (unsigned long)(m_pLast - m_pFirst); } T& operator[](unsigned long index) const { return m_pFirst[index]; } };
static long g_sum = 0; static int g_calls = 0;
struct CHandleBasedMemoryPool {
    virtual void ValidateResource(long handle) { ++g_calls; g_sum += handle; }
    CArray<CHeader*> Handles;
    void ValidateResources()
    {
        for (unsigned long index = 0; index < Handles.GetCount(); ++index) {
            CHeader* area = Handles[index];
            if (area && area->Type == 'T') ValidateResource(area->Handle);
        }
    }
};
static bool Run()
{
    CHeader taken1, taken2, free1; taken1.Type = 'T'; taken1.Handle = 5; taken2.Type = 'T'; taken2.Handle = 7; free1.Type = 'N'; free1.Handle = 100;
    CHeader* slots[5] = { &taken1, 0, &free1, &taken2, 0 };
    CHandleBasedMemoryPool pool; pool.Handles.m_pFirst = slots; pool.Handles.m_pLast = slots + 5; pool.Handles.m_pEnd = slots + 5;
    pool.ValidateResources();
    if (g_calls != 2 || g_sum != 12) return false;
    pool.Handles.m_pLast = slots;                      // empty
    pool.ValidateResources();
    return g_calls == 2;
}
int main()
{
    if (!Run()) { std::printf("00a02ad0_TEST FAIL\n"); return 1; }
    std::printf("00a02ad0_TEST PASS\n");
    return 0;
}