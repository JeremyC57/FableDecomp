// CPersistContext::Transfer<CWideString>(CWideString& value) -- nameless binary transfer of a string: CRC check with the empty key, a default CPersistTraits<CWideString> (its CWideString value), then read the string from the load stream (Mode 2) or write it to the save stream (Mode 3)
// Grade: DIFFER(91v91) -- frame-slot swap only: retail keeps the traits' default string at [ebp-4] and the read temporary at [ebp-8], ours the reverse (6 lines: the two ctor/dtor/push `lea`s); every other byte equal
// Leaf: ??$Transfer@VCWideString@@@CPersistContext@@QAEXAAVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004310A7, 91 bytes, `ret 4`; size-optimised frame-pointer TU (`push ecx ; push ecx`,
//   `push dword ptr [ebp+8]`, `leave`): `#pragma optimize("s", on)` + `("y", off)`, as the landed siblings 0x00431020/0x00431061/0x00431102.
// Name: the twin 0x00431143 (bound `??$Transfer@VCCharString@@@CPersistContext@@QAEXAAVCCharString@@@Z`) and the landed nameless
//   Transfer<float> 0x00431061 / Transfer<long> 0x00431102 / Transfer<unsigned long> 0x00431020 of the same block; the Ego_r/FableWin PDB
//   Transfer<CWideString> overloads all take a name (QAEXPBDAAVCWideString@@...), so the nameless TLC overload has no PDB symbol.
//   The catalog's "TransferWideString" is descriptive.
// Callees: CPersistContext::CheckCRC(const char*) 0x00404500 (PDB ?CheckCRC@CPersistContext@@IAEXPBD@Z, pooled "" 0x0122D70E);
//   CWideString::CWideString() 0x0099AED0, ~CWideString() 0x0099B510, operator=(const CWideString&) 0x0099B7D0 (PDB QAEABV0@ABV0@@Z);
//   the load stream's virtual ReadNullTerminatedWString() (CDataInputStream slot 7, +0x1c; RTTI vtable 0x0123675C slot 7 = 0x00994960);
//   CDataOutputStream::WriteNullTerminatedWString(const CWideString&) 0x00993F00.
// Shape: `CheckCRC(""); CPersistTraits<CWideString> traits; switch (Mode) { case 2: traits.TransferBinaryIn(*PLoadStream, value); case 3:
//   traits.TransferBinaryOut(*PSaveStream, value); }` with the PDB CPersistTraits<CWideString>::TransferBinaryIn(CMemoryDataInputStream&,
//   CWideString&) / TransferBinaryOut(CDataOutputStream&, const CWideString&) inlined (TransferBinaryIn `__forceinline`: under /Os VC7.1
//   otherwise calls it). Tried for the slot swap: a plain CWideString local (the earlier attempt), the traits object, the by-value
//   TransferBinaryIn(stream) overload (adds a `lea eax` reload), an inline TransferBinary helper holding the traits and the switch,
//   the read in the body, an explicit instantiation of CPersistTraits<CWideString>, reversed case order, the QFE-4035 compiler -- all swap.
// Layout: Ego_r.pdb CPersistContext (Mode +0x18, PLoadStream +0x24, PSaveStream +0x28); no engine header; CDataInputStream's virtual
//   order from the RTTI vtables (the generated engine/CDataInputStream.h flattens the vptr, so a local model).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CWideString {
public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    ~CWideString();                                         // 0x0099B510
    const CWideString& operator=(const CWideString& other); // 0x0099B7D0
};
class CCharString;
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
    void WriteNullTerminatedWString(const CWideString& text);  // 0x00993F00
};
template <class T> class CPersistTraits {
public:
    T Value;
    CPersistTraits() : Value() {}
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);
    void TransferBinaryOut(CDataOutputStream& stream, const T& value);
};
template <> __forceinline void CPersistTraits<CWideString>::TransferBinaryIn(CMemoryDataInputStream& stream, CWideString& value)
{
    value = stream.ReadNullTerminatedWString();
}
template <> inline void CPersistTraits<CWideString>::TransferBinaryOut(CDataOutputStream& stream, const CWideString& value)
{
    stream.WriteNullTerminatedWString(value);
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
template <> void CPersistContext::Transfer<CWideString>(CWideString& value)
{
    CheckCRC("");
    CPersistTraits<CWideString> traits;
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
