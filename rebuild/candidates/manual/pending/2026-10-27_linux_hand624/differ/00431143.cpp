// CPersistContext::Transfer<CCharString>(CCharString& value) -- nameless binary transfer of a string: CRC check with the empty key, a default CPersistTraits<CCharString> (its CCharString value), then read the string from the load stream (Mode 2) or write it to the save stream (Mode 3)
// Grade: DIFFER(91v91) -- frame-slot swap only: retail keeps the traits' default string at [ebp-4] and the read temporary at [ebp-8], ours the reverse (6 lines: the two ctor/dtor/push `lea`s); every other byte equal
// Leaf: ??$Transfer@VCCharString@@@CPersistContext@@QAEXAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00431143, 91 bytes, `ret 4`; size-optimised frame-pointer TU (`push ecx ; push ecx`,
//   `push dword ptr [ebp+8]`, `leave`): `#pragma optimize("s", on)` + `("y", off)`, as the landed siblings 0x00431020/0x00431061/0x00431102.
// Name: the binding `??$Transfer@VCCharString@@@CPersistContext@@QAEXAAVCCharString@@@Z` (reloc_bindings.tsv) and the landed nameless
//   Transfer<float> 0x00431061 / Transfer<long> 0x00431102 / Transfer<unsigned long> 0x00431020 of the same block; the Ego_r/FableWin PDB
//   Transfer<CCharString> overloads all take a name (QAEXPBDAAVCCharString@@...), so the nameless TLC overload has no PDB symbol.
//   The catalog's "TransferString" is descriptive.
// Callees: CPersistContext::CheckCRC(const char*) 0x00404500 (PDB ?CheckCRC@CPersistContext@@IAEXPBD@Z, pooled "" 0x0122D70E);
//   CCharString::CCharString() 0x0099E4B0, ~CCharString() 0x0099EAE0, operator=(const CCharString&) 0x0099EFB0 (PDB QAEABV0@ABV0@@Z);
//   the load stream's virtual ReadNullTerminatedString() (CDataInputStream slot 6, +0x18; RTTI vtable 0x0123675C slot 6 = 0x00995D30);
//   CDataOutputStream::WriteNullTerminatedString(const CCharString&) 0x00995AE0.
// Shape: `CheckCRC(""); CPersistTraits<CCharString> traits; switch (Mode) { case 2: traits.TransferBinaryIn(*PLoadStream, value); case 3:
//   traits.TransferBinaryOut(*PSaveStream, value); }` with the PDB CPersistTraits<CCharString>::TransferBinaryIn(CMemoryDataInputStream&,
//   CCharString&) / TransferBinaryOut(CDataOutputStream&, const CCharString&) inlined (TransferBinaryIn `__forceinline`: under /Os VC7.1
//   otherwise calls it). Tried for the slot swap: a plain CCharString local (the earlier attempt), the traits object, the by-value
//   TransferBinaryIn(stream) overload (adds a `lea eax` reload), an inline TransferBinary helper holding the traits and the switch,
//   the read in the body, an explicit instantiation of CPersistTraits<CCharString>, reversed case order, the QFE-4035 compiler -- all swap.
// Layout: Ego_r.pdb CPersistContext (Mode +0x18, PLoadStream +0x24, PSaveStream +0x28); no engine header; CDataInputStream's virtual
//   order from the RTTI vtables (the generated engine/CDataInputStream.h flattens the vptr, so a local model).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString {
public:
    void* PStringData;
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other); // 0x0099EFB0
};
class CWideString;
class CDataInputStream {
public:
    virtual ~CDataInputStream();
    virtual void SetPosition(unsigned long pos);
    virtual unsigned long GetPosition() const;
    virtual void Close();
    virtual void ReadCompressed(void* buffer, long size);
    virtual void ReadZlibCompressed(void* buffer, long size);
    virtual CCharString ReadNullTerminatedString();         // slot 6 (+0x18)
    virtual CWideString ReadNullTerminatedWString();        // slot 7 (+0x1c)
};
class CMemoryDataInputStream : public CDataInputStream {
};
class CDataOutputStream {
public:
    void WriteNullTerminatedString(const CCharString& text);   // 0x00995AE0
};
template <class T> class CPersistTraits {
public:
    T Value;
    CPersistTraits() : Value() {}
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);
    void TransferBinaryOut(CDataOutputStream& stream, const T& value);
};
template <> __forceinline void CPersistTraits<CCharString>::TransferBinaryIn(CMemoryDataInputStream& stream, CCharString& value)
{
    value = stream.ReadNullTerminatedString();
}
template <> inline void CPersistTraits<CCharString>::TransferBinaryOut(CDataOutputStream& stream, const CCharString& value)
{
    stream.WriteNullTerminatedString(value);
}
class CPersistContext {
public:
    enum EMode { MODE_0, MODE_1, MODE_BINARY_LOAD, MODE_BINARY_SAVE };
    unsigned char           _pad00[0x18];
    EMode                   Mode;                        // +0x18
    bool                    Binary;                      // +0x1c
    unsigned char           _pad1d[3];
    void*                   PStringToSaveTo;             // +0x20
    CMemoryDataInputStream* PLoadStream;                 // +0x24
    CDataOutputStream*      PSaveStream;                 // +0x28
    protected: void CheckCRC(const char* name); public:  // 0x00404500
    template <class T> void Transfer(T& value);
};
template <> void CPersistContext::Transfer<CCharString>(CCharString& value)
{
    CheckCRC("");
    CPersistTraits<CCharString> traits;
    switch (Mode)
    {
    case MODE_BINARY_LOAD:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_BINARY_SAVE:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}
