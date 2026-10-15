// CBankFile::FindIndexBySymbol(const CCharString&) const -- the bank entry index registered for the symbol, or 0 when the symbol is unknown
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CCDC0, length 35 bytes.
// Callees: map<CCharString, unsigned long>::find @ 0x009D2DD0 (`__fastcall`, the node back in eax).
// The manifest label "CQuestManager::OnQuestStartScreenDisplayed" is propagated and rejected (that method returns
// bool): the body sits among the CBankFile methods (0x009CCB20 MakeWritable .. 0x009CD140 FindIndexByPathname),
// is called as "the bank index by name" by the landed C3DMeshStats::InitialiseMeshGenerators (0x00A2EC40), and
// FableWin.pdb names ?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z. PDB CBankFile::SymbolIndices
// (map<CCharString, unsigned long>) is at +0xbc; the four CArrays before it are 12 bytes in the shipped build, so
// +0xac here. The tree node holds the key at +0x10 and the index at +0x14; the map's head node is its first word.
// Shape: the map's address is a named pointer (esi) so `end()` is re-read through it after the call; the found
// return is the `if` body (the `return 0` tail comes last).
struct CCharString {
    void* PStringData;
};

struct CSymbolIndexMap {                           // map<CCharString, unsigned long>: head node first
    struct SNode {
        SNode*        m_pLeft;
        SNode*        m_pParent;
        SNode*        m_pRight;
        unsigned long m_color;
        CCharString   m_key;                       // +0x10
        unsigned long m_index;                     // +0x14
    };
    SNode* m_pHead;
    unsigned long m_size;
    SNode* find(const CCharString& key) const;     // 0x009D2DD0
};

struct CBankFile {
    unsigned char   _pad_0x00[0xac];
    CSymbolIndexMap SymbolIndices;                 // +0xac
    unsigned long FindIndexBySymbol(const CCharString& symbol) const;
};

unsigned long CBankFile::FindIndexBySymbol(const CCharString& symbol) const
{
    const CSymbolIndexMap* indices = &SymbolIndices;
    CSymbolIndexMap::SNode* it = indices->find(symbol);
    if (it != indices->m_pHead)
        return it->m_index;
    return 0;
}