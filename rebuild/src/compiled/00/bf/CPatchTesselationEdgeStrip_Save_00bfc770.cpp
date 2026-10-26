// CPatchTesselationEdgeStrip::Save(CDataOutputStream* stream) -- write StartPos and Size (words) and the Vertical flag, then save the four vertex arrays through the SaveVerts<T> member template
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFC770, length 102 bytes.
// Callees: CDataOutputStream::WriteUShort @ 0x00993E80 (x2), WriteBool @ 0x00993E30; SaveVerts<CVertex> @ 0x00BFCF00,
// SaveVerts<CFanBaseVertex> @ 0x00BFCF90, SaveVerts<CWaterVertex> @ 0x00BFD020, SaveVerts<CWaterFanBaseVertex> @ 0x00BFD0B0
// (the catalog gives all four the CFanBaseVertex name; they are the four instantiations in member order).
// Layout: Ego_r.pdb CPatchTesselationEdgeStrip (0x2c bytes, vtable 0x012A8318; same model as the landed Reset 0x00BFC3B0).
// Shape: the stream kept in edi; the words are zero-extended with `xor eax,eax ; mov ax,[esi+4]`, the bool with
// `xor edx,edx ; mov dl,[esi+29h]`; each SaveVerts is a __fastcall member on the strip taking (array*, stream); `ret 4`.
class CDataOutputStream { public:
    void WriteUShort(unsigned short value);        // 0x00993E80
public:
    void WriteEBOOL(bool);   // 0x00993E30
};

template <class T>
class CEdgeStipVertexArray { public:
    T*   Data;                                     // +0x00
    long Count;                                    // +0x04
};

class CPatchTesselationEdgeStrip { public:                // vtable 0x012A8318 (CBase-derived), 0x2c bytes
    struct CVertex;
    struct CFanBaseVertex;
    struct CWaterVertex;
    struct CWaterFanBaseVertex;
    virtual void v00();
    unsigned short StartPos;                                            // +0x04
    unsigned short Size;                                                // +0x06
    CEdgeStipVertexArray<CVertex>             Verts;                    // +0x08
    CEdgeStipVertexArray<CFanBaseVertex>      FanBaseVerts;             // +0x10
    CEdgeStipVertexArray<CWaterVertex>        WaterVerts;               // +0x18
    CEdgeStipVertexArray<CWaterFanBaseVertex> WaterFanBaseVerts;        // +0x20
    bool Tesselated;                                                    // +0x28
    bool Vertical;                                                      // +0x29
    template <class T> void SaveVerts(CEdgeStipVertexArray<T>* verts, CDataOutputStream* stream);   // 0x00BFCF00..0x00BFD0B0
    void Save(CDataOutputStream* stream);
};

void CPatchTesselationEdgeStrip::Save(CDataOutputStream* stream)
{
    stream->WriteUShort(StartPos);
    stream->WriteUShort(Size);
    stream->WriteEBOOL(Vertical);
    SaveVerts(&Verts, stream);
    SaveVerts(&FanBaseVerts, stream);
    SaveVerts(&WaterVerts, stream);
    SaveVerts(&WaterFanBaseVerts, stream);
}