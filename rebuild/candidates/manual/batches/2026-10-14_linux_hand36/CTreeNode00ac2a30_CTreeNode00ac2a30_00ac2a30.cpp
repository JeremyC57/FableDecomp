// CTreeNode_00ac2a30::~CTreeNode_00ac2a30() -- delete the two child nodes at +0x3c and +0x40 (the same destructor recursively, then operator delete), then free the buffer at +0x30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC2A30, length 69 bytes.
// Callees: itself @ 0x00AC2A30 twice (the children's destructor), operator delete @ 0x00BFE9BC twice, free @ 0x00BFEA14. Shape: each `delete` is `test ; je ; mov ecx ;
// call ~ ; push ; call delete ; add esp,4` with the child in edi; the buffer pointer is loaded into esi last. The chunk label is propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CTreeNode_00ac2a30 {
    unsigned char     _pad[0x30];
    void*             m_pBuffer;                   // +0x30
    unsigned char     _pad34[8];
    CTreeNode_00ac2a30* m_pLeft;                   // +0x3c
    CTreeNode_00ac2a30* m_pRight;                  // +0x40
    ~CTreeNode_00ac2a30();
};

CTreeNode_00ac2a30::~CTreeNode_00ac2a30()
{
    delete m_pLeft;
    delete m_pRight;
    if (m_pBuffer)
        free(m_pBuffer);
}
