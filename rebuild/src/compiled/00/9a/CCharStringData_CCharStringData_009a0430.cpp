// CCharStringData::~CCharStringData() -- destroys the shared string block's CBasicString<char>: frees the buffer (array delete), zeroes the lengths, keeps the UseFastExtend bit
// Leaf: ??1CCharStringData@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A0430, 44 bytes (+ int3 padding). Callee: operator delete[] @ 0x00BFEB1C (MSVCR71 ??_V).
// Identity: the manifest label LHHttpDocumentParts::~LHHttpDocumentParts is propagated (wrong class: no +8 word there). The
// row has no callers and is a byte twin of 0x009A0190 (modelled as CBasicString<char>::~CBasicString) inside the same
// CBasicString<char> TU block. Ego_r.pdb CCharStringData (17 bytes) = CBasicString<char> Data +0 and long NoRefs +0x0D; the
// CCharString release 0x0099E9B0 inlines exactly this destructor before `operator delete`. Taking the wrapper's destructor
// for this copy gives each retail address its own symbol; which twin is which is not proven.
// Shape: the member's inline destructor is the whole body (NoRefs has none).
void __cdecl operator delete[](void* block);       // 0x00BFEB1C

#pragma pack(push, 1)
template <class T> class CBasicString { public:
    T*            PData;                          // +0x00
    unsigned long StringLength;                   // +0x04
    unsigned long DataLength : 31;                // +0x08
    bool          UseFastExtend : 1;              // +0x0C
    ~CBasicString()
    {
        if (PData) {
            delete[] PData;
            PData = 0;
        }
        StringLength = 0;
        DataLength = 0;
    }
};

class CCharStringData { public:
    CBasicString<char> Data;                      // +0x00
    long               NoRefs;                    // +0x0D
    ~CCharStringData();
};
#pragma pack(pop)

CCharStringData::~CCharStringData()
{
}