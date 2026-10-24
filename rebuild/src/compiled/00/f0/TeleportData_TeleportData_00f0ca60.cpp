// TeleportData::TeleportData(const TeleportData& other) -- compiler-generated memberwise copy constructor of the 32-byte script teleport record: two CCharStrings, two words and the carried-items vector
// Leaf: ??0TeleportData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0CA60, 57 bytes (+ int3 padding).
// Callees: CCharString copy ctor @ 0x0099EC30 (x2), vector<CCharString> copy ctor @ 0x007ACEA0
// (out of line; manifest "Vector_Copy").  Callers: 0x00F0C607, 0x00F0D4A5, 0x00F0D557, 0x00F0D57A,
// 0x00F0D597, 0x00F0D5C1 (quest-script code).
// TeleportData (Ego_r.pdb, the 32-byte variant): CCharString defName +0x00, ulong appearanceSeed
// +0x04, float health +0x08, CCharString scriptName +0x0c, vector<CCharString> itemsCarried +0x10
// (VC7.1 Dinkumware: allocator word, first, last, end).  The 24-byte variant (teleportTo /
// teleportMkr strings) is 0x00D18C50 / 0x00D15170 in another script TU.
// Shape: the class declares no copy constructor, so VC7.1 emits the implicit one out of line (COMDAT)
// for a copy-construction in the TU; `return this`.
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

template<class T> class vector {
public:
    unsigned char _Alval;                       // +0x0 (allocator, padded)
    T* _Myfirst;                                // +0x4
    T* _Mylast;                                 // +0x8
    T* _Myend;                                  // +0xc
    vector(const vector& other);                // 0x007ACEA0 for T = CCharString
    ~vector();
};

struct TeleportData {
    CCharString defName;                        // +0x00
    unsigned long appearanceSeed;               // +0x04
    float health;                               // +0x08
    CCharString scriptName;                     // +0x0c
    vector<CCharString> itemsCarried;           // +0x10
};

inline void* operator new(unsigned int, void* where) { return where; }

TeleportData* CopyTeleportData(void* where, const TeleportData& source)
{
    return new (where) TeleportData(source);
}