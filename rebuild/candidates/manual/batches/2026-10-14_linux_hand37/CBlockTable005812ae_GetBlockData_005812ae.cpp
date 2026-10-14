#pragma optimize("y", off)
#pragma optimize("s", on)
// CBlockTable_005812ae::GetBlockData(long bank, long block) -- look the (bank, block) key up in the tree at +0x18 (the found position in a 16-byte local), then recalculate and return the result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005812AE, length 49 bytes.
// Callees: the tree's find @ 0x00580D21 (on +0x18: the 16-byte result by address, the 8-byte key by address; the manifest label "BinarySearchTree_FindOrInsertPos"),
// the recalculation @ 0x00800020 (on `this`; its result returned; the label "CAnimation_RecalculateScale" is propagated). Shape: a frame pointer (the 0x0058xxxx
// module) with the key pair built in the frame from the two arguments; `leave ; ret 8`. The chunk label "CEngineStaticMapFileBlockManager::GetBlockData" is
// propagated; the class is named by address.
struct SBlockKey_005812ae { long m_bank; long m_block; };
struct SFindResult_005812ae { unsigned char _body[8]; };

struct CRetailTree_005812ae {
    void Find(SFindResult_005812ae& result, const SBlockKey_005812ae& key);   // 0x00580D21
};

struct CBlockTable_005812ae {
    unsigned char        _pad[0x18];
    CRetailTree_005812ae m_tree;                   // +0x18
    void* Recalculate();                           // 0x00800020
    void* GetBlockData(long bank, long block);
};

void* CBlockTable_005812ae::GetBlockData(long bank, long block)
{
    SBlockKey_005812ae key;
    key.m_bank = bank;
    key.m_block = block;
    SFindResult_005812ae result;
    m_tree.Find(result, key);
    return Recalculate();
}
