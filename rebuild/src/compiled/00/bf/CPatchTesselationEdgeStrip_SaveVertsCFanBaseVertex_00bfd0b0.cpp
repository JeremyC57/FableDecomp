// CPatchTesselationEdgeStrip::SaveVerts<CWaterFanBaseVertex>(CEdgeStipVertexArray<CWaterFanBaseVertex>& verts, CDataOutputStream& stream) -- writes the vertex count, then (when there are any) the range-compressed vertex data's size and bytes
// Leaf: SaveVerts
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFD0B0, 135 bytes, __fastcall (`this` unused), `ret 8`. Ego_r.pdb
//   ??$SaveVerts@UCWaterFanBaseVertex@CPatchTesselationEdgeStrip@@@CPatchTesselationEdgeStrip@@AAEXAAV?$CEdgeStipVertexArray@UCWaterFanBaseVertex@CPatchTesselationEdgeStrip@@@@AAVCDataOutputStream@@@Z
//   (a private member template).
// Callees: CDataOutputStream::WriteSLONG 0x00993EB0 (manifest "WriteCBYTE" is propagated), the stream's virtual
//   Write(const void*, long) (slot 4, +0x10), CRangeCompressor::Compress(void*, long count, long stride,
//   CArray<unsigned char>*) 0x00F3AFC0, free 0x00BFEA14.
// Layout: Ego_r.pdb CEdgeStipVertexArray<T> { T* Data; long Count; }; CWaterFanBaseVertex is 0x38 bytes (the pushed stride).
// Shape: the stream / compressor / output-buffer model of the landed CEngineWaterBackgroundSubPatch::Save 0x00BFD670 (the
//   same module): the empty compressor is homed in the dead `verts` argument slot once verts is in edi, the STLport-style
//   CArray<unsigned char> buffer is three nulled pointers, GetData() is begin or the array itself when empty, its
//   destructor frees the block. The vertex pointer is read into a named local before the compress call (retail keeps it
//   in edi, the register verts dies in; the direct `verts.Data` argument picks edx and shifts the later compare to ecx).
// NF377 (NAME_FIXES of NF374): the instance is SaveVerts<CWaterFanBaseVertex> (the fourth of Save 0x00BFC770's member-order calls
//   Verts / FanBaseVerts / WaterVerts / WaterFanBaseVerts = 0x00BFCF00 / 0x00BFCF90 / 0x00BFD020 / 0x00BFD0B0; the pushed stride 0x38 is
//   sizeof CWaterFanBaseVertex, CFanBaseVertex is 0x10 in the landed 0x00BFCF90); was the CFanBaseVertex spelling, which 0x00BFCF90 defines.
//   The unused made-up SaveFanBaseVerts declaration is dropped.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CDataOutputStream {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long size);    // slot 4 (+0x10)
public:
    void WriteSLONG(long);   // 0x00993EB0
};

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    CArray() : First(0), Last(0), End(0) { }
    ~CArray() { if (First) free(First); }
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};

class CRangeCompressor {
public:
    long Compress(const void* data, long count, long stride, CArray<unsigned char>& output);   // 0x00F3AFC0
};

template <class T> class CEdgeStipVertexArray {
public:
    T* Data;                                             // +0x00
    long Count;                                          // +0x04
};

class CPatchTesselationEdgeStrip {
public:
    struct CWaterFanBaseVertex {                         // 0x38 bytes
        unsigned char _bytes[0x38];
    };
private:
    template <class T> void SaveVerts(CEdgeStipVertexArray<T>& verts, CDataOutputStream& stream);
public:
};

template <class T>
void CPatchTesselationEdgeStrip::SaveVerts(CEdgeStipVertexArray<T>& verts, CDataOutputStream& stream)
{
    stream.WriteSLONG(verts.Count);
    if (verts.Count > 0) {
        CRangeCompressor compressor;
        CArray<unsigned char> buffer;
        T* data = verts.Data;
        long size = compressor.Compress(data, verts.Count, sizeof(T), buffer);
        stream.WriteSLONG(size);
        stream.Write(buffer.GetData(), size);
    }
}

template void CPatchTesselationEdgeStrip::SaveVerts<CPatchTesselationEdgeStrip::CWaterFanBaseVertex>(
    CEdgeStipVertexArray<CPatchTesselationEdgeStrip::CWaterFanBaseVertex>& verts, CDataOutputStream& stream);