// CombatDialogueManager::GetBestSpeechForCreature(const CThingCreatureBase& creature, const CThingCreatureBase& target, const CCombatManager& combatManager) const -- the combat speech group the creature should say now, or 0 when it may not speak: decapitation reaction (the target's last hit decapitated), enemy knocked down, threat (an expression was performed to it), will seen used (a type-0x44 world message exists), colleague injured (another attacker of the target is knocked down), injured (health under 15%), scared (the target is an AI creature, a 50% roll and the creature's attitude to the player is 4), else threat
// Grade: DIFFER(687v688) -- 1 grade, ~30 ungraded variants (b1-b17n, m1-m7, n1-n11, r1-r3). Frame (sub esp,0Ch), the inline first IsSpeechGroupAllowed (key copied to the frame), the colleague loop and every return path match; three local differences remain: (1) the target's CTCEntityEvents pointer lives in eax (`mov cl,[edi+28h]; xor eax,eax` ... `mov ecx,eax` before the call) where retail colours it ecx (`mov al,[edi+28h]; xor ecx,ecx`, `mov ecx,[eax+4]`), an extra `mov ecx,eax`; (2) both GetEvent calls place the out pointer in the dead find-iterator word L2 and the empty CEventFilter_None in the dead `target` slot, where retail puts the out pointer in the `target` slot (F+24h) and overlaps the empty filter on the live `creature` slot (F+20h); (3) the will-seen-used block loads g_pMainGameComponent into ecx/edx and the group key into eax where retail uses edx/ecx. Tried: named/wrapper/helper filters, out-var scoping and order, hit zero-init, split &&, manual vs inline first check, the real GetEvent template body as a same-TU noinline stand-in (filter moves to the creature slot but the frame grows to 10h/14h), named message filter and manager local, three list-ctor shapes. Next: model the same-TU find 0x008AEF20 and GetEvent bodies together (both are instantiated in this TU) so the slot packer sees the real escape picture.
// Leaf: ?GetBestSpeechForCreature@CombatDialogueManager@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AF990, 688 bytes, thiscall `ret 0Ch`.
// Ego_r.pdb: ?GetBestSpeechForCreature@CombatDialogueManager@@QBEKABVCThingCreatureBase@@0ABVCCombatManager@@@Z.
// Callees: CombatDialogueManager::IsSpeechAllowed 0x008AF5F0 / IsSpeechGroupAllowed 0x008AF1E0 (landed; its body is in this TU
//   and VC7.1 expands it at the first test only) / the speech map's find 0x008AEF20; the const CVectorMap::LowerBound 0x004365B0
//   (inline GetInterface, type 0x42 CTCEntityEvents = bit 2 of CThing+0x28); CTCEntityEvents::GetEvent<NEntityEvents::
//   CEventHitThing, CEventFilter_None> 0x008AF020 and GetEvent<NEntityEvents::CEventExpressionPerformedTo, CEventFilter_None>
//   0x008AF0A0 (manifest "GetEvent<CEventHitBy,...>" propagated); IsKnockedDown 0x0085F010 (`__fastcall`); malloc 0x00BFEA0E,
//   CMessageEventManager::FindAllMessages<CMessageEventFilter_IsOfType> 0x00493EAB, list<const CMessageEvent*>::~list 0x00493BDD;
//   CCombatManager::GetCombatantInfo 0x006E9360 and the attacker-map iterator ++ 0x006EF790; the creature's health fraction
//   0x006A4E40; CWorld::GetPRandomSeed 0x0049D810, GFROR13 0x00497890; COpinionReactionManager::GetThingAttitudeToPlayer
//   0x00701840; the frame getter 0x004167C8 on g_pMainGameComponent 0x013B86A0. Literal 0.15f at 0x012786A8.
// Layout (retail): CombatDialogueManager as the landed IsSpeechAllowed 0x008AF5F0; CCombatDialogueDef groups at Ego_r.pdb - 4
//   (as the landed Init 0x008AF2B0); CMainGameComponent PWorld +0x24; CWorld PMessageEventManager +0x60, POpinionReactionManager
//   +0x74 (engine/CWorld.h); CThing m_type byte +0x10 (2 = an AI creature); CEventHitThing Decapitate +0x14 (Ego_r.pdb);
//   CCombatantInfo DefenderInfo.Attackers +0x34 (map<unsigned __int64, CThingCreatureBase*>, as the landed 0x00979910).
// Shape: the entity-events pointers start at 0 and are read through the inline GetInterface (unchecked, as retail); the filters
//   are empty-class temporaries with user ctors (no byte store; VC7.1 lets the address-taken empty temporary overlap the live
//   `creature` slot); the message list is a block local with the out-of-line dtor; the 50% roll is the inline GFRandom.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
unsigned long __fastcall GFROR13(unsigned long value); // 0x00497890

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

