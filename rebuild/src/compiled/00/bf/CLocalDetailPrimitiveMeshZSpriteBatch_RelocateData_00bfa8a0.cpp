// CLocalDetailPrimitiveMeshZSpriteBatch::RelocateData(void* block, char* oldBase, char* newBase, unsigned long size) -- after the landscape memory manager moved a block: rebase whichever of the batch's two block pointers (+0x38 vertex, else +0x3c index) lay inside the moved range, then re-send the batch's primitive description to the renderer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFA8A0, length 128 bytes (CMovableResource vtable slot 1; the catalog
//         name is the slot, "vfunc_1").
// Leaf: RelocateData
// Callees: CLocalDetailPrimitiveMeshZSpriteBatch::SetupPrimitiveDesc() @ 0x00BF9FA0 (a 0x50-byte CAddPrimitiveDesc
//          returned by hidden pointer), the engine's primitive update @ 0x00B32000 (engine 0x01436E80, `this` of a
//          two-argument thiscall: the handle at +0x48 and the description).
// Shape: an inline range test (null, `>= old`, `< old + size`, unsigned) and an inline rebase that tests for null
// again (`p - old + new`); the second pointer is tried only when the first was not in range. The block argument
// is unused.
struct CAddPrimitiveDesc { unsigned char _body[0x50]; };
class CEnginePrimitiveHandle { public: void* __vftable; void* Data; };
struct CEngine_01436e80 {
    void UpdatePrimitive(CEnginePrimitiveHandle* handle, const CAddPrimitiveDesc& desc);   // 0x00B32000
};
extern CEngine_01436e80* g_pEngine_01436e80;      // 0x01436E80
inline bool IsInBlock(const char* p, const char* base, unsigned long size)
{
    return p && p >= base && p < base + size;
}
inline void Rebase(char*& p, const char* oldBase, char* newBase)
{
    if (p)
        p = p - oldBase + newBase;
}
struct CLocalDetailPrimitiveMeshZSpriteBatch {
    virtual void v00();
    virtual void RelocateData(void* block, char* oldBase, char* newBase, unsigned long size);
    unsigned char          _pad04[0x34];
    char*                  m_pVertexBlock;         // +0x38
    char*                  m_pIndexBlock;          // +0x3c
    unsigned char          _pad40[8];
    CEnginePrimitiveHandle m_handle;               // +0x48
    CAddPrimitiveDesc SetupPrimitiveDesc();        // 0x00BF9FA0
};

void CLocalDetailPrimitiveMeshZSpriteBatch::RelocateData(void* block, char* oldBase, char* newBase, unsigned long size)
{
    if (IsInBlock(m_pVertexBlock, oldBase, size))
        Rebase(m_pVertexBlock, oldBase, newBase);
    else if (IsInBlock(m_pIndexBlock, oldBase, size))
        Rebase(m_pIndexBlock, oldBase, newBase);
    g_pEngine_01436e80->UpdatePrimitive(&m_handle, SetupPrimitiveDesc());
}