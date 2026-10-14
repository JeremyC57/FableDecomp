#include <cstdio>
struct CResourceList;
static int g_removed = 0, g_loading = -1; static void* g_insertPrev = 0; static void* g_insertNext = 0;
struct CResource { virtual void v00() { } unsigned long m_pad4; CResourceList* ResourceList; CResource* PrevResource; CResource* NextResource; unsigned long ResourceSize; unsigned long LastUsedFrame;
    void ListRemove() { ++g_removed; } void ListInsert(CResource* p, CResource* n) { g_insertPrev = p; g_insertNext = n; } };
struct CResourceList { long m_pad0; CResource Head; unsigned long ResourceCount; long AllocatedMemory; long MaximumMemory; unsigned long CurrentFrame; };
struct CAnimationData : CResource { unsigned char Animations[0xc]; void* SharedAnimation; };
struct C3DAnimationInfo { virtual void v00() { } void* Bank; unsigned long Index; CAnimationData* Data;
    void BeginLoading(bool w) { g_loading = w ? 1 : 0; }
    void TouchAnimation()
    {
        CAnimationData* data = Data;
        if (data == 0 || data->SharedAnimation == 0) { BeginLoading(false); return; }
        if (data->ResourceList == 0) return;
        if (data->LastUsedFrame == data->ResourceList->CurrentFrame) return;
        data->ListRemove();
        data->ListInsert(data->ResourceList->Head.PrevResource, &data->ResourceList->Head);
        data->LastUsedFrame = data->ResourceList->CurrentFrame;
    } };
int main()
{
    CResourceList list; CResource tail; list.Head.PrevResource = &tail; list.CurrentFrame = 42;
    CAnimationData d; d.ResourceList = &list; d.LastUsedFrame = 41; d.SharedAnimation = (void*)1;
    C3DAnimationInfo i; i.Data = &d;
    i.TouchAnimation();
    if (g_removed != 1 || g_insertPrev != &tail || g_insertNext != &list.Head || d.LastUsedFrame != 42 || g_loading != -1) { std::printf("00a26a50_TEST FAIL\n"); return 1; }
    i.TouchAnimation();   // already touched this frame
    if (g_removed != 1) { std::printf("00a26a50_TEST FAIL\n"); return 1; }
    d.ResourceList = 0; d.LastUsedFrame = 0; i.TouchAnimation(); if (g_removed != 1 || g_loading != -1) { std::printf("00a26a50_TEST FAIL\n"); return 1; }
    d.SharedAnimation = 0; i.TouchAnimation(); if (g_loading != 0) { std::printf("00a26a50_TEST FAIL\n"); return 1; }
    g_loading = -1; i.Data = 0; i.TouchAnimation(); if (g_loading != 0) { std::printf("00a26a50_TEST FAIL\n"); return 1; }
    std::printf("00a26a50_TEST PASS\n"); return 0;
}