// ---- world ----
struct CMessageEvent;
struct CMessageEventFilter_IsOfType {
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
struct SMessagePtrNode {
    SMessagePtrNode*     m_pNext;
    SMessagePtrNode*     m_pPrev;
    const CMessageEvent* m_value;           // +0x08
};
struct CMessagePtrList {                    // STLport list<const CMessageEvent*>: the sentinel pointer
    SMessagePtrNode* m_pHead;
    CMessagePtrList()
    {
        SMessagePtrNode* node = (SMessagePtrNode*)malloc(sizeof(SMessagePtrNode));
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pHead = node;
    }
    ~CMessagePtrList();                     // 0x00493BDD
};
struct CMessageEventManager {
    template<class TFilter> long FindAllMessages(const TFilter& filter, CMessagePtrList& found);   // 0x00493EAB
};
class CThing;
enum EOpinionAttitudeType { OPINION_ATTITUDE_4 = 4 };
struct COpinionReactionManager {
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;   // 0x00701840
};
struct CWorld {
    unsigned char            _pad00[0x60];
    CMessageEventManager*    PMessageEventManager;      // +0x60 CCountedPointer<CMessageEventManager>::Data
    unsigned char            _pad64[0x74 - 0x64];
    COpinionReactionManager* POpinionReactionManager;   // +0x74 CCountedPointer<COpinionReactionManager>::Data
    unsigned long* GetPRandomSeed();                    // 0x0049D810
};
struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CWorld*       PWorld;                   // +0x24
    long GetFrameNumber();                  // 0x004167C8
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

__forceinline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

// ---- things ----
class CTCBase;
struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                          // +0x00
    SEntry* last;                           // +0x04
    SEntry* LowerBound(const long& type) const;   // 0x004365B0
};
namespace NEntityEvents {
struct CEventHitThing {                     // event type 0x1d (Ego_r.pdb CEventHitThing, 0x18 bytes)
    unsigned char _pad00[0x14];
    bool          Decapitate;               // +0x14
};
struct CEventExpressionPerformedTo;         // event type 3
}
struct CEventFilter_None {                  // empty filter (user ctor: no byte store)
    CEventFilter_None() { }
};
struct CTCEntityEvents {                    // interface type 0x42
    template <class TEvent, class TFilter> bool GetEvent(const TEvent** out, const TFilter& filter) const;   // 0x008AF020 / 0x008AF0A0
};
class CThing {
public:
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                  // +0x10 (1 = the player creature, 2 = an AI creature)
    unsigned char  _pad11[0x20 - 0x11];
    unsigned long  m_presentBits[9];        // +0x20 one bit per ETCInterfaceType
    CInterfaceMap  m_interfaces;            // +0x44
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
};
class CThingCreatureBase : public CThing {
public:
    float GetHealthFraction() const;        // 0x006A4E40
};
bool __fastcall IsKnockedDown(const CThingCreatureBase& creature);   // 0x0085F010

// ---- combat manager ----
struct CAttackerNode {                      // STLport node of map<unsigned __int64, CThingCreatureBase*>
    long                _M_color;           // +0x00
    CAttackerNode*      _M_parent;          // +0x04
    CAttackerNode*      _M_left;            // +0x08
    CAttackerNode*      _M_right;           // +0x0c
    unsigned __int64    first;              // +0x10 the attacker's unique id
    CThingCreatureBase* second;             // +0x18 the attacker
};
struct CAttackerIterator {
    CAttackerNode* _M_node;
    CAttackerIterator(CAttackerNode* node) : _M_node(node) { }
    bool operator!=(const CAttackerIterator& other) const { return _M_node != other._M_node; }
    CAttackerIterator& operator++();        // 0x006EF790
    CAttackerNode* operator->() const { return _M_node; }
};
struct CAttackerMap {
    CAttackerNode* _M_header;               // +0x00 header node (left = leftmost)
    unsigned long  _M_node_count;           // +0x04
    CAttackerIterator begin() const { return CAttackerIterator(_M_header->_M_left); }
    CAttackerIterator end() const { return CAttackerIterator(_M_header); }
};
struct CCombatantInfo {
    unsigned char _pad00[0x34];             // Parent, AttackerInfo (retail 0x30 bytes)
    CAttackerMap  Attackers;                // +0x34 DefenderInfo.Attackers
};
struct CCombatManager {
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const;   // 0x006E9360
};

