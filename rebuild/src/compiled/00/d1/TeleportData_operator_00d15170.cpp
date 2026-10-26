// TeleportData::operator=(const TeleportData& other) -- compiler-generated memberwise assignment of the script teleport record: four CCharStrings and two plain words
// Leaf: ??4TeleportData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D15170, 69 bytes (+ int3 padding).
// Callee: CCharString::operator= @ 0x0099EFB0 (x4).  Only caller: 0x00D147FD (quest-script code).
// TeleportData (Ego_r.pdb, 24 bytes): CCharString defName +0x00, ulong appearanceSeed +0x04, float
// health +0x08, CCharString scriptName +0x0c, CCharString teleportTo +0x10, CCharString
// teleportMkr +0x14.  (A 32-byte TeleportData with a vector<CCharString> itemsCarried at +0x10 exists
// in another script TU, see 0x00F0CA60.)
// Shape: the class declares no operator=, so VC7.1 emits the implicit one out of line (COMDAT) when
// a plain `a = b` caller exists in the TU: string, two dword copies, three strings, `return *this`.
class CCharString { public:
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

void AssignTeleportData(TeleportData& target, const TeleportData& source)
{
    target = source;
}