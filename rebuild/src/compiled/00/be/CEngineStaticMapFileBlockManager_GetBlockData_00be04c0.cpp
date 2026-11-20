// CEngineStaticMapFileBlockManager::GetBlockData(long block) const -- find the block in Blocks and return its loaded data
// Leaf: ?GetBlockData@CEngineStaticMapFileBlockManager@@QBEPBEJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE04C0, length 32 bytes, `ret 4`.
// Callees: the tree's const find 0x00BE0930 (out of line, hidden const_iterator result in the one-word frame, the key by reference),
//   CBankFileAsyncData::PeekData 0x009D5580 (const) on CFileBlock::LoadingData (node +0x1C), as a tail call. Shape: `push ecx` frame,
//   `add ecx,8`, the result read back for the tail call's `this`; no end() test.
// Naming (NF172, 2026-11-20, q11713): ported off the invented CRetailBlockSet / SBlockNode_00be0400 local models onto
//   engine/CEngineStaticMapFileBlockManager.h (NF172 opt-in GETBLOCKDATA_CONST) + stlport/fable_map.h; own leaf from the stand-in
//   ...@@QAEPAXJ@Z to Ego_r.pdb 0x00A41E20 / FableWin.pdb 0x02DF74B0 ?GetBlockData@CEngineStaticMapFileBlockManager@@QBEPBEJ@Z (const, const
//   unsigned char*), the spelling all 5 landed callers bind. The const find binds the 0x00BE0930 self row's Leaf (the non-template
//   ?find@...QBE...; was ?Find@CRetailBlockSet@@...): the D1 member-template respell of 0x00BE0930 is a family item (NAME_FIXES).
#define FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_GETBLOCKDATA_CONST
#define FABLE_RETAIL_CBANKFILEASYNCDATA_PEEKDATA
#define FABLE_RETAIL_STLP_MAP
// Callee respelled (NF177, PRIORITY q12088): 0x00BE0930 was the non-template ?find@?$_Rb_tree@J...QBE; owner decision D1 (STLport 4.6.1 _tree.h
//   member-template find) gives ??$find@J@?$_Rb_tree@J...QBE, the self row's spelling.
#define FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE
#include "engine/CBankFileAsyncData.h"
#include "engine/CEngineStaticMapFileBlockManager.h"

const unsigned char* CEngineStaticMapFileBlockManager::GetBlockData(long block) const
{
    return (const unsigned char*)Blocks.find(block)->second.LoadingData->PeekData();
}
