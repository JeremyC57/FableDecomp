// CGameScriptInterface::AddCrimeCommitted(const CScriptThing& victim, ECrime crime, bool informWitnesses, const CScriptThing* criminal, const CScriptThing* witness, EOpinionPostDeedType postType) const -- when the hero lives and the victim is valid: post the crime to the hero's opinion deed log (kind 0x51) at the hero's position (the witness defaults to the first member of the victim's group component 0x22), then tell every live member of that group with components 0xB4 and 0x4F about the deed unless it already knows
// Grade: DIFFER(567v559) -- calls, constants and control flow match; register allocation differs: VC7.1 keeps `victim` in ebx and spills criminalThing to the dead criminal slot, retail keeps criminalThing in ebx, re-reads victim from its argument slot and keeps the deed index in the dead criminal slot (re-read each loop pass)
// Leaf: AddCrimeCommitted
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089FEB0, length 560 bytes (`ret 18h`, no frame).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetHeroCreature 0x00487DC0, CScriptThing slots 11/75,
//   CVectorMap LowerBound 0x0040F020, the group member-list count 0x0065F17B, GetPItem 0x00A01B50, CThing::GetPosition
//   0x004C73D0, CTCHeroOpinionDeedLog::PostDeed 0x006C6990, CTCCreatureOpinionOfHero::KnowsOfDeed 0x00681AE0 / InformOfDeed 0x00683590.
// Layout: CThing presence bits +0x20 and the component map +0x44 as the landed 0x007BB150; the group component's member list (bare head-node pointer) at +0x2C.
typedef long ECrime;
typedef long EOpinionPostDeedType;
class CTCBase;
struct C3DVector;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char m_flags6c;                    // +0x6c
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_dead : 1;                   // +0x91 bit 0
    unsigned char m_91 : 7;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {
    void* _vfptr;
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct SMemberNode {
    SMemberNode* _M_next;
    SMemberNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};
struct CMemberList {
    SMemberNode* _M_node;
    long Count() const;                         // 0x0065F17B
};
struct CTCGroup {
    unsigned char _pad00[0x2c];
    CMemberList Members;                        // +0x2c
    void InformMembers(long deed, long crime);
};
struct CThingAICreature;
struct CFaction;
struct CTCHeroOpinionDeedLog {
    long PostDeed(long deedType, const C3DVector& position, CThing* target, CThing* victim, long postType,
                  bool flagged, bool b2, CFaction* faction, bool b3, long extra);   // 0x006C6990
};
struct CTCCreatureOpinionOfHero {
    bool KnowsOfDeed(long deedHandle, long type, bool includeReported);   // 0x00681AE0
    void InformOfDeed(long deedHandle, bool b);                           // 0x00683590
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;           // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;               // slot 75 (+0x12c)
};
struct CPlayer {
    CThing* GetHeroCreature();                  // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CWorld; struct CMainGameComponent; struct CDisplayEngine; struct CDefinitionManager;
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                 World;              // +0x04
    CMainGameComponent*     Component;          // +0x08
    CDisplayEngine*         DisplayEngine;      // +0x0c
    CDefinitionManager*     DefinitionManager;  // +0x10
    CPlayerManager*         PlayerManager;      // +0x14
    virtual void AddCrimeCommitted(const CScriptThing& victim, ECrime crime, bool informWitnesses,
                                   const CScriptThing* criminal, const CScriptThing* witness, EOpinionPostDeedType postType) const;
};

__forceinline void CTCGroup::InformMembers(long deed, long crime)
{
    SMemberNode* head = Members._M_node;
    for (SMemberNode* node = head->_M_next; node != head; node = node->_M_next) {
        CThing* member = node->_M_data.GetPItem();
        if (member && !member->m_dead && member->HasTC(0xB4) && member->HasTC(0x4F)) {
            CTCCreatureOpinionOfHero* opinion = member->GetTC<CTCCreatureOpinionOfHero>(0x4F);
            if (!opinion->KnowsOfDeed(deed, crime, true))
                opinion->InformOfDeed(deed, false);
        }
    }
}
void CGameScriptInterface::AddCrimeCommitted(const CScriptThing& victim, ECrime crime, bool informWitnesses,
                                             const CScriptThing* criminal, const CScriptThing* witness, EOpinionPostDeedType postType) const
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetHeroCreature();
    if (!hero || hero->m_dead)
        return;
    if (!victim.IsValid())
        return;
    CThing* criminalThing = criminal ? criminal->GetThing() : 0;
    CThing* witnessThing = 0;
    if (witness && witness->IsValid() && (witness->GetThing()->m_flags6c & 0x10))
        witnessThing = witness->GetThing();
    if (informWitnesses && !witnessThing) {
        CThing* v = victim.GetThing();
        if (v->HasTC(0x22)) {
            CTCGroup* group = v->GetTC<CTCGroup>(0x22);
            if (group->Members.Count())
                witnessThing = group->Members._M_node->_M_next->_M_data.GetPItem();
        }
    }
    long deed = -1;
    if (hero->HasTC(0x51)) {
        CTCHeroOpinionDeedLog* log = hero->GetTC<CTCHeroOpinionDeedLog>(0x51);
        deed = log->PostDeed(crime, hero->GetPosition(), criminalThing, witnessThing, postType, true, false, 0, false, 0);
    }
    if (informWitnesses && deed != -1 && victim.IsValid() && victim.GetThing()->HasTC(0x22)) {
        victim.GetThing()->GetTC<CTCGroup>(0x22)->InformMembers(deed, crime);
    }
}
