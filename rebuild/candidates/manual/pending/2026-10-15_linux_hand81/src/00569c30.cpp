// CBlockVectorOwner_00569c30::~CBlockVectorOwner_00569c30() -- run the owner's Clear() (0x0083FBC0), free the buffer at +0xc, then destroy the block vector at +0 by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00569C30, length 32 bytes.
// Callees: Clear @ 0x0083FBC0 (thiscall on the whole object: it erases the vector at +0 and resets
// the container at +0xc; manifest label "SetStaticCast<NUISystem::CComponent>" is propagated),
// free @ 0x00BFEA14 (explicit null test, `add esp,4`: a speed TU), the out-of-line vector
// destructor @ 0x00569940 (frees each 12-byte element's block, then the buffer; landed as
// CRetailVector_00569940::Destroy) reached by tail jump with ecx = this.
// The manifest label "CTCAssassinRush::FinishMove" is propagated over many byte-different bodies;
// nothing references this body by call or vtable, so the class is named by what it owns.
// Shape: no vtable store anywhere = a non-polymorphic class; the user destructor body is the one
// Clear() call, then the members are destroyed in reverse declaration order: the buffer at +0xc
// (inline `if (p) free(p)` destructor) and the vector at +0 (its out-of-line destructor becomes
// the sibling-call `jmp` because ecx already holds `this`).
// Leaf: ??1CBlockVectorOwner_00569c30
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

struct SOwnedBlock {                                        // 12-byte element owning a block at +0
    void*         m_pData;
    unsigned char _rest[0x8];
};

struct CBlockVector {                                       // first, last, end
    SOwnedBlock* m_pFirst;
    SOwnedBlock* m_pLast;
    SOwnedBlock* m_pEnd;
    ~CBlockVector();                                        // 0x00569940
};

struct CRawBuffer {                                         // first, last, end of a POD array
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    ~CRawBuffer()
    {
        if (m_pFirst != 0)
            free(m_pFirst);
    }
};

struct CBlockVectorOwner_00569c30 {
    CBlockVector m_blocks;                                  // +0x00
    CRawBuffer   m_buffer;                                  // +0x0c
    void Clear();                                           // 0x0083FBC0
    ~CBlockVectorOwner_00569c30();
};

CBlockVectorOwner_00569c30::~CBlockVectorOwner_00569c30()
{
    Clear();
}
