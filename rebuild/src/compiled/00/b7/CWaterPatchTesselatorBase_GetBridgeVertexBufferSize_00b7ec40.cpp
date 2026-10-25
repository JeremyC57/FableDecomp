// CWaterPatchTesselatorBase::GetBridgeVertexBufferSize() -- the number of vertices in the bridge vertex buffer (CTVertexWaterBackground instantiation)
// Leaf: ?GetBridgeVertexBufferSize@?$CWaterPatchTesselatorBase@VCTVertexWaterBackground@@@@EAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B7EC40, 30 bytes (+2 int3 padding), thiscall; vtable 0x012A3334 slot 1 (rtti-vtables.tsv).
//   PDB name ?GetBridgeVertexBufferSize@?$CWaterPatchTesselatorBase@VCTVertexWaterBackground@@@@EAEJXZ (private virtual). The catalog's
//   "vfunc_N" label is the slot number.
// Callees: none (leaf).
// Layout: Ego_r.pdb CWaterPatchTesselatorBase<T> (BridgeVertexBuffer at PDB +0x18) re-based on the retail bytes: the
//   12-byte retail CArray sits at +0x14. No generated engine header for the template.
// Shape: the CArray size: the pointer difference divided by the element size (reciprocal multiply, signed).
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

template <class T> long CWaterPatchTesselatorBase<T>::GetBridgeVertexBufferSize()
{
    return BridgeVertexBuffer.GetCount();
}

template long CWaterPatchTesselatorBase<CTVertexWaterBackground>::GetBridgeVertexBufferSize();