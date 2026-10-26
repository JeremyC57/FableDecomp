// CPODVector_00701e40::TransferBinaryOut(CDataOutputStream*) const -- write a vector of 4-byte POD elements: the element count, then the raw element block (or the vector object itself when empty)
// Leaf: TransferBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x00701E40, 59 bytes (+ int3 padding), `ret 4` (__fastcall on the vector itself, the stream on the stack).
// Label: the catalog name "CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::TransferBinaryOut" is propagated:
//   `this` is the vector, not a traits object (no direct callers (an out-of-line instantiation)). The element type is
//   unknown; it is modelled by size (4 bytes) and the class named by address.
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0 (the count), CDataOutputStream::Write(const void*, unsigned long) = virtual
//   slot 4 (+0x10).
// Shape: the data/size idiom of the landed CSimVoice::TransferBinaryOut 0x006808C0 and 0x004EA26B: data =
//   `size() > 0 ? first : (void*)this` (unsigned `ja`), byte count size() * sizeof; both accessors __forceinline, the
//   second size() shared by both arguments. Speed-optimised (default) TU (`sar 2` / `shl 2`).

class CDataOutputStream { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteLong(long value);                                 // 0x00993EB0
};

struct SPODElement_00701e40 { unsigned char _bytes[4]; };

struct CPODVector_00701e40 {                  // STLport vector, 12 bytes
    SPODElement_00701e40* _M_start;
    SPODElement_00701e40* _M_finish;
    SPODElement_00701e40* _M_end_of_storage;
    __forceinline unsigned long size() const { return _M_finish - _M_start; }
    __forceinline const SPODElement_00701e40* data() const { return size() > 0 ? _M_start : (const SPODElement_00701e40*)this; }
    void TransferBinaryOut(CDataOutputStream* stream) const;
};

void CPODVector_00701e40::TransferBinaryOut(CDataOutputStream* stream) const
{
    stream->WriteLong(size());
    stream->Write(data(), size() * sizeof(SPODElement_00701e40));
}