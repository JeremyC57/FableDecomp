// CTestQuest::operator=(const CTestQuest& other) -- assignment: the six CCharString members assigned in order and ColourIndex copied; returns *this
// Leaf: ??4CTestQuest
// Retail: Fable.exe @ 0x004A8F50, 87 bytes (+ int3 padding). Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0 (six times).
// Layout: PDB CTestQuest (0x1c bytes, no vtable): QuestName +0, HSPName +4, ColourIndex +8, DisplayName +0xc,
// IniFile +0x10, OutiFile +0x14, QuestCardDefName +0x18 (no engine header; modelled locally with the PDB names).
// Shape: memberwise in declaration order with the source pointer in edi; the ColourIndex dword is copied between the
// second and third string calls, its load scheduled before the `lea` of the next source member; the last source address
// is formed with `add edi,18h`. Written out by hand: the DDK out-lines the compiler-generated
// copy/assignment of a record whose members have their own (out-of-line) copy operations.
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);             // 0x0099EC30
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

class CTestQuest {
public:
    CCharString QuestName;                         // +0x00
    CCharString HSPName;                           // +0x04
    long        ColourIndex;                       // +0x08
    CCharString DisplayName;                       // +0x0c
    CCharString IniFile;                           // +0x10
    CCharString OutiFile;                          // +0x14
    CCharString QuestCardDefName;                  // +0x18
    CTestQuest(const CTestQuest& other);
    CTestQuest& operator=(const CTestQuest& other);
};

CTestQuest& CTestQuest::operator=(const CTestQuest& other)
{
    QuestName = other.QuestName;
    HSPName = other.HSPName;
    ColourIndex = other.ColourIndex;
    DisplayName = other.DisplayName;
    IniFile = other.IniFile;
    OutiFile = other.OutiFile;
    QuestCardDefName = other.QuestCardDefName;
    return *this;
}
