// CombatDialogueManager::AttemptToSpeak() -- when the current talker is alive, has a pending text group and a CTCTalk interface, play the AI speech; started -> stamp the group's NextAllowedFrame (frame + fps * MinTimeBetweenSpeechRepeat) and clear the pending group, final failure -> clear it
// Grade: DIFFER(ours v retail) -- 235v235: esi/edi swapped for &PCurrentTalker / &CurrentTalkerTextGroupIndex throughout, and the find()/fps/def loads are scheduled in another order (retail: find, then fps snapshot + def load, frame call; iterator temp read after _ftol)
// Leaf: AttemptToSpeak
// Retail: Fable.exe @ 0x008AF500, 235 bytes, `__thiscall`, plain `ret` (Ego_r.pdb ?AttemptToSpeak@CombatDialogueManager@@AAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020
// (inline GetInterface, type 0xF = CTCTalk, bit 15 of CThing+0x20), CTCTalk::PlayAISpeech 0x006E3B60, map::find 0x008AEED0,
// GetFrameNumber 0x004167C8 on g_pWorld 0x013B86A0, _ftol 0x00BFEA70; g_framesPerSecond 0x01375550.
// Layout (retail): SpeechGroupInfoMap +0x00 (12-byte STLport map), PDialogueDef +0x0C, PCurrentTalker +0x10,
// CurrentTalkerTextGroupIndex +0x18, LastSpeechFrame +0x1C, LastBattleCryFrame +0x20; CCombatDialogueDef
// MinTimeBetweenSpeechRepeat +0x40 (PDB +0x44).
// Tried: named iterator local + `frame() + fps * x` (229v235, frame 8); `fps * x + frame()` (same); an inline
// GetFramesPerSecond() (frame 0xC, 236v235); the find() temporary on the assignment's left (this file).
struct TSpeechGroupInfo {                   // CombatDialogueManager::TSpeechGroupInfo, Ego_r.pdb 4 bytes
    long NextAllowedFrame;                  // +0x00
};

struct SSpeechGroupNode {                   // STLport _Rb_tree_node<pair<const unsigned long, TSpeechGroupInfo> >
    bool              _M_color;             // +0x00
    SSpeechGroupNode* _M_parent;            // +0x04
    SSpeechGroupNode* _M_left;              // +0x08
    SSpeechGroupNode* _M_right;             // +0x0C
    unsigned long     first;                // +0x10 key
    TSpeechGroupInfo  second;               // +0x14
};

struct CSpeechGroupMapIterator {            // STLport _Rb_tree_iterator (non-POD: returned through a hidden pointer)
    SSpeechGroupNode* _M_node;
    CSpeechGroupMapIterator() { }
    SSpeechGroupNode* operator->() const { return _M_node; }
};

struct CSpeechGroupInfoMap {                // STLport map<unsigned long, TSpeechGroupInfo>, 12 bytes retail
    SSpeechGroupNode* _M_header;            // +0x00
    unsigned long     _M_node_count;        // +0x04
    unsigned char     _M_key_compare;       // +0x08 (empty less<unsigned long>)
    CSpeechGroupMapIterator find(const unsigned long& key) const;   // 0x008AEF20
    CSpeechGroupMapIterator find(const unsigned long& key);         // 0x008AEED0
};

struct CWorld {
    long GetFrameNumber();                  // 0x004167C8 (manifest label "GFGetPreMainMemoryUsed" is propagated)
};
extern CWorld* g_pWorld;                    // 0x013B86A0
class CBaseObject;
class CTCBase;
struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                          // +0x00
    SEntry* last;                           // +0x04
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThingCreatureBase {                  // local model (engine/CThingCreatureBase.h is all padding here)
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];        // +0x20 one bit per ETCInterfaceType
    CInterfaceMap  m_interfaces;            // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  Dead : 1;                // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                   // +0x04
    CBaseObject* GetPItem() const;          // 0x00A01B50
};
#pragma pack(pop)
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* operator->() const { return (T*)GetPItem(); }
    T* Get() const { return (T*)GetPItem(); }
};

enum EDialogueLayer { DIALOGUE_LAYER_0, DIALOGUE_LAYER_1 };
enum ESpeechPlaybackResult { SPEECH_PLAYBACK_STARTED, SPEECH_PLAYBACK_FAILED_FINAL, SPEECH_PLAYBACK_FAILED_RETRY };
class CTCTalk {
public:
    ESpeechPlaybackResult PlayAISpeech(unsigned long textGroup, bool queue, EDialogueLayer layer, bool b1, bool b2);   // 0x006E3B60
};

struct CCombatDialogueDef {                 // retail offsets (Ego_r.pdb minus 4)
    unsigned char _pad00[0x3c];
    float MinTimeBetweenSpeech;             // +0x3C
    float MinTimeBetweenSpeechRepeat;       // +0x40
    float MinDelayAfterBattleCry;           // +0x44
};
struct CDefPointerCombatDialogueDef {       // CDefPointer<const CCombatDialogueDef>
    const CCombatDialogueDef* Data;
    const CCombatDialogueDef* operator->() const { return Data; }
};
extern long g_framesPerSecond;              // 0x01375550
inline long GetFramesPerSecond() { return g_framesPerSecond; }

class CombatDialogueManager {
public:
    CSpeechGroupInfoMap                     SpeechGroupInfoMap;          // +0x00
    CDefPointerCombatDialogueDef            PDialogueDef;                // +0x0C
    CIntelligentPointer<CThingCreatureBase> PCurrentTalker;              // +0x10
    unsigned long                           CurrentTalkerTextGroupIndex; // +0x18
    long                                    LastSpeechFrame;             // +0x1C
    long                                    LastBattleCryFrame;          // +0x20
    void AttemptToSpeak();
};

void CombatDialogueManager::AttemptToSpeak()
{
    if (PCurrentTalker.Get() == 0 || PCurrentTalker->Dead)
        return;
    if (CurrentTalkerTextGroupIndex == 0)
        return;
    CTCTalk* talk;
    if (!PCurrentTalker->GetInterface(&talk, 0xf))
        return;
    ESpeechPlaybackResult result = talk->PlayAISpeech(CurrentTalkerTextGroupIndex, false, DIALOGUE_LAYER_1, false, false);
    if (result == SPEECH_PLAYBACK_STARTED)
    {
        SpeechGroupInfoMap.find(CurrentTalkerTextGroupIndex)->second.NextAllowedFrame = (long)(g_pWorld->GetFrameNumber() + GetFramesPerSecond() * PDialogueDef->MinTimeBetweenSpeechRepeat);
        CurrentTalkerTextGroupIndex = 0;
    }
    else if (result == SPEECH_PLAYBACK_FAILED_FINAL)
        CurrentTalkerTextGroupIndex = 0;
}
