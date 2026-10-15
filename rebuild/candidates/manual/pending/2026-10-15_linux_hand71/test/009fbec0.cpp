#include <cstdio>

struct CResourceList;

struct CIVCountedPointeeBase {
    long RefCount;
    CIVCountedPointeeBase() : RefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
};

struct CResource : CIVCountedPointeeBase {
    CResourceList* ResourceList; CResource* PrevResource; CResource* NextResource; unsigned long ResourceSize; unsigned long LastUsedFrame;
    CResource() : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0) { }
};

int main()
{
    CResource r;
    if (r.RefCount != 1 || r.ResourceList || r.PrevResource || r.NextResource || r.ResourceSize || r.LastUsedFrame)
    { std::printf("009fbec0_TEST FAIL\n"); return 1; }
    std::printf("009fbec0_TEST PASS\n");
    return 0;
}
