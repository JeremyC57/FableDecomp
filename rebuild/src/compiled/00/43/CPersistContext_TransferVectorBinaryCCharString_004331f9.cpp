// CPersistContext::TransferVectorBinary<CCharString>(std::vector<CCharString>&) -- binary-mode vector transfer: CRC check, then read from the load stream or write to the save stream
// Leaf: ??$TransferVectorBinary@VCCharString@@@CPersistContext@@
// Retail: Fable.exe @ 0x004331F9, 46 bytes, `ret 4` (the vector is the only argument). Catalog label
// CCreatureAction_FireMissileWeapon::FrameUpdate is wrong: the body is a CPersistContext member template --
// CheckCRC("") (0x00404500, Ego_r.pdb ?CheckCRC@CPersistContext@@IAEXPBD@Z; the pooled empty literal at 0x0122D70E), then
// Mode (+0x18) == 2 (binary load) ? GFSerialiseVectorBinaryIn<CCharString>(*PLoadStream +0x24, v) (0x00433273)
// : GFSerialiseVectorBinaryOut<CCharString>(*PSaveStream +0x28, v) (0x00433227, landed as GFSerialiseVectorBinaryOut<CCharString>), both __fastcall as in Ego_r.pdb
// (??$GFSerialiseVectorBinaryIn@..@@YIXAAVCMemoryDataInputStream@@AAV?$vector@..). Callers include CFrontEndDef::Transfer (0x004331C0, vAttractModeMovie at +0x3c) and a dozen other CCharString-vector transfers.
// The name is descriptive: Ego_r.pdb's CPersistContext::TransferVector<T> takes (const char*, vector&); this retail
// instance takes only the vector and handles only the binary modes.
// Layout: Ego_r.pdb CPersistContext (Mode +0x18, PLoadStream +0x24, PSaveStream +0x28; no engine header, as landed 0x00757160).
// Opt: size TU (unaligned entry; the literal pushed before `this` is moved to esi). Shape: a switch on Mode with one
// case (`dec eax ; dec eax ; je`); an if/else compiles to `cmp [esi+18h],2`.
#pragma optimize("s", on)
class CMemoryDataInputStream;
class CDataOutputStream;
class CCharString;
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
template <class T> void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<T>& v);
template <class T> void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& stream, std::vector<T>& v);
class CPersistContext {
public:
    void*                   PDefinitionManager;          // +0x00
    void*                   PSymbolMap;                  // +0x04
    void*                   PParser;                     // +0x08
    void*                   ErrorCallback;               // +0x0c
    void*                   PStringToClean;              // +0x10
    bool                    UseDefaults;                 // +0x14
    bool                    UseCRCCheck;                 // +0x15
    bool                    SingleTransfer;              // +0x16
    bool                    Finished;                    // +0x17
    long                    Mode;                        // +0x18 EMode: 2 binary load
    bool                    Binary;                      // +0x1c
    void*                   PStringToSaveTo;             // +0x20
    CMemoryDataInputStream* PLoadStream;                 // +0x24
    CDataOutputStream*      PSaveStream;                 // +0x28
    protected: void CheckCRC(const char* name); public:                     // 0x00404500
    template <class T> void TransferVectorBinary(std::vector<T>& v)
    {
        CheckCRC("");
        switch (Mode) {
        case 2:
            GFSerialiseVectorBinaryIn(*PLoadStream, v);
            break;
        default:
            GFSerialiseVectorBinaryOut(*PSaveStream, v);
            break;
        }
    }
};
template void CPersistContext::TransferVectorBinary<CCharString>(std::vector<CCharString>&);