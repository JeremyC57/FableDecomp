// CompareDataOffset::operator()(unsigned long a, unsigned long b) -- sort predicate of CBankFile entry indices by their data offset in the bank (packed offsets when present, else the runtime-data array)
// Leaf: ??RCompareDataOffset
// Retail: Fable.exe @ 0x009CC640, 94 bytes, `__fastcall`, `ret 8`. Ego_r.pdb ??RCompareDataOffset@CBankFile@@QAE_NKK@Z (the
// nested CBankFile::CompareDataOffset { CBankFile* BankFile; }, used by std::sort over the entry indices; it is modelled at
// namespace scope here because the shared header's CBankFile cannot take a nested class). Six callers in the sort helpers
// around 0x009D2E51..0x009D36E3.
// Callees: CPackedUIntArray::operator[](unsigned long) 0x00A629F0.
// Layout: engine/CBankFile.h with FABLE_RETAIL_BANK_FILE (retail 12-byte CArrays): RuntimeData +0x20 (CArray of 12-byte
// CRuntimeData, DataOffset first), PackedDataOffset +0x38 (CPackedUIntArray: PackedInts +0, Size +4, Bits +8, Bias +0xC).
// Shape: the per-index offset is an inline helper whose array members come from inline reference accessors (a reference
// local hoists the `lea ecx,[eax+38h]` above the test); packed offsets are indexed 1-based (`index - 1`); the unsigned
// compare returns through `cmp ; sbb ; neg`.
#define FABLE_RETAIL_BANK_FILE
#include "engine/CBankFile.h"

struct CRuntimeData {                       // CBankFile::CRuntimeData, Ego_r.pdb 12 bytes
    unsigned long DataOffset;               // +0x00
    unsigned long DataSize;                 // +0x04
    unsigned char DataType;                 // +0x08
    bool          Valid;                    // +0x09
};
struct CPackedUIntArray {                   // Ego_r.pdb 16 bytes
    unsigned long* PackedInts;              // +0x00
    unsigned long  Size;                    // +0x04
    unsigned char  Bits;                    // +0x08
    unsigned long  Bias;                    // +0x0C
    unsigned long operator[](unsigned long index);   // 0x00A629F0
};
struct CRuntimeDataArray {                  // CArray<CBankFile::CRuntimeData>: first / last / end
    CRuntimeData* First;
    CRuntimeData* Last;
    CRuntimeData* End;
};

inline CPackedUIntArray& PackedDataOffsetOf(CBankFile* bank) { return *(CPackedUIntArray*)bank->PackedDataOffset; }
inline CRuntimeDataArray& RuntimeDataOf(CBankFile* bank) { return *(CRuntimeDataArray*)bank->RuntimeData; }
inline unsigned long GetEntryDataOffset(CBankFile* bank, unsigned long index)
{
    if (PackedDataOffsetOf(bank).Size != 0)
        return PackedDataOffsetOf(bank)[index - 1];
    return RuntimeDataOf(bank).First[index].DataOffset;
}

struct CompareDataOffset {                  // CBankFile::CompareDataOffset
    CBankFile* BankFile;                    // +0x00
    bool operator()(unsigned long a, unsigned long b);
};

bool CompareDataOffset::operator()(unsigned long a, unsigned long b)
{
    return GetEntryDataOffset(BankFile, a) < GetEntryDataOffset(BankFile, b);
}