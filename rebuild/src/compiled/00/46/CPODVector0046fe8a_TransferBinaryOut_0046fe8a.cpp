// CPODVector_0046fe8a::TransferBinaryOut(CDataOutputStream*) const -- write a vector of 0x18-byte POD elements: the element count, then the raw element block (or the vector object itself when empty)
// Leaf: TransferBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046FE8A, 68 bytes, `ret 4` (__fastcall on the vector itself, the stream on the stack).
// Label: the catalog name "CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::TransferBinaryOut" is propagated:
//   `this` is the vector, not a traits object (its only caller, the persist-context transfer helper 0x0046FE40 (CheckCRC, a 12-byte traits local, mode switch), calls it as `value->TransferBinaryOut(outputStream)` with the value in ecx; its read twin is 0x0046FEE9). The element type is
//   unknown; it is modelled by size (0x18 bytes) and the class named by address.
// Callees: CDataOutputStream::WriteULong @ 0x00993EA0 (the count), CDataOutputStream::Write(const void*, unsigned long) = virtual
//   slot 4 (+0x10).
// Shape: the data/size idiom of the landed CSimVoice::TransferBinaryOut 0x006808C0 and 0x004EA26B: data =
//   `size() > 0 ? first : (void*)this` (unsigned `ja`), byte count size() * sizeof; both accessors __forceinline, the
//   second size() shared by both arguments. Size-optimised TU: #pragma optimize("s", on) (`push 18h ; pop ecx`, `lea ; shl` for the *24).
#pragma optimize("s", on)
struct CDataOutputStream {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteULong(unsigned long value);                         // 0x00993EA0
};

struct SPODElement_0046fe8a { unsigned char _bytes[0x18]; };

struct CPODVector_0046fe8a {                  // STLport vector, 12 bytes
    SPODElement_0046fe8a* _M_start;
    SPODElement_0046fe8a* _M_finish;
    SPODElement_0046fe8a* _M_end_of_storage;
    __forceinline unsigned long size() const { return _M_finish - _M_start; }
    __forceinline const SPODElement_0046fe8a* data() const { return size() > 0 ? _M_start : (const SPODElement_0046fe8a*)this; }
    void TransferBinaryOut(CDataOutputStream* stream) const;
};

void CPODVector_0046fe8a::TransferBinaryOut(CDataOutputStream* stream) const
{
    stream->WriteULong(size());
    stream->Write(data(), size() * sizeof(SPODElement_0046fe8a));
}