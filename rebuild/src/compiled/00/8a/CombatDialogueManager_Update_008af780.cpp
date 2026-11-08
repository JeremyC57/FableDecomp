// CombatDialogueManager::Update() -- while a live talker exists: retry its pending text group, or once it has stopped talking (or has no CTCTalk) drop it and stamp LastSpeechFrame
// Leaf: Update
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x008AF780, 204 bytes, `__fastcall`, plain `ret` (Ego_r.pdb ?Update@CombatDialogueManager@@QAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), AttemptToSpeak 0x008AF500, CVectorMap::LowerBound(const long*)
// 0x0040F020 (inline GetInterface, type 0xF = CTCTalk), CTCTalk::IsTalking 0x006E28A0, the intelligent-pointer ctor 0x00A01B10,
// SetPItem 0x00A01B90, dtor 0x00A01C10, CMainGameComponent::GetCurrentWorldFrame 0x004167C8 on CMainGameComponent::Get() 0x013B86A0.
// Layout (retail): SpeechGroupInfoMap +0x00 (12-byte STLport map), PDialogueDef +0x0C, PCurrentTalker +0x10
// (CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8), CurrentTalkerTextGroupIndex +0x18, LastSpeechFrame +0x1C,
// LastBattleCryFrame +0x20. CThing interface bits at +0x20, the interface CVectorMap at +0x44, Dead = +0x91 bit 0.
// Shape: `if (GetInterface(&talk, 0xF) && talk->IsTalking()) return;` (esi pushed only inside the interface branch); the reset
// assigns a temporary CIntelligentPointer<CThingCreatureBase>(0).
#include "fable_compiler.h"
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

class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    long GetCurrentWorldFrame() const;             // 0x004167C8 (Ego_r.pdb ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ)
};
class CBaseObject;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingCreatureBase {                  // local model (engine/CThingCreatureBase.h is all padding here)
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];        // +0x20 one bit per ETCInterfaceType
    CInterfaceMap  m_interfaces;            // +0x44
    unsigned char  _pad54[0x3d];
    unsigned char  Dead : 1;                // +0x91 bit 0
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThingCreatureBase { };

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;                   // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();     // 0x00A01C10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};
#pragma pack(pop)
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8
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
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CWorldPlayers {                           // CMainGameComponent::Get() viewed for its player manager (+0x1C)
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
        const long frame = CMainGameComponent::Get()->GetCurrentWorldFrame();
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

void CombatDialogueManager::Update()
{
    if (PCurrentTalker.Get() == 0 || PCurrentTalker->Dead)
        return;
    if (CurrentTalkerTextGroupIndex != 0)
    {
        AttemptToSpeak();
        return;
    }
    CTCTalk* talk;
    if (PCurrentTalker->GetInterface(&talk, 0xf) && talk->IsTalking())
        return;
    PCurrentTalker = CIntelligentPointer<CThingCreatureBase>(0);
    LastSpeechFrame = CMainGameComponent::Get()->GetCurrentWorldFrame();
}