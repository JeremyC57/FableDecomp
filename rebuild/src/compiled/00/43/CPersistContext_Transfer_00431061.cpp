// CPersistContext::Transfer(float& value) -- binary-only, unnamed float transfer: CRC check, then load or save by mode
// Leaf: ??$Transfer@M@CPersistContext@@QAEXAAM@Z
// Retail: Fable.exe @ 0x00431061, 70 bytes, `ret 4`; 30 callers in the 0x00430B00-0x00430FF0 persist block (ecx = the
// context, one pushed field address each). No Ego_r.pdb symbol takes only the value (the PDB Transfer<T> all take a name);
// the leaf is a placeholder. Catalog label CEngineLightingManager::UpdateShadowScene is wrong.
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E); a CPersistTraits<float> local (its value zeroed,
// `fldz / fstp`); Mode (+0x18) 2 = binary load: traits.TransferBinaryIn(*PLoadStream (+0x24), value) (0x0040EFB0);
// 3 = binary save: PSaveStream (+0x28)->WriteFloat(value) (0x00993EE0, catalog label WriteCBYTE). Sibling 0x004310A7 is the
// CWideString instantiation.
// Layout: Ego_r.pdb CPersistContext -- Mode +0x18, PLoadStream +0x24, PSaveStream +0x28; no engine/ header used (only
// three members touched).
// Shape: size TU with frame pointers (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CMemoryDataInputStream;
class CDataOutputStream {
public:
    void WriteFloat(float value);                        // 0x00993EE0
};
template <class T> class CPersistTraits {
public:
    T Value;
    CPersistTraits() : Value() {}
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x0040EFB0 for float
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
template <> void CPersistContext::Transfer<float>(float& value)
{
    CheckCRC("");
    CPersistTraits<float> traits;
    switch (Mode)
    {
    case MODE_BINARY_LOAD:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_BINARY_SAVE:
        PSaveStream->WriteFloat(value);
        break;
    }
}