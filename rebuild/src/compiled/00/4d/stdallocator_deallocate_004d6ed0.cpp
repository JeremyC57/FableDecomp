// std::allocator::deallocate(pointer,unsigned int) -- Release non-null allocated storage.
// Leaf: ?deallocate@?$allocator@VCUnprovenElement_004d6ed0@@@std@@QAEXPAVCUnprovenElement_004d6ed0@@I@Z
// Flags: /O1
// Identity: descriptive. No true callers: raw absolute cross-reference falls inside an unrelated rel32 call displacement. Body ignores receiver/size, conditionally calls CRT free00bfea14, pops two stack parameters. STLport allocator::deallocate is a shape-based hypothesis; actual element and owning instantiation cannot be recovered. Forward-declared descriptive element avoids claiming a record size or identity. ABI evidence cannot distinguish equivalent stdcall helper.
// Layout: empty original allocator template; pointee is incomplete and never accessed. No engine receiver header.
#include <stdlib.h>
class CUnprovenElement_004d6ed0;
namespace std{template<class T>class allocator{public:void deallocate(T* memory,unsigned int){if(memory)free(memory);}};}
template class std::allocator<CUnprovenElement_004d6ed0>;