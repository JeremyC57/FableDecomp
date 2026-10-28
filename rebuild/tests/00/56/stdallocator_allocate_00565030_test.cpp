#include <stdio.h>
#include <stdlib.h>
static size_t requested;static int calls;static bool failAllocation;static unsigned char storage[256];static void* TestMalloc(size_t n){requested=n;++calls;return failAllocation?0:storage;}
#define malloc TestMalloc
// std::allocator::allocate(unsigned int,const void*) -- Allocate a nonempty range of elements.
// Leaf: ?allocate@?$allocator@VCUnprovenElement_00565030@@@std@@QAEPAVCUnprovenElement_00565030@@IPBX@Z
// Identity: descriptive. Own count zero guard, shift3, CRT malloc00bfea0e, return two stack args; receiver unused. No true callers: first raw xref is unrelated rel32; second matches ASCII Pos in data strings. STLport allocator::allocate count/hint is a shape-based hypothesis; exact element identity unknown. Only element extent8 is recovered, no invented semantic fields.
// Layout: original empty allocator template with descriptive element whose only modeled property is the proven extent. No engine header.
#include <stdlib.h>
class CUnprovenElement_00565030{public:unsigned char UnmodeledBytes[8];};
namespace std{template<class T>class allocator{public:T* allocate(unsigned int n,const void*){return n?(T*)malloc(n*sizeof(T)):0;}};}
template class std::allocator<CUnprovenElement_00565030>;

#undef malloc
int main(){std::allocator<CUnprovenElement_00565030> a;bool ok=a.allocate(0,0)==0&&calls==0;CUnprovenElement_00565030* p=a.allocate(3,0);ok=ok&&(void*)p==storage&&calls==1&&requested==24;failAllocation=true;ok=ok&&a.allocate(2,storage)==0&&calls==2&&requested==16;failAllocation=false;p=a.allocate(1,storage);ok=ok&&(void*)p==storage&&calls==3&&requested==8;puts(ok?"00565030_TEST PASS":"00565030_TEST FAIL");return ok?0:1;}