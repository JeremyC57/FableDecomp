// CPersistContext::TransferReactionMatchList_0067bfb0(std::list<CReactionMatch*>* value) -- CheckCRC(""), then with a CPersistTraits<list<CReactionMatch*>> helper (which owns an empty scratch list) write the list in mode 3 (save) or read it in mode 2 (load)
// Leaf: TransferReactionMatchList_0067bfb0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067BFB0, length 108 bytes (padded to 112).
// Callees: CPersistContext::CheckCRC @ 0x00404500 (the empty key @ 0x0122D70E), malloc @ 0x00BFEA0E (the scratch list's
//          head node), CPersistTraits<list<CReactionMatch*>>::TransferBinaryOut @ 0x00678250 / TransferBinaryIn @ 0x006782B0,
//          list<CReactionMatch*>::~list @ 0x004AB420 (manifest "CGuiControlTreePane::SortTreeRecursively" is propagated:
//          the body frees every node and then the head).
// Identity: the catalog label "COpinionReactionManagerDef::Transfer" belongs to the caller (0x006798E6 calls this);
// `this` here is the persist context (CheckCRC on ecx, mode +0x18, load stream +0x24, save stream +0x28), the same
// shape as the landed CPersistContext::TransferFloatRange_00458f2b. The list value type is fixed by the traits callees.
// Shape: the traits local is one frame word (an STLport list: just the head-node pointer, allocator folded) built by
// an inline constructor (malloc'd self-linked head) before the mode ladder; its destructor is the out-of-line ~list.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

struct CReactionMatch;
struct CDataOutputStream;
struct CMemoryDataInputStream;

struct SReactionMatchNode {
    SReactionMatchNode* m_next;
    SReactionMatchNode* m_prev;
    CReactionMatch*     m_data;
};

struct CReactionMatchList {                         // STLport list<CReactionMatch*> (4 bytes: the head node)
    SReactionMatchNode* m_node;
    CReactionMatchList()
    {
        SReactionMatchNode* head = (SReactionMatchNode*)malloc(sizeof(SReactionMatchNode));
        head->m_next = head;
        head->m_prev = head;
        m_node = head;
    }
    ~CReactionMatchList();                          // 0x004AB420
};

struct CPersistTraitsReactionMatchList {            // CPersistTraits<list<CReactionMatch*>>
    CReactionMatchList m_scratch;
    void TransferBinaryOut(CDataOutputStream* stream, CReactionMatchList* value);    // 0x00678250
    void TransferBinaryIn(CMemoryDataInputStream* stream, CReactionMatchList* value); // 0x006782B0
};

struct CPersistContext {
    unsigned char           _pad0[0x18];
    long                    m_mode;                 // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1[8];
    CMemoryDataInputStream* m_pLoadStream;          // +0x24
    CDataOutputStream*      m_pSaveStream;          // +0x28
    void CheckCRC(const char* name);                // 0x00404500
    void TransferReactionMatchList_0067bfb0(CReactionMatchList* value);
};

void CPersistContext::TransferReactionMatchList_0067bfb0(CReactionMatchList* value)
{
    CheckCRC("");
    CPersistTraitsReactionMatchList traits;
    switch (m_mode) {
    case 3:
        traits.TransferBinaryOut(m_pSaveStream, value);
        break;
    case 2:
        traits.TransferBinaryIn(m_pLoadStream, value);
        break;
    }
}