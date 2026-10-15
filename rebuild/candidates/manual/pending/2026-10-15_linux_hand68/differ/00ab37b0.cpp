// CVertexStreamFormat::CalculateVertexSize() const -- the vertex size: the sum of the data-type sizes of the stream's data elements
// Grade: DIFFER(44v58) -- the lane reverses the loop counter (mov edi,eax ... dec edi ; jne) where retail keeps the index in esi against a size re-derived in the preheader (mov edi,[ecx+8] ; mov eax,edx ; sub edi,eax ; sar edi,3)
// Leaf: CalculateVertexSize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB37B0, length 58 bytes.
// Callees: CVertexStreamFormat::GetDataTypeSize(EVertexStreamDataType) @ 0x00AB3740 (static `__fastcall`; its
// jump-table body is reproduced here so the compiler knows it preserves edx and writes no memory -- retail keeps
// the element pointer in edx across the call). Ego_r.pdb ?CalculateVertexSize@CVertexStreamFormat@@QBEKXZ.
// Identity: the manifest label CPlayerManager::PrepareForLoad is propagated; the callers are in
// ConvertVertexStream_AnimatedToFormat 0x00A90FC0.
// Layout: vptr, then the retail 12-byte vector<CDataElement> (8-byte elements {usage, type}) at +4.
// Tried: unsigned index (this file), signed `long` index (44, `jle`), the callee defined after the caller (44).
// Next: lane gap -- loop-counter reversal and preheader CSE of the size (QFE vs RTM optimiser).
enum EVertexStreamDataType { VSDT_0 = 0 };

struct CVertexStreamFormat {
    struct CDataElement {                                   // 8 bytes
        unsigned long         Usage;
        EVertexStreamDataType Type;
    };
    virtual void v00();
    CDataElement* m_pFirst;                                 // +0x04 retail std::vector<CDataElement> (12 bytes)
    CDataElement* m_pLast;                                  // +0x08
    CDataElement* m_pEnd;                                   // +0x0c
    unsigned int size() const { return m_pLast - m_pFirst; }
    static unsigned long __fastcall GetDataTypeSize(EVertexStreamDataType type);   // 0x00AB3740
    unsigned long CalculateVertexSize() const;
};

__declspec(noinline) unsigned long __fastcall CVertexStreamFormat::GetDataTypeSize(EVertexStreamDataType type)
{
    switch (type) {
    case 0: case 4: case 5: case 7: case 9: case 12: case 13: return 4;
    case 1: case 6: case 11: return 8;
    case 2: return 12;
    case 3: return 16;
    case 8: return 2;
    case 10: return 6;
    }
    return 0;
}

unsigned long CVertexStreamFormat::CalculateVertexSize() const
{
    unsigned long size = 0;
    for (unsigned int i = 0; i < this->size(); ++i)
        size += GetDataTypeSize(m_pFirst[i].Type);
    return size;
}