// ---- the manager ----
struct CCombatDialogueDef {                 // retail offsets (Ego_r.pdb minus 4)
    unsigned char _pad00[0x48];
    unsigned long EnemySighted;             // +0x48
    unsigned long RunForCover;              // +0x4C
    unsigned long Hiding;                   // +0x50
    unsigned long Ambushing;                // +0x54
    unsigned long WillSeenUsed;             // +0x58
    unsigned long Threat;                   // +0x5C
    unsigned long Encourage;                // +0x60
    unsigned long FoeKnockedDownByColleague;   // +0x64
    unsigned long MenLosing;                // +0x68
    unsigned long EnemyKnockedDown;         // +0x6C
    unsigned long RangedCombatAiming;       // +0x70
    unsigned long RangedCombatAimingBlocked;   // +0x74
    unsigned long Injured;                  // +0x78
    unsigned long ReactionToDecapitation;   // +0x7C
    unsigned long ColleagueInjured;         // +0x80
    unsigned long Scared;                   // +0x84
    unsigned long Flee;                     // +0x88
};
struct CDefPointerCombatDialogueDef {       // CDefPointer<const CCombatDialogueDef>
    const CCombatDialogueDef* Data;
    const CCombatDialogueDef* operator->() const { return Data; }
};
struct CIntelligentPointerCreature {        // CIntelligentPointer<CThingCreatureBase>, 8 bytes
    void* vptr;
    void* m_pItem;
};
class CombatDialogueManager {
public:
    CSpeechGroupInfoMap          SpeechGroupInfoMap;          // +0x00
    CDefPointerCombatDialogueDef PDialogueDef;                // +0x0C
    CIntelligentPointerCreature  PCurrentTalker;              // +0x10
    unsigned long                CurrentTalkerTextGroupIndex; // +0x18
    long                         LastSpeechFrame;             // +0x1C
    long                         LastBattleCryFrame;          // +0x20
    bool IsSpeechGroupAllowed(unsigned long group) const;
    bool IsSpeechAllowed(const CThingCreatureBase& creature) const;   // 0x008AF5F0
    unsigned long GetBestSpeechForCreature(const CThingCreatureBase& creature, const CThingCreatureBase& target,
                                           const CCombatManager& combatManager) const;
};

// The landed IsSpeechGroupAllowed 0x008AF1E0 (same TU, above): not landed from here.
bool CombatDialogueManager::IsSpeechGroupAllowed(unsigned long group) const
{
    const long frame = g_pMainGameComponent->GetFrameNumber();
    CSpeechGroupMapIterator it = SpeechGroupInfoMap.find(group);
    bool allowed = frame >= it->second.NextAllowedFrame;
    return allowed;
}

unsigned long CombatDialogueManager::GetBestSpeechForCreature(const CThingCreatureBase& creature, const CThingCreatureBase& target,
                                                              const CCombatManager& combatManager) const
{
    if (!IsSpeechAllowed(creature))
        return 0;
    CTCEntityEvents* events = 0;
    creature.GetInterface(&events, 0x42);
    if (IsSpeechGroupAllowed(PDialogueDef->ReactionToDecapitation)) {
        CTCEntityEvents* targetEvents = 0;
        target.GetInterface(&targetEvents, 0x42);
        const NEntityEvents::CEventHitThing* hit;
        if (targetEvents->GetEvent(&hit, CEventFilter_None()) && hit->Decapitate)
            return PDialogueDef->ReactionToDecapitation;
    }
    if (IsKnockedDown(target))
        return PDialogueDef->EnemyKnockedDown;
    {
        const NEntityEvents::CEventExpressionPerformedTo* expression;
        if (events->GetEvent(&expression, CEventFilter_None()))
            return PDialogueDef->Threat;
    }
    if (IsSpeechGroupAllowed(PDialogueDef->WillSeenUsed)) {
        CMessagePtrList messages;
        CMessageEventFilter_IsOfType filter(0x44);
        CMessageEventManager* manager = g_pMainGameComponent->PWorld->PMessageEventManager;
        if (manager->FindAllMessages(filter, messages) > 0)
            return PDialogueDef->WillSeenUsed;
    }
    if (IsSpeechGroupAllowed(PDialogueDef->ColleagueInjured)) {
        const CAttackerMap& attackers = combatManager.GetCombatantInfo(target).Attackers;
        for (CAttackerIterator it = attackers.begin(); it != attackers.end(); ++it) {
            if (it->second != &creature && IsKnockedDown(*it->second))
                return PDialogueDef->ColleagueInjured;
        }
    }
    if (creature.GetHealthFraction() < 0.15f)
        return PDialogueDef->Injured;
    if (target.m_type == 2 && GFRandom(100, g_pMainGameComponent->PWorld->GetPRandomSeed()) > 50
        && g_pMainGameComponent->PWorld->POpinionReactionManager->GetThingAttitudeToPlayer(creature) == OPINION_ATTITUDE_4)
        return PDialogueDef->Scared;
    return PDialogueDef->Threat;
}
