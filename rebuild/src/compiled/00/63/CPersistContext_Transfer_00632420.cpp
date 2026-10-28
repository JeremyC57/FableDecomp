// CPersistContext::Transfer(EEngineGraphicType& value) -- binary-only, unnamed enum transfer: CRC-check the empty key, then binary load through CPersistTraits<EEngineGraphicType> or binary save as a long; the other modes do nothing
// Leaf: ??$Transfer@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00632420, 80 bytes (body 0x4C + int3 padding), thiscall `ret 4`.
// Identity: T = `EEngineGraphicType`; evidence: its only caller NUISystem::CUIDef::Transfer 0x00631C60 (vtable 0x01259F8C slot 18, RTTI .?AVCUIDef@NUISystem@@) passes +0xA0 = Ego_r CUIDef::MeshType (PDB +0xBC; five 12-byte vectors and a 12-byte map before it; W4EEngineGraphicType@@)
//   Family G002 (12 byte-identical bodies, one COMDAT per enum: no ICF in retail, so each address is one T; the bodies sit
//   next to the def class whose Transfer first instantiated them). The manifest labels "CPersistContext_TransferFloat" /
//   "CPersistTraits<float>::Transfer" are propagated: a float would be saved through WriteFloat, not this long writer, and
//   the PDB types of the transferred members are enums. No Ego_r symbol takes only the value (TLC's def transfers are
//   binary-only and unnamed); the parameter list follows the landed Transfer<unsigned long> 0x00431020.
// Callees: CPersistContext::CheckCRC 0x00404500 (the pooled empty literal 0x0122D70E), CDataOutputStream::WriteSLONG
//   0x00993EB0 (the 4-byte writer), CPersistTraits<EEngineGraphicType>::TransferBinaryIn 0x00632470.
// Layout: Ego_r.pdb CPersistContext -- Mode +0x18, PLoadStream +0x24, PSaveStream +0x28.
// Shape: speed TU. The traits object is constructed before the switch (DefaultVal zeroed: `mov [esp+4],0` scheduled
//   between `sub eax,2` and its `je`); the switch keeps only cases 2 and 3, binary save laid out first. VC7.1 rejects
//   `Value()` for an enum declared in a namespace (C2065), so the zero is spelt `Value((T)0)` (same code).
enum EEngineGraphicType { ENGINE_GRAPHIC_TYPE_0 };
class CMemoryDataInputStream;
class CDataOutputStream {
public:
    void WriteSLONG(long value);                         // 0x00993EB0
};
template <class T> class CPersistTraits {
public:
    T Value;                                             // PDB: DefaultVal
    CPersistTraits() : Value((T)0) {}
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x00632470
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
template <class T> void CPersistContext::Transfer(T& value)
{
    CheckCRC("");
    CPersistTraits<T> traits;
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
template void CPersistContext::Transfer<EEngineGraphicType >(EEngineGraphicType& value);