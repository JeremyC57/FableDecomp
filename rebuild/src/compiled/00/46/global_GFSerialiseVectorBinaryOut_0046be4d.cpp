// GFSerialiseVectorBinaryOut(CDataOutputStream& stream, vector<CReverbEnvironmentEntry>& value) -- write the element count, then every reverb environment's 21 parameters as one raw 0x54-byte block
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046BE4D, 60 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: CSoundDef::Transfer (landed 0x0046A9CE) hands ReverbEnvironmentEntries (+0x6c) to CPersistContext::TransferBinary
//   @ 0x0046BE1F, whose save half is this body; Ego_r.pdb ??$GFSerialiseVectorBinaryOut@VCReverbEnvironmentEntry@@...;
//   CReverbEnvironmentEntry (0x54 bytes) = {float Params[21]}.
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count), the stream's virtual Write(const void*, long) (slot 4,
//   `call [eax+10h]`; CFileDataOutputStream 0x00994480 / CMemoryDataOutputStream 0x00996CC0).
// Shape: the generic template (landed 0x005C12A1) with the entry's traits inlined (`__forceinline`) to a single raw
//   `stream.Write(value, sizeof(CReverbEnvironmentEntry))`; `count = value.size()` (`push 54h ; pop ecx ; idiv`), the byte
//   offset in ebp, the count run down in ebx. Size TU, no frame pointer: `#pragma optimize("s", on)`.
// Model: engine/CDataOutputStream.h is data-only (its vptr is a `void*`), so the writer is a local polymorphic view with the
//   PDB's virtual slots (destructor, SetPosition, GetPosition, Close, Write) over the same {vptr, NewLine} layout;
//   CReverbEnvironmentEntry is local (no engine header); the vector is STLport's three words.
#pragma optimize("s", on)

class CCharStringData;
class CDataOutputStream { public:
    virtual ~CDataOutputStream();                     // slot 0
    virtual void SetPosition(unsigned long position);       // slot 1
    virtual unsigned long GetPosition() const;              // slot 2
    virtual void Close();                                   // slot 3
    virtual void Write(const void* data, long size);        // slot 4
    CCharStringData* NewLine_PStringData;                   // +0x04
    void WriteSLONG(long value);                            // 0x00993EB0
};
class CReverbEnvironmentEntry { public:
    float Params[21];                                       // +0x00
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CReverbEnvironmentEntry> {
    __forceinline void TransferBinaryOut(CDataOutputStream& stream, const CReverbEnvironmentEntry* value) const
    {
        stream.Write(value, sizeof(CReverbEnvironmentEntry));
    }
};
template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& stream, std::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}
template void __fastcall GFSerialiseVectorBinaryOut<CReverbEnvironmentEntry>(CDataOutputStream& stream, std::vector<CReverbEnvironmentEntry>& value);