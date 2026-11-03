#ifndef FABLE_ENGINE_CVERTEXGROUPBLOCK_H
#define FABLE_ENGINE_CVERTEXGROUPBLOCK_H
// New header (chunk 725b): CVertexGroupBlock, retail sizeof 0x18: Groups (CArray<unsigned long>, STLport vector 0xC) +0,
// GroupToIndexMap (std::map<unsigned long,long>, STLport tree 0xC) +0xC (Ego_r 0x20 with Dinkumware containers). Its
// constructor ??0CVertexGroupBlock@@QAE@XZ 0x00AD9030 and destructor ??1CVertexGroupBlock@@QAE@XZ 0x00A96DA0 are out of line
// (reloc_bindings), so the members stay opaque here.
#include "rebuild_abi.h"
class CVertexGroupBlock {
public:
    CVertexGroupBlock();               // 0x00AD9030
    ~CVertexGroupBlock();              // 0x00A96DA0
private:
    unsigned char Groups[0xc];         // CArray<unsigned long> +0x00
    unsigned char GroupToIndexMap[0xc];// std::map<unsigned long,long> +0x0C
};
FABLE_STATIC_ASSERT(sizeof(CVertexGroupBlock) == 0x18);
#endif
