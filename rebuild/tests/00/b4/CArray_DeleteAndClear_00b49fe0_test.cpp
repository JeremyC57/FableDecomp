#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
// Behaviour test for CArray<CEngineLightingManager::CManagerLocalLight*>::DeleteAndClear() @ 0x00B49FE0: every pointer
// goes back on the local-light heap's free list (most recent first), the heap's last free runs RefCountedRelease when
// enabled, and the array ends empty with its storage kept.
struct CObjectEntry { CObjectEntry* Next; };
static int g_released = 0;
class CDynamicObjectHeapBase {
public:
    long AllocCount;
    long TotalAllocatorBlocks;
    bool RefCountReleaseEnabled;
    CDynamicObjectHeapBase* HeapNext;
    void RefCountedRelease() { ++g_released; }
};
class CDynamicObjectHeap : public CDynamicObjectHeapBase {
public:
    CObjectEntry* FreeList;
    void Free(void* object)
    {
        --AllocCount;
        ((CObjectEntry*)object)->Next = FreeList;
        FreeList = (CObjectEntry*)object;
        if (AllocCount == 0 && RefCountReleaseEnabled)
            RefCountedRelease();
    }
};
static CDynamicObjectHeap g_LocalLightHeap;
struct CManagerLocalLight {
    void* Payload[18];
    static void __fastcall operator delete(void* object) { g_LocalLightHeap.Free(object); }
};
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
template <class T> class CArray {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
    void clear() { _M_finish = (T*)CopyTrivial(_M_finish, _M_finish, _M_start); }
    void DeleteAndClear()
    {
        for (unsigned int i = 0; i < size(); ++i)
            delete (*this)[i];
        clear();
    }
};
static CManagerLocalLight g_lights[3];
int main()
{
    CManagerLocalLight* storage[4] = { &g_lights[0], &g_lights[1], &g_lights[2], 0 };
    CArray<CManagerLocalLight*> a;
    a._M_start = storage; a._M_finish = storage + 3; a._M_end_of_storage = storage + 4;
    g_LocalLightHeap.AllocCount = 3; g_LocalLightHeap.RefCountReleaseEnabled = true; g_LocalLightHeap.FreeList = 0;
    a.DeleteAndClear();
    if (a._M_finish != a._M_start || a._M_end_of_storage != storage + 4 || a.size() != 0) { std::printf("00b49fe0_TEST FAIL\n"); return 1; }
    if (g_LocalLightHeap.AllocCount != 0 || g_released != 1) { std::printf("00b49fe0_TEST FAIL\n"); return 1; }
    CObjectEntry* e = g_LocalLightHeap.FreeList;
    if (e != (CObjectEntry*)&g_lights[2] || e->Next != (CObjectEntry*)&g_lights[1] || e->Next->Next != (CObjectEntry*)&g_lights[0] || e->Next->Next->Next != 0) { std::printf("00b49fe0_TEST FAIL\n"); return 1; }
    a._M_finish = storage + 1;                          // one more, release disabled
    storage[0] = &g_lights[1];
    g_LocalLightHeap.AllocCount = 1; g_LocalLightHeap.RefCountReleaseEnabled = false;
    a.DeleteAndClear();
    if (g_released != 1 || g_LocalLightHeap.AllocCount != 0 || g_LocalLightHeap.FreeList != (CObjectEntry*)&g_lights[1] || a.size() != 0) { std::printf("00b49fe0_TEST FAIL\n"); return 1; }
    a.DeleteAndClear();                                 // empty: nothing freed
    if (g_LocalLightHeap.AllocCount != 0 || a.size() != 0) { std::printf("00b49fe0_TEST FAIL\n"); return 1; }
    std::printf("00b49fe0_TEST PASS\n");
    return 0;
}