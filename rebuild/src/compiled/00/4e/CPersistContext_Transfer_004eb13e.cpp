// CPersistContext::Transfer(EQuakeLength& value) -- binary-only, unnamed enum transfer: CRC check, then load or save by mode
// Leaf: ??$Transfer@W4EQuakeLength@@@CPersistContext@@QAEXAAW4EQuakeLength@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EB13E, 60 bytes, thiscall, `ret 4` (size module with a frame pointer).
// Identity (proven): its only caller is CExplosionDef::Transfer 0x004EB02F (the next member); the load arm calls 0x004E6B50, bound ?TransferBinaryIn@?$CPersistTraits@W4EQuakeLength@@@@....
// Callees: CPersistContext::CheckCRC 0x00404500 (the pooled empty literal 0x0122D70E), CPersistTraits<EQuakeLength>::TransferBinaryIn
//   0x004E6B50, CDataOutputStream::WriteSLONG 0x00993EB0.
// Shape (family M062, representative 0x00471765): the landed Transfer<long> 0x00431102 with the enum traits as a temporary in the
//   load arm (its 4-byte DefaultVal unset by the user constructor, so it is homed in the dead `value` slot: `lea ecx,[ebp+8]`) and the save arm widening the enum.
// Layout: Ego_r.pdb CPersistContext -- Mode +0x18, PLoadStream +0x24, PSaveStream +0x28.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CMemoryDataInputStream;
class CDataOutputStream {
public:
    void WriteSLONG(long value);                          // 0x00993EB0
};
enum EQuakeLength { EQUAKELENGTH_NONE = 0 };
template <class T> class CPersistTraits;
template <> class CPersistTraits<EQuakeLength> {
public:
    CPersistTraits() { }                                  // DefaultVal left unset (no store in retail)
    void TransferBinaryIn(CMemoryDataInputStream& stream, EQuakeLength& value);   // 0x004E6B50
    EQuakeLength DefaultVal;
};
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
    protected: void CheckCRC(const char* name); public:                     // 0x00404500
    template <class T> void Transfer(T& value);
};
template <> void CPersistContext::Transfer<EQuakeLength>(EQuakeLength& value)
{
    CheckCRC("");
    switch (Mode)
    {
    case MODE_BINARY_LOAD:
        CPersistTraits<EQuakeLength>().TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_BINARY_SAVE:
        PSaveStream->WriteSLONG((long)value);
        break;
    }
}