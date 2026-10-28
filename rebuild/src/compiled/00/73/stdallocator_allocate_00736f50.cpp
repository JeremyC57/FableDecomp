// std::allocator::allocate(unsigned int,const void*) -- Allocate a nonempty range of elements.
// Leaf: ?allocate@?$allocator@VCUnprovenElement_00736f50@@@std@@QAEPAVCUnprovenElement_00736f50@@IPBX@Z
// Identity: descriptive. Own count zero guard, shift2, CRT malloc00bfea0e, return two stack args; receiver unused. No true callers: first raw xref is unrelated rel32; second matches ASCII Pos in data strings. STLport allocator::allocate count/hint is a shape-based hypothesis; exact element identity unknown. Only element extent4 is recovered, no invented semantic fields.
// Layout: original empty allocator template with descriptive element whose only modeled property is the proven extent. No engine header.
#include <stdlib.h>
class CUnprovenElement_00736f50{public:unsigned char UnmodeledBytes[4];};
namespace std{template<class T>class allocator{public:T* allocate(unsigned int n,const void*){return n?(T*)malloc(n*sizeof(T)):0;}};}
template class std::allocator<CUnprovenElement_00736f50>;