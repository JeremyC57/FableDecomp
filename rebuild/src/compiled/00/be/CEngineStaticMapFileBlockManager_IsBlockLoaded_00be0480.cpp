// CEngineStaticMapFileBlockManager::IsBlockLoaded(long block) const -- find the block in Blocks; false when absent, else whether its async data has finished
// Leaf: ?IsBlockLoaded@CEngineStaticMapFileBlockManager@@QBE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE0480, length 52 bytes.
// Callees: the const member-template find of Blocks (+8) @ 0x00BE0930 (hidden const_iterator result in the one-word frame, the key by
// reference), CBankFileAsyncData::IsFinished @ 0x009D54E0 (on the found block's LoadingData; a bool in al). Shape: the map address is formed
// with `lea esi,[ecx+8]` and its header word re-read for the end() compare; `xor al,al` for the miss.
// Naming (NF177, PRIORITY q12088): ported off the row-local CRetailBlockSet / SBlockNode_00be0400 stand-ins (callee was
//   ?Find@CRetailBlockSet@@QAEXAAPAUSBlockNode_00be0400@@ABJ@Z) onto engine/CEngineStaticMapFileBlockManager.h (Blocks is
//   std::map<long,CFileBlock>): the callee is now owner decision D1's ??$find@J@?$_Rb_tree@J...QBE (FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE, as
//   0x00BE04C0), and the self leaf the Ego_r.pdb 0x00A41DE0 / FableWin.pdb 0x02DF7220 const spelling (was QAE; the landed referencers
//   0x00BDE390 / 0x00BDE860 already bind QBE, 0x00BDC180 / 0x00BF8030 / 0x00BF84C0 move with it in this hand-back).
#define FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_ISBLOCKLOADED
#define FABLE_RETAIL_CBANKFILEASYNCDATA_WATER
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE
#include "engine/CBankFileAsyncData.h"
#include "engine/CEngineStaticMapFileBlockManager.h"

bool CEngineStaticMapFileBlockManager::IsBlockLoaded(long block) const
{
    std::map<long,CFileBlock>::const_iterator it = Blocks.find(block);
    if (it == Blocks.end())
        return false;
    return it->second.LoadingData->IsFinished();
}
