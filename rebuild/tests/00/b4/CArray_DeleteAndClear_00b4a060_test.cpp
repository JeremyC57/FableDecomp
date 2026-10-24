#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
// Behaviour test for CArray<CEngineLightingManager::CManagerShadowedSpotLight*>::DeleteAndClear() @ 0x00B4A060: every pointer
// goes back on the shadowed-spot-light heap's free list (most recent first), the heap's last free runs RefCountedRelease when
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
static CDynamicObjectHeap g_ShadowedSpotLightHeap;
struct CManagerShadowedSpotLight {
    void* Payload[24];
    static void __fastcall operator delete(void* object) { g_ShadowedSpotLightHeap.Free(object); }
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
static CManagerShadowedSpotLight g_lights[3];
int main()
{
    CManagerShadowedSpotLight* storage[4] = { &g_lights[0], &g_lights[1], &g_lights[2], 0 };
    CArray<CManagerShadowedSpotLight*> a;
    a._M_start = storage; a._M_finish = storage + 3; a._M_end_of_storage = storage + 4;
    g_ShadowedSpotLightHeap.AllocCount = 3; g_ShadowedSpotLightHeap.RefCountReleaseEnabled = true; g_ShadowedSpotLightHeap.FreeList = 0;
    a.DeleteAndClear();
    if (a._M_finish != a._M_start || a._M_end_of_storage != storage + 4 || a.size() != 0) { std::printf("00b4a060_TEST FAIL\n"); return 1; }
    if (g_ShadowedSpotLightHeap.AllocCount != 0 || g_released != 1) { std::printf("00b4a060_TEST FAIL\n"); return 1; }
    CObjectEntry* e = g_ShadowedSpotLightHeap.FreeList;
    if (e != (CObjectEntry*)&g_lights[2] || e->Next != (CObjectEntry*)&g_lights[1] || e->Next->Next != (CObjectEntry*)&g_lights[0] || e->Next->Next->Next != 0) { std::printf("00b4a060_TEST FAIL\n"); return 1; }
    a._M_finish = storage + 1;                          // one more, release disabled
    storage[0] = &g_lights[1];
    g_ShadowedSpotLightHeap.AllocCount = 1; g_ShadowedSpotLightHeap.RefCountReleaseEnabled = false;
    a.DeleteAndClear();
    if (g_released != 1 || g_ShadowedSpotLightHeap.AllocCount != 0 || g_ShadowedSpotLightHeap.FreeList != (CObjectEntry*)&g_lights[1] || a.size() != 0) { std::printf("00b4a060_TEST FAIL\n"); return 1; }
    a.DeleteAndClear();                                 // empty: nothing freed
    if (g_ShadowedSpotLightHeap.AllocCount != 0 || a.size() != 0) { std::printf("00b4a060_TEST FAIL\n"); return 1; }
    std::printf("00b4a060_TEST PASS\n");
    return 0;
}