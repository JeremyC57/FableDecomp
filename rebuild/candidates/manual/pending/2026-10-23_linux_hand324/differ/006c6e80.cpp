// CTCHeroOpinionDeedLog::FrameUpdate() -- every 3 s (offset by the thing's frame base) expires finished deeds whose knowledge life (from the later of the deed frame and the village's last guard sighting) has run out and stops steal/picklock deeds whose expression ended; every second re-informs the deeds still searching for witnesses, every 300 s drops searching entries whose deed is gone; clears the child-hurt event, and tracks the hero's weapon-out crime
// Leaf: FrameUpdate
// Retail: Fable.exe @ 0x006C6E80, 1251 bytes, `ret`.
// Callees: CWorld::GetCurrentFrame 0x0049D870 (__fastcall), __aullrem 0x00BFEBB0, CBaseIntelligentPointer::GetPItem
// 0x00A01B50 / ~ 0x00A01C10, LowerBound 0x0040F020, the def-manager getter 0x006869D0, CDefinitionManager::GetDef
// 0x006C76B0, operator delete 0x00BFE9BC, _Rb_tree_rebalance_for_erase 0x0042A34E, free 0x00BFEA14, CCharString 0x0099EBF0 /
// ~ 0x0099EAE0, the expression query 0x004AF610 on 0x013B89FC, RemoveDeedStillSearchingForWitnesses 0x006C5920,
// InformRelevantThingsOfDeed 0x006C6040, CTCEntityEvents::GetEvent 0x006C7F30 / remove 0x008112D0, GetPlayerManager
// 0x00686D40, GetMainPlayer 0x00449970, GetCharacter 0x00487DC0, IsThingCarryingWeapon 0x006A1540 (__fastcall),
// PlayerTakenOutWeapon 0x006C6E20, 0x006C59E0, RemoveCrimeFromVillage 0x006C5F80; g_framesPerSecond 0x01375550.
// Layout: as the landed RemoveCrimeFromVillage 0x006C5F80 (map +0x0c, witnesses list +0x18, VillageForCrimes +0x20,
// WeaponOutDeedHandle +0x28), DeedEffectsDefIndices +0x2e (PDB +0x3a less 12).
// Grade: DIFFER(1220v1251) -- first draft: frame 0x38 v 0x30, deed-map loop rotation/iterator registers (retail ebp iterator, current in a frame word), this kept in ebp
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};
extern "C" void __cdecl free(void* block);
extern long g_framesPerSecond;                       // 0x01375550
struct CWorld {
    static long __fastcall GetCurrentFrame();        // 0x0049D870
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x18];
    unsigned __int64 FrameBase;                      // +0x18 (descriptive)
    unsigned long m_presentBits[2];                  // +0x20
    unsigned long m_flags;                           // +0x28
    unsigned char _pad2c[0x18];
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_limboFlags;                      // +0x91
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCInline(long wanted)
    {
        CInterfaceMap::SEntry* first = m_interfaces.first;
        CInterfaceMap::SEntry* last = m_interfaces.last;
        int len = (int)(last - first);
        while (len > 0) {
            int half = len / 2;
            if (first[half].type < wanted) {
                first += half + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return (T*)((first == last || wanted < first->type) ? last : first)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
class CBaseIntelligentPointer {
public:
    void* PData;
    void* PNext;
    CThing* GetPItem() const;                        // 0x00A01B50
    ~CBaseIntelligentPointer();                      // 0x00A01C10
};
struct CTCVillage {
    unsigned char _pad00[0x80];
    long FramePlayerLastSeenByGuard;                 // +0x80
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long m_count;                                    // +0x04
};
struct COpinionDeedEffectsDef : CDefBase {
    unsigned char _pad08[0x34];
    long KnowledgeLifeInSeconds;                     // +0x3c
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_count;
    }
    __forceinline ~CDefPointer()
    {
        if (Object)
            if (--Object->m_count == 0)
                Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<COpinionDeedEffectsDef>* out);   // 0x006C76B0
};
struct CThingAICreature;
struct CFaction;
struct COpinionDeed {
    long          DeedType;                          // +0x00
    long          DeedHandle;                        // +0x04
    float         Position[3];                       // +0x08
    long          FrameHappened;                     // +0x14
    CBaseIntelligentPointer PTarget;                 // +0x18
    bool          WasSearchingForWitnesses;          // +0x20
    bool          IsStillHappening;                  // +0x21
    bool          IsCrimeReportedToVillage;          // +0x22
    bool          IsCrime;                           // +0x23
    long          ExtraData;                         // +0x24
};
struct SDeedNode {
    long       _M_color;
    SDeedNode* _M_parent;
    SDeedNode* _M_left;
    SDeedNode* _M_right;
    long          first;                             // +0x10
    COpinionDeed* second;                            // +0x14
};
void __cdecl RebalanceForErase(SDeedNode* z, SDeedNode*& root, SDeedNode*& leftmost, SDeedNode*& rightmost);   // 0x0042A34E
struct COpinionDeedMap {                             // STLport map<long, COpinionDeed*> (12 bytes)
    SDeedNode*   _M_header;
    unsigned int _M_node_count;
    long         _M_key_compare;
    __forceinline SDeedNode* find(long k)
    {
        SDeedNode* y = _M_header;
        SDeedNode* x = _M_header->_M_parent;
        while (x != 0) {
            if (!(x->first < k)) {
                y = x;
                x = x->_M_left;
            }
            else
                x = x->_M_right;
        }
        if (y == _M_header || k < y->first)
            y = _M_header;
        return y;
    }
    __forceinline void erase(SDeedNode* node)
    {
        SDeedNode* y = node;
        RebalanceForErase(y, _M_header->_M_parent, _M_header->_M_left, _M_header->_M_right);
        if (y)
            free(y);
        --_M_node_count;
    }
};
__forceinline SDeedNode* Increment(SDeedNode* node)
{
    if (node->_M_right != 0) {
        node = node->_M_right;
        while (node->_M_left != 0)
            node = node->_M_left;
    }
    else {
        SDeedNode* y = node->_M_parent;
        while (node == y->_M_right) {
            node = y;
            y = y->_M_parent;
        }
        if (node->_M_right != y)
            node = y;
    }
    return node;
}
struct CWitnessesSearchingDeed {
    long DeedHandle;                                 // +0x00
    long DeedType;                                   // +0x04
    CBaseIntelligentPointer Recipient;               // +0x08
    long PostType;                                   // +0x10
    const CFaction* PFaction;                        // +0x14
    bool ReverseFactionSearch;                       // +0x18
};
struct SWitnessNode {
    SWitnessNode* _M_next;
    SWitnessNode* _M_prev;
    CWitnessesSearchingDeed _M_data;                 // +0x08
};
struct CExpressionManager {
    bool IsPlaying(const CCharString& name);         // 0x004AF610
};
extern CExpressionManager* g_pExpressionManager;     // 0x013B89FC
struct CEntityEvent;
struct CIsEventOfType {
    long Owner;
    long Type;
};
struct CTCEntityEvents {
    bool GetEvent(CEntityEvent** out, const CIsEventOfType* pred);   // 0x006C7F30
    void RemoveEvent(CEntityEvent* event);           // 0x008112D0
};
struct CPlayer {
    CThing* GetCharacter();                          // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                        // 0x00449970
};
bool __fastcall IsThingCarryingWeapon(CThing* thing);   // 0x006A1540

class CTCHeroOpinionDeedLog {
public:
    void*           __vftable;                       // +0x00
    CThing*         Thing;                           // +0x04
    long            m_8;                             // +0x08
    COpinionDeedMap OpinionDeedMap;                  // +0x0c
    SWitnessNode*   OpinionDeedsSearchingForWitnesses;   // +0x18
    long            NextHandle;                      // +0x1c
    CBaseIntelligentPointer VillageForCrimes;        // +0x20
    long            WeaponOutDeedHandle;             // +0x28
    bool            WeaponOutCrimeEnabled;           // +0x2c
    bool            GuardsIgnoreCrimes;              // +0x2d
    unsigned short  DeedEffectsDefIndices[92];       // +0x2e
    CDefinitionManager* GetDefinitionManager();      // 0x006869D0
    CPlayerManager* GetPlayerManager();              // 0x00686D40
    void RemoveDeedStillSearchingForWitnesses(long handle);   // 0x006C5920
    void ClearDeed(long handle);                     // 0x006C59E0
    void RemoveCrimeFromVillage(long handle);        // 0x006C5F80
    void PlayerTakenOutWeapon();                     // 0x006C6E20
    bool InformRelevantThingsOfDeed(long handle, CThingAICreature* creature, long postType, const CFaction* faction, bool flag);   // 0x006C6040
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long type)
    {
        CDefPointer<COpinionDeedEffectsDef> def;
        GetDefinitionManager()->GetDef(DeedEffectsDefIndices[type], &def);
        return def;
    }
    void FrameUpdate();
};

void CTCHeroOpinionDeedLog::FrameUpdate()
{
    unsigned __int64 base = Thing->FrameBase;
    long fps = g_framesPerSecond;
    unsigned __int64 frame = CWorld::GetCurrentFrame() + base;
    if (frame % (unsigned long)(fps * 3) == 0) {
        SDeedNode* end = OpinionDeedMap._M_header;
        for (SDeedNode* it = end->_M_left; it != end; ) {
            SDeedNode* current = it;
            it = Increment(it);
            COpinionDeed* deed = current->second;
            if (!deed->IsStillHappening) {
                long happened = deed->FrameHappened;
                if (deed->IsCrimeReportedToVillage && VillageForCrimes.GetPItem()) {
                    CTCVillage* village = 0;
                    if (VillageForCrimes.GetPItem()->HasTC(0x22))
                        village = VillageForCrimes.GetPItem()->GetTC<CTCVillage>(0x22);
                    if (village->FramePlayerLastSeenByGuard > happened)
                        happened = village->FramePlayerLastSeenByGuard;
                }
                CDefPointer<COpinionDeedEffectsDef> effects = GetDeedEffectsDef(deed->DeedType);
                bool expired = happened + effects->KnowledgeLifeInSeconds * fps < CWorld::GetCurrentFrame();
                if (expired) {
                    delete current->second;
                    OpinionDeedMap.erase(current);
                }
            }
            else {
                bool stopped = deed->DeedType == 6 && !g_pExpressionManager->IsPlaying(CCharString("Expression_Steal", -1))
                            || deed->DeedType == 4 && !g_pExpressionManager->IsPlaying(CCharString("Expression_Picklock", -1));
                if (stopped)
                    RemoveDeedStillSearchingForWitnesses(deed->DeedHandle);
            }
        }
    }
    if ((frame + 5) % (unsigned long)fps == 0) {
        SWitnessNode* end = OpinionDeedsSearchingForWitnesses;
        for (SWitnessNode* n = end->_M_next; n != end; n = n->_M_next)
            InformRelevantThingsOfDeed(n->_M_data.DeedHandle, (CThingAICreature*)n->_M_data.Recipient.GetPItem(), n->_M_data.PostType,
                                       n->_M_data.PFaction, n->_M_data.ReverseFactionSearch);
    }
    if ((frame + 5) % (unsigned long)(fps * 300) == 0) {
        SWitnessNode* end = OpinionDeedsSearchingForWitnesses;
        SWitnessNode* n = end->_M_next;
        while (n != end) {
            SWitnessNode* current = n;
            n = n->_M_next;
            SDeedNode* found = OpinionDeedMap.find(current->_M_data.DeedHandle);
            if (found == OpinionDeedMap._M_header || found->second == 0) {
                current->_M_prev->_M_next = current->_M_next;
                current->_M_next->_M_prev = current->_M_prev;
                current->_M_data.Recipient.~CBaseIntelligentPointer();
                free(current);
            }
        }
    }
    if (Thing->m_flags & 4) {
        CTCEntityEvents* events = Thing->GetTCInline<CTCEntityEvents>(0x42);
        CEntityEvent* event = 0;
        CIsEventOfType pred;
        pred.Owner = 0;
        pred.Type = 0x15;
        if (events->GetEvent(&event, &pred))
            events->RemoveEvent(event);
    }
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetCharacter();
    if (hero && (hero->m_limboFlags & 1) == 0) {
        bool carrying = IsThingCarryingWeapon(hero);
        if (WeaponOutDeedHandle != -1 && OpinionDeedMap.find(WeaponOutDeedHandle) == OpinionDeedMap._M_header)
            WeaponOutDeedHandle = -1;
        if (carrying) {
            if (WeaponOutDeedHandle == -1)
                PlayerTakenOutWeapon();
            return;
        }
        if (WeaponOutDeedHandle != -1) {
            RemoveDeedStillSearchingForWitnesses(WeaponOutDeedHandle);
            ClearDeed(WeaponOutDeedHandle);
            RemoveCrimeFromVillage(WeaponOutDeedHandle);
            WeaponOutDeedHandle = -1;
        }
    }
}
