// CEngineStaticMapFileBlockManager::ReleaseRequest(long block) -- find the block in Blocks and drop its RefCount
// Leaf: ?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE0400, length 27 bytes, `ret 4`.
// Callees: STLport _Rb_tree<long, pair<const long,CFileBlock> >::find<long> 0x00BE08E0 (out of line, hidden iterator result in the
//   one-word frame, the key by reference). Shape: `push ecx` frame, the two addresses pushed, `add ecx,8` (Blocks), the result read back
//   and `dec dword ptr [eax+14h]` (node value +0x10: key, CFileBlock::RefCount +0x14); no end() test.
// Naming (NF172, 2026-11-20, q4395): ported off the invented local CRetailBlockSet / SBlockNode_00be0400 models onto
//   engine/CEngineStaticMapFileBlockManager.h (Blocks = std::map<long,CFileBlock>, stlport/fable_map.h), so the find binds the STLport
//   member template ??$find@J@?$_Rb_tree@... (owner decision D1, 2026-11-03), the self row 0x00BE08E0's Leaf; was
//   ?FindInsertionPoint@CRetailBlockSet@@QAEXAAPAUSBlockNode_00be0400@@ABJ@Z. Own name: Ego_r.pdb + FableWin.pdb
//   ?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z (the row's existing spelling; Leaf line added).
#define FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_RELEASEREQUEST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL
#include "engine/CEngineStaticMapFileBlockManager.h"

void CEngineStaticMapFileBlockManager::ReleaseRequest(long block)
{
    --Blocks.find(block)->second.RefCount;
}
