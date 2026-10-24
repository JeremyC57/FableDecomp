// behaviour test for 0x00AF6920 _STL::__uninitialized_copy over pair<EPSCType, CComponentInstance>: copy each entry (type, vtable, counted component with a reference bump, enabled flag, instance name)
#include <cstdio>
#include <new>
struct CCountBlock { unsigned int RefCount; };
struct CCountedPointer { int* Data; CCountBlock* Block;
    CCountedPointer() : Data(0), Block(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Block(o.Block) { if (Block) ++Block->RefCount; } };
struct CComponentInstance {
    virtual int Kind() const { return 1; }
    virtual ~CComponentInstance() { }
    CCountedPointer Component; bool Enabled; unsigned int InstanceName;
};
struct SPair { long first; CComponentInstance second; };
static SPair* UninitCopy(SPair* first, SPair* last, SPair* result)
{
    SPair* cur = result;
    for (; first != last; ++first, ++cur) new (cur) SPair(*first);
    return cur;
}
static double gStoreAligned[16 * 8];
static char* const gStore = (char*)gStoreAligned;
int main()
{

    CCountBlock blk = { 1 };
    int comp = 0;
    SPair src[2];
    src[0].first = 4; src[0].second.Component.Data = &comp; src[0].second.Component.Block = &blk; src[0].second.Enabled = true; src[0].second.InstanceName = 77;
    src[1].first = 6; src[1].second.Component.Data = 0; src[1].second.Component.Block = 0; src[1].second.Enabled = false; src[1].second.InstanceName = 78;
    SPair* raw = (SPair*)gStore;
    SPair* end = UninitCopy(src, src + 2, raw);
    if (end != raw + 2 || blk.RefCount != 2) { std::printf("00af6920_TEST FAIL\n"); return 1; }
    if (raw[0].first != 4 || raw[0].second.Component.Data != &comp || !raw[0].second.Enabled || raw[0].second.InstanceName != 77) { std::printf("00af6920_TEST FAIL\n"); return 1; }
    if (raw[1].first != 6 || raw[1].second.Component.Block != 0 || raw[1].second.Enabled || raw[1].second.InstanceName != 78) { std::printf("00af6920_TEST FAIL\n"); return 1; }
    if (raw[0].second.Kind() != 1) { std::printf("00af6920_TEST FAIL\n"); return 1; }                    // the copy carries the component-instance vtable
    std::printf("00af6920_TEST PASS\n");
    return 0;
}