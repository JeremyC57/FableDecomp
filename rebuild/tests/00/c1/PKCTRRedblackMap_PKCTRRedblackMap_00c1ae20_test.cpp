// Behaviour test for 00c1ae20: PKCTRRedblackMap constructor -- empty tree over the sentinel, allocator kept, sentinel key/value untouched
#include <cstdio>
#include <new>
struct CMapNodeAllocator { virtual void Free(void*) { } virtual void* Allocate() { return 0; } virtual ~CMapNodeAllocator() { } };
struct PKCTRRedblackNode { PKCTRRedblackNode* Left; PKCTRRedblackNode* Right; PKCTRRedblackNode* Parent; long Colour; unsigned long Key; void* Value; };
struct PKCTRRedblackMap {
    CMapNodeAllocator* PAllocator; PKCTRRedblackNode Sentinel; PKCTRRedblackNode* Root; PKCTRRedblackNode* LastFound;
    PKCTRRedblackMap(CMapNodeAllocator* allocator) : PAllocator(allocator)
    { Root = &Sentinel; LastFound = &Sentinel; Sentinel.Left = &Sentinel; Sentinel.Right = &Sentinel; Sentinel.Parent = 0; Sentinel.Colour = 0; }
};
static bool Check()
{
    static unsigned char buf[sizeof(PKCTRRedblackMap)];
    for (unsigned int i = 0; i < sizeof(buf); ++i) buf[i] = 0x5a;
    CMapNodeAllocator a;
    PKCTRRedblackMap* m = new (buf) PKCTRRedblackMap(&a);
    return m->PAllocator == &a && m->Root == &m->Sentinel && m->LastFound == &m->Sentinel && m->Sentinel.Left == &m->Sentinel
        && m->Sentinel.Right == &m->Sentinel && m->Sentinel.Parent == 0 && m->Sentinel.Colour == 0 && m->Sentinel.Key == 0x5a5a5a5a
        && sizeof(PKCTRRedblackMap) == 0x24;
}
int main()
{
    if (!Check()) { std::printf("00c1ae20_TEST FAIL\n"); return 1; }
    std::printf("00c1ae20_TEST PASS\n");
    return 0;
}
