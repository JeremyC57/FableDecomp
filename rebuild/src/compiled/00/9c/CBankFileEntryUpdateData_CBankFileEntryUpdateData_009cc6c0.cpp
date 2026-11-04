// CBankFileEntryUpdateData::~CBankFileEntryUpdateData() -- bank-entry update record dtor: ClearInfo (free the info buffer, zero its size), then ~Filenames
// Leaf: ??1CBankFileEntryUpdateData@@AAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CC6C0 (ctor 0x009CC6A0 adjacent).
// Identity: Ego_r.pdb ??1CBankFileEntryUpdateData@@AAE@XZ (private); the retail record has no StateBlock member.
// Callees: ClearInfo 0x009CBEE0 inlined (delete[] InfoBuffer), CSmallVector<CCharString,8>::Clear 0x009D2110 through ~CSmallVector.
// Shape: `ClearInfo();` then ~Filenames.
void __cdecl operator delete[](void* block);                 // 0x00BFEB1C (MSVCR71 ??_V thunk)

class CCharString;

template <class T, int N> class CSmallVector {              // Ego_r.pdb CSmallVector<CCharString,8>: Start +0, Size (schar) +4
public:
    CSmallVector() : Start(0), Size(0) { }
    ~CSmallVector() { Clear(); }
protected:
    void Clear();                                            // 0x009D2110 ?Clear@?$CSmallVector@VCCharString@@$07@@IAEXXZ
    T*          Start;                                       // +0x00
    signed char Size;                                        // +0x04
};

class CBankFileEntryUpdateData {
private:
    CBankFileEntryUpdateData();                              // 0x009CC6A0
    ~CBankFileEntryUpdateData();
public:
    void ClearInfo();                                        // 0x009CBEE0 ?ClearInfo@CBankFileEntryUpdateData@@QAEXXZ
    unsigned long                 StateBlockCRC;             // +0x00
    unsigned long                 InfoSize;                  // +0x04
    char*                         InfoBuffer;                // +0x08
    CSmallVector<CCharString, 8>  Filenames;                 // +0x0c
    bool                          RequiresUpdate;            // +0x14
    bool                          Exists;                    // +0x15
};

void CBankFileEntryUpdateData::ClearInfo()
{
    if (InfoBuffer) {
        delete[] InfoBuffer;
        InfoBuffer = 0;
    }
    InfoSize = 0;
}

CBankFileEntryUpdateData::~CBankFileEntryUpdateData()
{
    ClearInfo();
}