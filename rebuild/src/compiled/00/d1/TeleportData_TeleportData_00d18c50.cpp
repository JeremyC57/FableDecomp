// TeleportData::TeleportData(const TeleportData& other) -- compiler-generated memberwise copy constructor of the script teleport record: four CCharStrings and two plain words
// Leaf: ??0TeleportData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D18C50, 69 bytes (+ int3 padding).
// Callee: CCharString copy ctor @ 0x0099EC30 (x4).  Callers: 0x00D18C15, 0x00D18D17, 0x00D18D3A,
// 0x00D18D57, 0x00D18D81 (quest-script code; the assignment twin is 0x00D15170).
// TeleportData (Ego_r.pdb, 24 bytes): CCharString defName +0x00, ulong appearanceSeed +0x04, float
// health +0x08, CCharString scriptName +0x0c, CCharString teleportTo +0x10, CCharString
// teleportMkr +0x14.  (A 32-byte TeleportData with a vector<CCharString> itemsCarried at +0x10 exists
// in another script TU, see 0x00F0CA60.)
// Shape: the class declares no copy constructor, so VC7.1 emits the implicit one out of line (COMDAT)
// for a copy-construction in the TU: string, two dword copies, three strings, `return this`.
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct TeleportData {
    CCharString defName;                        // +0x00
    unsigned long appearanceSeed;               // +0x04
    float health;                               // +0x08
    CCharString scriptName;                     // +0x0c
    CCharString teleportTo;                     // +0x10
    CCharString teleportMkr;                    // +0x14
};

inline void* operator new(unsigned int, void* where) { return where; }

TeleportData* CopyTeleportData(void* where, const TeleportData& source)
{
    return new (where) TeleportData(source);
}