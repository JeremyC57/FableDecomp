// Behaviour test for 00c1b330: CMapNodeAllocator::Allocate -- one 0x18-byte node block
#include <cstdio>
#include <new>
struct PKCTRRedblackNode { PKCTRRedblackNode* Left; PKCTRRedblackNode* Right; PKCTRRedblackNode* Parent; long Colour; unsigned long Key; void* Value; };
struct CMapNodeAllocator { virtual void* Allocate() { return ::operator new(sizeof(PKCTRRedblackNode)); } virtual ~CMapNodeAllocator() { } };
static bool Check() { CMapNodeAllocator a; void* p = a.Allocate(); bool ok = p != 0 && sizeof(PKCTRRedblackNode) == 0x18; ::operator delete(p); return ok; }
int main()
{
    if (!Check()) { std::printf("00c1b330_TEST FAIL\n"); return 1; }
    std::printf("00c1b330_TEST PASS\n");
    return 0;
}
