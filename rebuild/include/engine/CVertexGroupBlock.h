#ifndef FABLE_ENGINE_CVERTEXGROUPBLOCK_H
#define FABLE_ENGINE_CVERTEXGROUPBLOCK_H
// New header (chunk 725b): CVertexGroupBlock, retail sizeof 0x18: Groups (CArray<unsigned long>, STLport vector 0xC) +0,
// GroupToIndexMap (std::map<unsigned long,long>, STLport tree 0xC) +0xC (Ego_r 0x20 with Dinkumware containers). Its
// constructor ??0CVertexGroupBlock@@QAE@XZ 0x00AD9030 and destructor ??1CVertexGroupBlock@@QAE@XZ 0x00A96DA0 are out of line
// (reloc_bindings), so the members stay opaque here.
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#endif
class CVertexGroupBlock {
public:
    CVertexGroupBlock();               // 0x00AD9030
    ~CVertexGroupBlock();              // 0x00A96DA0
#ifdef FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
    // Retail opt-in (chunk F1, row 0x00AD9770 CVertexGroupBlockList::RemoveAllSubsets): the Ego_r.pdb members with their real types
    // (the including row picks how their copy constructors are emitted), and FableWin.pdb / Ego_r.pdb
    // ?IsProperSubsetOf@CVertexGroupBlock@@QBE_NABV1@@Z (landed 0x00AD8CA0). Same layout as the opaque default.
    bool IsProperSubsetOf(const CVertexGroupBlock& other) const;   // 0x00AD8CA0
#ifdef FABLE_RETAIL_CVERTEXGROUPBLOCK_REMOVELAST
    // Per-TU opt-in (wf20, row 0x00AD9F10): FableWin.pdb / Ego_r.pdb ?RemoveLast@CVertexGroupBlock@@QAEXK@Z (landed 0x00AD9470).
    void RemoveLast(unsigned long count);   // 0x00AD9470
#endif
#ifdef FABLE_RETAIL_CVERTEXGROUPBLOCK_ADDGROUP
    // Per-TU opt-in (wf20 attempt 3, row 0x00AD9F10): ?AddGroup@CVertexGroupBlock@@QAEXK@Z (landed 0x00AD91F0), called by InsertBlock.
    void AddGroup(unsigned long group);     // 0x00AD91F0
#endif
    // FableWin.pdb ?GetGroupCount@CVertexGroupBlock@@QBEKXZ, ?GetGroup@CVertexGroupBlock@@QBEKK@Z, ?HasGroup@CVertexGroupBlock@@QBE_NK@Z
    // (inline in retail: IsProperSubsetOf 0x00AD8CA0 expands them around the map find call 0x006699C0).
    unsigned long GetGroupCount() const { return Groups.size(); }
    unsigned long GetGroup(unsigned long index) const { return Groups[index]; }
    bool HasGroup(unsigned long group) const { return GroupToIndexMap.find(group)!=GroupToIndexMap.end(); }
private:
    CArray<unsigned long> Groups;                   // +0x00
    std::map<unsigned long,long> GroupToIndexMap;   // +0x0C
};
#else
private:
    unsigned char Groups[0xc];         // CArray<unsigned long> +0x00
    unsigned char GroupToIndexMap[0xc];// std::map<unsigned long,long> +0x0C
};
#endif
FABLE_STATIC_ASSERT(sizeof(CVertexGroupBlock) == 0x18);
#endif
