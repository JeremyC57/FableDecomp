// CombatDialogueManager::IsSpeechAllowed(const CThingCreatureBase& creature) const -- no while another talker is alive; otherwise only if the hero is alive, its interface-0x36 list holds the creature, and enough seconds have passed since the last speech and the last battle cry
// Leaf: IsSpeechAllowed
// Retail: Fable.exe @ 0x008AF5F0, 239 bytes, `__fastcall`, `ret 4` (Ego_r.pdb ?IsSpeechAllowed@CombatDialogueManager@@QBE_NABVCThingCreatureBase@@@Z;
// called by the landed PlayBattleCry 0x008AF850).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2), CPlayerManager::GetMainPlayer 0x00449970 (g_pWorld+0x1C),
// CPlayer::GetHeroThing 0x00487DC0, the const CVectorMap::LowerBound(const long&) 0x004365B0 (inline GetInterface, type 0x36 =
// bit 22 of CThing+0x24), the type-0x36 component's list membership test 0x00747450 (std::find over +0x38..+0x3C != end;
// manifest label "CTCBuyableHouse::MovedInNotification" is propagated; component class named descriptively),
// GetFrameNumber 0x004167C8 on g_pWorld 0x013B86A0; g_framesPerSecond 0x01375550.
// Layout (retail): PDialogueDef +0x0C, PCurrentTalker +0x10, LastSpeechFrame +0x1C, LastBattleCryFrame +0x20;
// CCombatDialogueDef MinTimeBetweenSpeech +0x3C, MinDelayAfterBattleCry +0x44 (PDB +0x40/+0x48).
// Shape: `(float)g_framesPerSecond` written at each use keeps one fild and two `fdiv` (a float local turns them into a
// reciprocal multiply); the two time tests as one `!(a < x) && !(b < y)` guard on `return true` give retail's `jnp fail`
// with the success path falling through (two `if (a < x) return false;` invert the first branch).
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

class CWorld { public:
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
    virtual void v00();
    CBaseObject* m_pItem;                   // +0x04
    CBaseObject* GetPItem() const;          // 0x00A01B50
};
#pragma pack(pop)
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* operator->() const { return (T*)GetPItem(); }
    T* Get() const { return (T*)GetPItem(); }
};

enum EDialogueLayer { DIALOGUE_LAYER_0, DIALOGUE_LAYER_1 };
enum ESpeechPlaybackResult { SPEECH_PLAYBACK_STARTED, SPEECH_PLAYBACK_FAILED_FINAL, SPEECH_PLAYBACK_FAILED_RETRY };
class CTCTalk {
public:
    ESpeechPlaybackResult PlayAISpeech(unsigned long textGroup, bool queue, EDialogueLayer layer, bool b1, bool b2);   // 0x006E3B60
};

class CCombatDialogueDef { public:                 // retail offsets (Ego_r.pdb minus 4)
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
    bool IsSpeechAllowed(const CThingCreatureBase& creature) const;
};

bool CombatDialogueManager::IsSpeechAllowed(const CThingCreatureBase& creature) const
{
    if (PCurrentTalker.Get() && !PCurrentTalker->Dead)
        return false;
    CThingCreatureBase* hero = ((CWorldPlayers*)g_pWorld)->PlayerManager->GetMainPlayer()->GetHeroThing();
    if (hero == 0 || hero->Dead)
        return false;
    CTCCombatSpeechTargets* targets;
    if (!hero->GetInterface(&targets, 0x36))
        return false;
    if (!targets->ContainsCreature(creature))
        return false;
    long frame = g_pWorld->GetFrameNumber();
    if (!((frame - LastSpeechFrame) / (float)g_framesPerSecond < PDialogueDef->MinTimeBetweenSpeech)
        && !((frame - LastBattleCryFrame) / (float)g_framesPerSecond < PDialogueDef->MinDelayAfterBattleCry))
        return true;
    return false;
}