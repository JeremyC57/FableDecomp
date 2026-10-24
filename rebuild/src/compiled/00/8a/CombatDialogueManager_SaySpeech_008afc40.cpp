// CombatDialogueManager::SaySpeech(CThingCreatureBase& creature, unsigned long textGroup) -- if the creature may say the group, make it the current talker with that pending group and try to speak at once
// Leaf: SaySpeech
// Retail: Fable.exe @ 0x008AFC40, 113 bytes, `__fastcall`, `ret 8` (Ego_r.pdb ?SaySpeech@CombatDialogueManager@@QAE_NAAVCThingCreatureBase@@K@Z).
// Callees: CanSaySpeech 0x008AF6E0, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 /
// dtor 0x00A01C10, AttemptToSpeak 0x008AF500.
// Layout (retail): SpeechGroupInfoMap +0x00 (12-byte STLport map), PDialogueDef +0x0C, PCurrentTalker +0x10
// (CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8), CurrentTalkerTextGroupIndex +0x18, LastSpeechFrame +0x1C,
// LastBattleCryFrame +0x20. CThing interface bits at +0x20, the interface CVectorMap at +0x44, Dead = +0x91 bit 0.
// Shape: the success body inside a positive `if (CanSaySpeech(...))` (retail's `je fail` to the tail); the talker is set by
// assigning a temporary CIntelligentPointer<CThingCreatureBase>(&creature) (inline ctor: base ctor, own vtable, SetPItem).
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
    SEntry* LowerBound(const long& type) const;   // 0x004365B0
};
class CThingCreatureBase {                  // local model (engine/CThingCreatureBase.h is all padding here)
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];        // +0x20 one bit per ETCInterfaceType
    CInterfaceMap  m_interfaces;            // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  Dead : 1;                // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
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
    CBaseObject* m_pItem;                   // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();     // 0x00A01C10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};
#pragma pack(pop)
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    virtual void Touch() { }
    T* operator->() const { return (T*)GetPItem(); }
    T* Get() const { return (T*)GetPItem(); }
};

enum EDialogueLayer { DIALOGUE_LAYER_0, DIALOGUE_LAYER_1 };
enum ESpeechPlaybackResult { SPEECH_PLAYBACK_STARTED, SPEECH_PLAYBACK_FAILED_FINAL, SPEECH_PLAYBACK_FAILED_RETRY };
class CTCTalk {
public:
    bool IsTalking() const;                                  // 0x006E28A0
    bool DoesSpeechExist(unsigned long textGroup);           // 0x006E2910
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
class CPlayer {
public:
    CThingCreatureBase* GetHeroThing() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer();                   // 0x00449970
};
class CWorldPlayers {                           // g_pWorld viewed for its player manager (+0x1C)
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;              // +0x1C
};
class CTCCombatSpeechTargets {                  // interface type 0x36 (bit 22 of CThing+0x24): a creature list at +0x38/+0x3C
public:
    bool ContainsCreature(const CThingCreatureBase& creature);   // 0x00747450 (std::find over the list != end)
};
class CombatDialogueManager {
public:
    CSpeechGroupInfoMap                     SpeechGroupInfoMap;          // +0x00
    CDefPointerCombatDialogueDef            PDialogueDef;                // +0x0C
    CIntelligentPointer<CThingCreatureBase> PCurrentTalker;              // +0x10
    unsigned long                           CurrentTalkerTextGroupIndex; // +0x18
    long                                    LastSpeechFrame;             // +0x1C
    long                                    LastBattleCryFrame;          // +0x20
    bool IsSpeechGroupAllowed(unsigned long group) const
    {
        const long frame = g_pWorld->GetFrameNumber();
        CSpeechGroupMapIterator it = SpeechGroupInfoMap.find(group);
        bool allowed = frame >= it->second.NextAllowedFrame;
        return allowed;
    }
    bool IsSpeechAllowed(const CThingCreatureBase& creature) const;      // 0x008AF5F0
    bool CanSaySpeech(CThingCreatureBase& creature, unsigned long textGroup) const;
    void AttemptToSpeak();                                               // 0x008AF500
    void Update();
    bool SaySpeech(CThingCreatureBase& creature, unsigned long textGroup);
};

bool CombatDialogueManager::SaySpeech(CThingCreatureBase& creature, unsigned long textGroup)
{
    if (CanSaySpeech(creature, textGroup))
    {
        PCurrentTalker = CIntelligentPointer<CThingCreatureBase>(&creature);
        CurrentTalkerTextGroupIndex = textGroup;
        AttemptToSpeak();
        return true;
    }
    return false;
}