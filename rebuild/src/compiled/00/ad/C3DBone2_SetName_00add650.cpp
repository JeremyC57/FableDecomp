// C3DBone2::SetName(const CCharString& name) -- store the CRC of the bone's name string (empty string when the name has no data)
// Leaf: ?SetName@C3DBone2@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADD650, 0x3F bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: Ego_r.pdb ?SetName@C3DBone2@@QAEXABVCCharString@@@Z; C3DBone2 (60 bytes: NameCRC +0, Parent +4,
// OriginalNoChildren +8, LocalizationMatrix +0xC). Only caller C3DMeshFileBoneChunk::GetBone() const 0x00AB9DD0
// (`bone.SetName(Name)` on its CCharString at +0x6C after the Parent -1 / OriginalNoChildren 0 stores). Catalog
// "CCRC_CalcRangeChecksum" is descriptive.
// Callee: CCRC::Calc(unsigned long seed, const void* data, unsigned long length) @ 0x004014A0 (static __fastcall).
// Data: the empty string literal 0x0129AAF4.
// Shape: `NameCRC = CCRC::CalcFromString(name);` with CalcFromString and the CCharString accessors inline, each with
// its null arm first: the length (`PStringData == 0 ? 0 : PStringData->Length`) is evaluated first, then the data
// pointer's null test splits into two Calc calls ("" or PStringData->Data).
class CCharString {
public:
    struct CStringData {
        const char* Data;                                   // +0x0
        long        Length;                                 // +0x4
    };
    CStringData* PStringData;                               // +0x0
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Data; }
    unsigned long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
};

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
    static unsigned long __fastcall CalcFromString(const CCharString& s) { return Calc(0, s.GetString(), s.Length()); }
};

class CMatrix3x4 { public: float M[12]; };

class C3DBone2 {                                            // Ego_r.pdb, 60 bytes
public:
    unsigned long NameCRC;                                  // +0x00
    long          Parent;                                   // +0x04
    long          OriginalNoChildren;                       // +0x08
    CMatrix3x4    LocalizationMatrix;                       // +0x0C
    void SetName(const CCharString& name);
};

void C3DBone2::SetName(const CCharString& name)
{
    NameCRC = CCRC::CalcFromString(name);
}