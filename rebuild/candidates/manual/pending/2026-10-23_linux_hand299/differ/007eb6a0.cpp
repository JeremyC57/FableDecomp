// Grade: DIFFER(735v748) -- flow, TC lookups, deed/message/tutorial calls in place; residue is the register map (retail this in ebp with the hero spilled to the dead height slot and edi reused for the owner pointer; ours this in edi, hero in ebp) plus the knock-on frame/push order; tried named centre local (fixed frame 0x58), if/else animation choice (fixed the push arms), GetHero wrapper
// CTCActionUseSearch::Use(CThing* user) -- searching a container: base use, pick the high/low search animation from the container's physics-mesh centre height, give the hero the search tutorial (stopping there when it is shown), search the container, alert the owner, post a theft deed (with a morality hit when the container held a reward) and notify the user's buying component
// Leaf: vfunc_28
class CCharString {
public:
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString& operator=(const char* text);          // 0x0099EFE0
    ~CCharString();                                    // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CPhysicsMeshObject {
    unsigned char _body[0x34];
    ~CPhysicsMeshObject();                              // 0x00A127D0
    C3DVector GetWorldCentre() const;                   // 0x00A11BA0
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92();
    virtual const void* GetPhysicsMeshInfo() const;     // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject() const;    // 0x006B0850
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Dead : 1;                     // +0x91 bit 0
    unsigned char _pad92[0x100 - 0x92];
    long ShopValue;                             // +0x100
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void* m_vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTCOwnedEntity {                         // interface 0x33
    unsigned char _pad00[0x0d];
    CBaseIntelligentPointer Owner;              // +0x0d
};
#pragma pack(pop)
struct CPlayer {
    unsigned char _pad00[0x34];
    CBaseIntelligentPointer PHero;              // +0x34
    CThing* GetHero() const { return PHero.GetPItem(); }
};
struct CPlayerManager { CPlayer* GetMainPlayer(); };   // 0x00449970
struct CTCHero { bool GiveTutorialForCategory(long category, bool force); };   // 0x006C9FA0
struct CTCSearchableContainer {                 // interface 0x3c
    bool HasReward() const;                     // 0x0074E500
    void Search(CThing* user);                  // 0x00812340
};
struct CFaction;
struct CThingAICreature;
struct CTCHeroOpinionDeedLog {                  // interface 0x51
    long PostDeed(long deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  long postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};
struct CTCHeroStats {                           // interface 4
    unsigned char _pad00[0x28];
    long Morality;                              // +0x28
    void SetMorality(long morality);            // 0x0057C2E6
};
struct CTCShopper { void BoughtSomething(long value); };   // interface 0x36, 0x00747030
struct CMessageEventManager {
    void AddMessage(long type, long priority, const C3DVector& position, CThing* target, long a, long b, long c);   // 0x00687540
};
struct CWorld {
    unsigned char _pad00[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
};
struct CGlobalDefs { unsigned char _pad[0xe0]; long SearchTheftMoralityChange; };   // +0xe0
struct CDefinitionHolder { unsigned char _pad[0xdc]; CGlobalDefs* Globals; };       // +0xdc
struct CGameState { unsigned char _pad[0xf6]; bool TutorialsDisabled; };            // +0xf6
extern CGameState* g_pGameState;                // 0x013B8790
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    CWorld* GetWorld();                         // 0x00686A70
    CPlayerManager* GetPlayerManager();         // 0x00686D40
    const CDefinitionHolder* GetDefinitions() const;   // 0x006869D0
};
class CTCActionUseBase : public CTCBase {
public:
    void Use(CThing* user);                     // 0x007EAB90
    bool IsThisTheft() const;                   // 0x007EACB0
};
class CTCActionUseSearch : public CTCActionUseBase {
public:
    void Use(CThing* user);
};

void CTCActionUseSearch::Use(CThing* user)
{
    CTCActionUseBase::Use(user);
    float height;
    if (ParentThing->PhysicsTC->GetPhysicsMeshInfo()) {
        C3DVector centre = ParentThing->PhysicsTC->GetPhysicsMeshObject().GetWorldCentre();
        height = centre.Z;
    } else
        height = 2.0f;
    CCharString animation;
    if (height > 1.15f)
        animation = "SEARCH_HIGH";
    else
        animation = "SEARCH_LOW";
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHero();
    if (hero && !hero->Dead && !g_pGameState->TutorialsDisabled && hero->HasTC(0x29)) {
        if (hero->GetTC<CTCHero>(0x29)->GiveTutorialForCategory(0x21, false))
            return;
    }
    bool hadReward = false;
    if (ParentThing->HasTC(0x3c)) {
        CTCSearchableContainer* container = ParentThing->GetTC<CTCSearchableContainer>(0x3c);
        hadReward = container->HasReward();
        container->Search(user);
    }
    if (ParentThing->HasTC(0x33)) {
        CBaseIntelligentPointer& owner = ParentThing->GetTC<CTCOwnedEntity>(0x33)->Owner;
        if (owner.GetPItem())
            GetWorld()->PMessageEventManager->AddMessage(0x1c, 0x32, user->GetPosition(), owner.GetPItem(), 0, 0, 0);
    }
    if (IsThisTheft() && user->HasTC(0x51)) {
        user->GetTC<CTCHeroOpinionDeedLog>(0x51)->PostDeed(6, user->GetPosition(), ParentThing, 0, 2, true, false, 0, false, 0);
        if (hadReward && hero && !hero->Dead && hero->HasTC(4)) {
            CTCHeroStats* stats = hero->GetTC<CTCHeroStats>(4);
            stats->SetMorality(GetDefinitions()->Globals->SearchTheftMoralityChange + stats->Morality);
        }
    }
    if (user->HasTC(0x36))
        user->GetTC<CTCShopper>(0x36)->BoughtSomething(user->ShopValue);
}
