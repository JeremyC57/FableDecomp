// CTestQuest::CTestQuest() -- default-construct a test-quest record: the quest, HSP, display, ini, outi and quest-card-def names (the colour index is left unset)
// Leaf: ??0CTestQuest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004A7710, length 52 bytes (+ int3 padding), returns this; no call or address
// references (an unreferenced COMDAT, as is its destructor 0x004A7750 right after it: five CCharString dtors +0x18..+4 and
// the +0 one as the tail). The manifest label (_Dest_val<allocator<CEditLevelMerger::CConflict>>) is propagated and wrong.
// Layout: Ego_r.pdb CTestQuest (0x1c) = CCharString QuestName +0, HSPName +4, long ColourIndex +8, CCharString DisplayName
// +0xc, IniFile +0x10, OutiFile +0x14, QuestCardDefName +0x18 -- the only PDB class with strings at exactly these offsets.
// Callees: CCharString::CCharString() @ 0x0099E4B0 (x6).
// Shape: the implicit default constructor (members in declaration order, the long uninitialised) emitted out of line by
// constructing one in this TU through placement new; `this` stays in esi and comes back in eax.
class CCharString {
public:
    void* PStringData;
    CCharString();                                           // 0x0099E4B0
    ~CCharString();                                          // 0x0099EAE0
};

class CTestQuest {                                           // 0x1c bytes
public:
    CCharString QuestName;                                   // +0x00
    CCharString HSPName;                                     // +0x04
    long        ColourIndex;                                 // +0x08
    CCharString DisplayName;                                 // +0x0c
    CCharString IniFile;                                     // +0x10
    CCharString OutiFile;                                    // +0x14
    CCharString QuestCardDefName;                            // +0x18
};

inline void* operator new(unsigned int, void* where) { return where; }

CTestQuest* ConstructTestQuest(void* where)                  // emits the implicit constructor; not part of the retail body
{
    return new (where) CTestQuest;
}