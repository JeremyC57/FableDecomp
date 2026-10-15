#include <cstdio>

struct CResourceList { int id; };

struct CIVCountedPointeeBase {
    long RefCount;
    CIVCountedPointeeBase() : RefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual int Kind() const { return 1; }
};

struct CResource : CIVCountedPointeeBase {
    CResourceList* ResourceList; CResource* PrevResource; CResource* NextResource; unsigned long ResourceSize; unsigned long LastUsedFrame;
    CResource() : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0) { }
    virtual int Kind() const { return 2; }
};

int main()
{
    CResourceList list; CResource a; a.RefCount = 7; a.ResourceList = &list; a.PrevResource = &a; a.NextResource = &a;
    a.ResourceSize = 0x40; a.LastUsedFrame = 99;
    CResource b(a);                                   // the implicit copy constructor: every word, own vtable
    const CIVCountedPointeeBase& base = b;
    if (base.Kind() != 2 || b.RefCount != 7 || b.ResourceList != &list || b.PrevResource != &a || b.NextResource != &a
        || b.ResourceSize != 0x40 || b.LastUsedFrame != 99)
    { std::printf("0046649a_TEST FAIL\n"); return 1; }
    std::printf("0046649a_TEST PASS\n");
    return 0;
}
