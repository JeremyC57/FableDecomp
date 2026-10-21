// Grade: DIFFER(435v439) -- tail only: retail re-reads the type argument from its slot for the GetFrequency push (edx twice, vtable in eax); the lane loads it once into eax and reuses it
// COpinionReactionManager::GetReactionMatchStrength(EOpinionReactionType type, const CThing* thing) -- strongest match of the given reaction type among the def's reaction matches for the thing seen by the hero (0 when the thing has no opinion-reactions component or there is no live hero), scaled by the type's frequency traits
// Leaf: GetReactionMatchStrength
// Retail: Fable.exe (TLC, VC7.1) @ 0x00701590, 0x1B5 bytes (padded to 448), `ret 8`, float in st0.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (inlined lookups 0x4F, 0x50, 0xD7),
//   the player accessor 0x0049D9D0 on [g_pMainGameComponent 0x013B86A0]+0x24, CPlayer::GetHeroCreature 0x00487DC0, the
//   match-context constructor 0x004538EB (out of line: vector<float>(5, 0.0f)), the manager's context fill 0x00700AE0,
//   the 0xD7 component's bool query 0x006CF810, CReactionMatch virtual slot 1 (strength), CReactionFrequencyTraits virtual
//   slot 0, free 0x00BFEA14.
// Layout: Ego_r.pdb COpinionReactionManager PDef +0; COpinionReactionManagerDef (retail 0x28 below the PDB from the
//   12-byte vectors/maps): Matches list +0xa8, FrequencyTraits +0x324; CReactionMatch type +4; thing interface bits +0x20,
//   map +0x44, flags word +0x38 (bit 23), dead bit +0x91.
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                         // +0x20
    CInterfaceMap  m_interfaces;                             // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  Dead : 1;                                 // +0x91 bit 0
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCIfPresent(long interfaceType) const
    {
        T* component = 0;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
};
struct CPlayer {
    CThing* GetHeroCreature();                               // 0x00487DC0
};
struct CPlayerSource {
    CPlayer* GetCurrentPlayer();                             // 0x0049D9D0
};
struct CMainGameComponent {
    unsigned char  _pad00[0x24];
    CPlayerSource* Players;                                  // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;             // 0x013B86A0
struct CTCOpinionReactions;                                  // interface 0x4F
struct CTCHeroOpinion;                                       // interface 0x50
struct CTCQueryD7 {                                          // interface 0xD7
    bool Query() const;                                      // 0x006CF810
};
struct CReactionContext {                                    // vector<float>(5, 0.0f)
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    CReactionContext();                                      // 0x004538EB
    ~CReactionContext() { if (_M_start) free(_M_start); }
};
struct CReactionMatch {
    virtual void v00();
    virtual float GetStrength(const CReactionContext& context) const;   // slot 1
    long Type;                                               // +0x04
};
struct CReactionFrequencyTraits {
    virtual float GetFrequency(long type) const;             // slot 0
};
struct SMatchNode {
    SMatchNode*     _M_next;
    SMatchNode*     _M_prev;
    CReactionMatch* _M_data;
};
struct COpinionReactionManagerDef {
    unsigned char              _pad00[0xa8];
    SMatchNode*                Matches;                      // +0xa8
    unsigned char              _padac[0x324 - 0xac];
    CReactionFrequencyTraits** FrequencyTraits;              // +0x324
};
struct CThingFlags38 {
    unsigned char _pad00[0x38];
    unsigned long Flags;                                     // +0x38
};
class COpinionReactionManager {
public:
    COpinionReactionManagerDef* PDef;                        // +0x00
    void FillContext(CTCOpinionReactions* reactions, CReactionContext& context, CTCHeroOpinion* hero);   // 0x00700AE0
    float GetReactionMatchStrength(long type, const CThing* thing);
};
float COpinionReactionManager::GetReactionMatchStrength(long type, const CThing* thing)
{
    if (thing->HasTC(0x4f)) {
        CTCOpinionReactions* reactions = thing->GetTC<CTCOpinionReactions>(0x4f);
        CThing* hero = g_pMainGameComponent->Players->GetCurrentPlayer()->GetHeroCreature();
        if (hero && !hero->Dead) {
            CTCHeroOpinion* heroOpinion = hero->GetTCIfPresent<CTCHeroOpinion>(0x50);
            CReactionContext context;
            FillContext(reactions, context, heroOpinion);
            float best = 0.0f;
            SMatchNode* head = PDef->Matches;
            for (SMatchNode* it = head->_M_next; it != head; it = it->_M_next) {
                CReactionMatch* match = it->_M_data;
                if (match->Type != type)
                    continue;
                float strength = 0.0f;
                if ((match->Type != 0xb && match->Type != 0xc)
                    || ((((const CThingFlags38*)thing)->Flags & 0x800000) && thing->GetTC<CTCQueryD7>(0xd7)->Query()))
                    strength = match->GetStrength(context);
                if (best < strength)
                    best = strength;
            }
            CReactionFrequencyTraits* traits = PDef->FrequencyTraits[type];
            if (traits)
                best *= traits->GetFrequency(type);
            return best;
        }
    }
    return 0.0f;
}
