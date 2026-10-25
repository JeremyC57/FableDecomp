// CWaterPatchTesselatorBase::SetBridgeVertexBufferSize(long size) -- resize the bridge vertex buffer to `size` vertices (CTVertexWaterBackground instantiation)
// Leaf: ?SetBridgeVertexBufferSize@?$CWaterPatchTesselatorBase@VCTVertexWaterBackground@@@@EAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B7FCE0, 26 bytes (+6 int3 padding), thiscall; vtable 0x012A3334 slot 4 (rtti-vtables.tsv).
//   PDB name ?SetBridgeVertexBufferSize@?$CWaterPatchTesselatorBase@VCTVertexWaterBackground@@@@EAEXJ@Z (private virtual). The catalog's
//   "vfunc_N" label is the slot number.
// Callees: the CArray (STLport vector<CTVertexWaterBackground>) resize(size_type, const CTVertexWaterBackground&) 0x00B7FD00 (out of line).
// Layout: Ego_r.pdb CWaterPatchTesselatorBase<T> (BridgeVertexBuffer at PDB +0x18) re-based on the retail bytes: the
//   12-byte retail CArray sits at +0x14. No generated engine header for the template.
// Shape: STLport's one-argument `resize(n)` = `resize(n, _Tp())` inlined: the element's empty user constructor leaves the
//   frame temporary unwritten, the two-argument resize runs out of line on the member (`add ecx,14h`).
class CTVertexWaterBackground {                    // 56 bytes (Ego_r.pdb CGenericVertex<CTVertexWaterBackgroundBase>)
public:
    CTVertexWaterBackground() {}                   // no initialisation (retail's resize temporary is left unwritten)
    short X;                                       // +0x00
    short Y;                                       // +0x02
    float Z;                                       // +0x04
    float ShoreLookUpArray[12];                    // +0x08
};

template <class T> class CArray {                  // retail CArray: the 12-byte STLport vector
public:
    T* _M_start;                                   // +0
    T* _M_finish;                                  // +4
    T* _M_end_of_storage;                          // +8
    long GetCount() const { return _M_finish - _M_start; }
    T* GetData() { return GetCount() ? _M_start : (T*)this; }
    void resize(unsigned int newSize, const T& x);  // out of line (0x00B7FD00 for this instantiation)
    void resize(unsigned int newSize) { resize(newSize, T()); }
};

class CPatchTesselationEdgeStrip;
enum LANDSCAPE_EDGE_STRIP_SIDE { LANDSCAPE_EDGE_STRIP_SIDE_NONE };

template <class T> class CWaterPatchTesselatorBase {
private:
    virtual void TesselateEdge(CPatchTesselationEdgeStrip* strip, long a, long b, bool c, bool d,
                               LANDSCAPE_EDGE_STRIP_SIDE side, unsigned short* indices);   // slot 0
    virtual long GetBridgeVertexBufferSize();                  // slot 1
    virtual unsigned char* GetBridgeVertexBufferAsCArray();    // slot 2
    virtual long GetSizeOfVertex();                            // slot 3 (0x00B7EC30, landed)
    virtual void SetBridgeVertexBufferSize(long size);         // slot 4
    unsigned char _base[0x10];                                 // +0x04 tesselator state
    CArray<T> BridgeVertexBuffer;                              // +0x14
};

template <class T> void CWaterPatchTesselatorBase<T>::SetBridgeVertexBufferSize(long size)
{
    BridgeVertexBuffer.resize(size);
}

template void CWaterPatchTesselatorBase<CTVertexWaterBackground>::SetBridgeVertexBufferSize(long);