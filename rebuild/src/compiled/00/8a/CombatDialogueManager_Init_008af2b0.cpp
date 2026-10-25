// CombatDialogueManager::Init() -- fetch the COMBAT_DIALOGUE_DEF_INSTANCE def into PDialogueDef, then register every combat speech group the def names (EnemySighted .. Flee) in the speech-group map with a zero NextAllowedFrame
// Leaf: ?Init@CombatDialogueManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AF2B0, 588 bytes (+ int3 padding), plain `ret`, ebp frame aligned to 8.
// Ego_r.pdb: ?Init@CombatDialogueManager@@QAEXXZ; CombatDialogueManager { map<unsigned long, TSpeechGroupInfo> SpeechGroupInfoMap
//   +0 (retail STLport map, 12 bytes), CDefPointer<const CCombatDialogueDef> PDialogueDef (PDB +0x10, retail +0xC), ... };
//   CCombatDialogueDef speech groups EnemySighted .. Flee (PDB +0x4C .. +0x8C, retail 4 lower: +0x48 .. +0x88).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("COMBAT_DIALOGUE_DEF_INSTANCE" 0x01278688), ~CCharString
//   @ 0x0099EAE0, CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDef<CCombatDialogueDef>(const CCharString&,
//   CDefPointer*) @ 0x008AEE60, map<unsigned long, TSpeechGroupInfo>::insert(const value_type&) @ 0x008AFD00 (17 times;
//   pair<iterator, bool> through the hidden pointer, discarded).
// Shape: the def name is an implicit (lea-passed) CCharString temporary built before the pushes; each insert builds its
//   {group, {0}} pair in the same 8-byte frame slot (the zero kept in ebx), which with the 8-byte insert result gives the
//   `and esp,-8` frame. Speed-optimised (default) TU.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CCombatDialogueDef {                             // retail layout (Ego_r.pdb offsets - 4)
public:
    unsigned char _pad00[0x3c];
    float MinTimeBetweenSpeech;                        // +0x3C
    float MinTimeBetweenSpeechRepeat;                  // +0x40
    float MinDelayAfterBattleCry;                      // +0x44
    long  EnemySighted;                                // +0x48
    long  RunForCover;                                 // +0x4C
    long  Hiding;                                      // +0x50
    long  Ambushing;                                   // +0x54
    long  WillSeenUsed;                                // +0x58
    long  Threat;                                      // +0x5C
    long  Encourage;                                   // +0x60
    long  FoeKnockedDownByColleague;                   // +0x64
    long  MenLosing;                                   // +0x68
    long  EnemyKnockedDown;                            // +0x6C
    long  RangedCombatAiming;                          // +0x70
    long  RangedCombatAimingBlocked;                   // +0x74
    long  Injured;                                     // +0x78
    long  ReactionToDecapitation;                      // +0x7C
    long  ColleagueInjured;                            // +0x80
    long  Scared;                                      // +0x84
    long  Flee;                                        // +0x88
};
template <class T> struct CDefPointer {
    T* Data;
    T* operator->() const { return Data; }
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
    bool GetDef(const CCharString& name, CDefPointer<const CCombatDialogueDef>* out);   // 0x008AEE60
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
}
struct TSpeechGroupInfo {                              // CombatDialogueManager::TSpeechGroupInfo, 4 bytes
    long NextAllowedFrame;                             // +0x00
    TSpeechGroupInfo() : NextAllowedFrame(0) { }
};
struct CSpeechGroupMapIterator {                       // STLport _Rb_tree_iterator (non-POD)
    void* _M_node;
    CSpeechGroupMapIterator() { }
};
struct CSpeechGroupInfoMap {                           // STLport map<unsigned long, TSpeechGroupInfo>, 12 bytes retail
    void*         _M_header;                           // +0x00
    unsigned long _M_node_count;                       // +0x04
    unsigned char _M_key_compare;                      // +0x08
    std::pair<CSpeechGroupMapIterator, bool> insert(const std::pair<const unsigned long, TSpeechGroupInfo>& value);   // 0x008AFD00
};

class CombatDialogueManager {
public:
    CSpeechGroupInfoMap SpeechGroupInfoMap;            // +0x00
    CDefPointer<const CCombatDialogueDef> PDialogueDef;   // +0x0C
    void AddSpeechGroup(unsigned long group)
    {
        SpeechGroupInfoMap.insert(std::pair<const unsigned long, TSpeechGroupInfo>(group, TSpeechGroupInfo()));
    }
    void Init();
};

void CombatDialogueManager::Init()
{
    CGameDefinitionManager::Get().GetDef("COMBAT_DIALOGUE_DEF_INSTANCE", &PDialogueDef);
    AddSpeechGroup(PDialogueDef->EnemySighted);
    AddSpeechGroup(PDialogueDef->RunForCover);
    AddSpeechGroup(PDialogueDef->Hiding);
    AddSpeechGroup(PDialogueDef->Ambushing);
    AddSpeechGroup(PDialogueDef->WillSeenUsed);
    AddSpeechGroup(PDialogueDef->Threat);
    AddSpeechGroup(PDialogueDef->Encourage);
    AddSpeechGroup(PDialogueDef->FoeKnockedDownByColleague);
    AddSpeechGroup(PDialogueDef->MenLosing);
    AddSpeechGroup(PDialogueDef->EnemyKnockedDown);
    AddSpeechGroup(PDialogueDef->RangedCombatAiming);
    AddSpeechGroup(PDialogueDef->RangedCombatAimingBlocked);
    AddSpeechGroup(PDialogueDef->Injured);
    AddSpeechGroup(PDialogueDef->ReactionToDecapitation);
    AddSpeechGroup(PDialogueDef->ColleagueInjured);
    AddSpeechGroup(PDialogueDef->Scared);
    AddSpeechGroup(PDialogueDef->Flee);
}