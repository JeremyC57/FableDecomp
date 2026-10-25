// CPersistContext::Transfer(long& value) -- binary-only, unnamed long transfer: CRC check, then load or save by mode
// Leaf: ??$Transfer@J@CPersistContext@@QAEXAAJ@Z
// Retail: Fable.exe @ 0x00431102, 65 bytes, `ret 4`; ecx = the context, one pushed field address. No Ego_r.pdb symbol
// takes only the value (the PDB has ??$Transfer@J@CPersistContext@@QAEXPBDAAJ... with a name); the parameter list is a
// placeholder, as for the landed Transfer<float> 0x00431061 (same body with WriteFloat / CPersistTraits<float>).
// Identity (T = long): CBettingDef::Transfer (0x004DF5C7) calls it three times, on MinBet/MaxBet/BetIncrement (struct_layouts_egor CBettingDef +0x38/+0x3C/+0x40, long), and the unsigned long twin 0x00431020 for its five ulong fields; CHasNameDef's three ulong fields go only to 0x00431020. Ego_r.pdb has ??$Transfer@J@CPersistContext@@.
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E); a CPersistTraits<long> local (value zeroed,
// `and [ebp-4],0`); Mode (+0x18) 2 = binary load: traits.TransferBinaryIn(*PLoadStream (+0x24), value) (0x0040FE60);
// 3 = binary save: PSaveStream (+0x28)->WriteSLONG(value) (0x00993EB0; landed TUs bind 0x00993EA0 = WriteULONG and
// 0x00993EB0 = WriteSLONG, and both the long and the unsigned long instantiation call 0x00993EB0).
// Replaces a pack(1) `struct T::UpdateShadowScene` model with fake profiler names (FINDINGS chunk 382).
// Layout: Ego_r.pdb CPersistContext -- Mode +0x18, PLoadStream +0x24, PSaveStream +0x28.
// Shape: size TU with frame pointers (`push ebp / mov ebp,esp`, `leave`).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CMemoryDataInputStream;
class CDataOutputStream {
public:
    void WriteSLONG(long value);                         // 0x00993EB0 (the 4-byte writer both the long and unsigned long transfers call)
};
template <class T> class CPersistTraits {
public:
    T Value;
    CPersistTraits() : Value() {}
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x0040FE60 for long
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
    void CheckCRC(const char* name);                     // 0x00404500
    template <class T> void Transfer(T& value);
};
template <> void CPersistContext::Transfer<long>(long& value)
{
    CheckCRC("");
    CPersistTraits<long> traits;
    switch (Mode)
    {
    case MODE_BINARY_LOAD:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_BINARY_SAVE:
        PSaveStream->WriteSLONG(value);
        break;
    }
}
