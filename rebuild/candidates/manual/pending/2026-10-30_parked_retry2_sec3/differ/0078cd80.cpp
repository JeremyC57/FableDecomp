// CTCBerserk::CleanupAbility(bool) -- tear the berserk ability down: drop the crazy-move flourish from the hero's combat TC, kill the body effect and every dependent thing, reset TC 3, detach from the owner's +0x64 controller, remove this ability's haste modifiers, fade the radial blur out (at once in a cutscene), spawn the exit effect, stop the looping sound and (outside a cutscene) play the level's end sound, reset TC 0x31, re-evaluate the owner, queue the "STRENGTH_BERSERK_EXIT" action and clear the owner as an opinion source
// Grade: DIFFER(1136v1164) -- 365 difflines (parked 1120/333): the four end-sound arms now pass the literal to the const CCharString& parameter (implicit conversion), which builds each temporary BEFORE the other pushes in its own slot with the shared dtor tail exactly as retail. Residue: HasTC probes (retail 'shr ; and al,1' bool stored to a stack byte, ours test byte), owner ebx/ebp swap, 0xd4 v 0xac frame. Tried: named per-arm locals (1076/341, shares one slot), default arguments (no change). Next: HasTC as a bool return of the shifted word (GOTCHAS wave 24k)
// Leaf: CleanupAbility
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078CD80, 1168 bytes, `ret 4`.
// Literals: "HERO_MELEE_COMBAT_ABILITY_BERSERK_CRAZY_MOVE" 0x0126BC34, "BERSERK_EXIT_EFFECT" 0x0126BC74, "" 0x0122D70E,
//   "HERO_ABILITY_BEZERK_END_NO_POWERUP" 0x0126BDF4, "..._LEVEL_1" 0x0126BDD4, "..._LEVEL_2" 0x0126BDB4, "..._LEVEL_3" 0x0126BD94,
//   "STRENGTH_BERSERK_EXIT" 0x0126BD7C.
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CAbilityController;
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CAbilityController* Controller;                     // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
    void Reevaluate();                                  // 0x00662650
    void AddAction(void* action);                       // 0x006644F0
    __forceinline bool HasTC(long type) const { return ((m_presentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    bool IsDead() const { return (m_flags & 1) != 0; }
};
struct CAbilityController {
    void RemoveAbility(CTCBase* ability);               // 0x004BFAE0
    void SetScale(float scale);                         // 0x004BFA50
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& position);   // 0x0072AB70
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CDefPointeeBase {
    virtual void Destroy();
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CSpecialEffectsDef : CDefPointeeBase {
    bool GetEffectFromString(const CCharString& name, long* effect);   // 0x00669D30
};
struct CBerserkDef : CDefPointeeBase {
    unsigned char _pad08[0x68 - 0x08];
    long RadialBlurFadeTime;                            // +0x68
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
struct CDefinitionManager { long GetDefGlobalIndex(const CCharString& name); };   // 0x009AD410
struct CMainGameComponent { CDefinitionManager* PeekDefinitionManager(); };       // 0x00415FBC
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
struct CTCCombat { void RemoveMeleeFlourishAbility(long move); };   // 0x007366C0
struct CTCMode3 { void Reset(bool b); };                // 0x0071BEE0
struct CTCMode31 { void ClearMode(long mode); };        // 0x0073ABA0
struct CTCSoundPlayer {
    unsigned char _pad00[0x18];
    CCharString CurrentCriteria;                        // +0x18
    void StopSample(unsigned long sound);               // 0x0066ADD0
    void PlayAttachedCriteria(const CCharString& name, bool loop, long a, long b, long c);   // 0x0066BD00
};
struct CModifierIterator { void* node; };
struct CModifierRange { CModifierIterator first, second; };
struct CModifierMap {
    CModifierRange equal_range(CTCBase* const& key);    // 0x00580B2F
    void erase(CModifierIterator first, CModifierIterator last);   // 0x00580658
};
struct CTCHaste {
    unsigned char _pad00[0x18];
    CModifierMap Modifiers;                             // +0x18
    void Recalculate();                                 // 0x00800020
};
struct CRadialBlur { void FadeOut(long time, void* key); };   // 0x006B82E0
struct CEngineHolder { unsigned char _pad[0x1964]; CRadialBlur* RadialBlur; };
struct COpinionReactionManager { void ClearOpinionSource(CThing* thing, bool all); };   // 0x007008C0
struct CWorld {
    unsigned char _pad00[0x18];
    CEngineHolder* Engine;                              // +0x18
    unsigned char _pad1c[0x74 - 0x1c];
    COpinionReactionManager* OpinionReactions;          // +0x74
};
struct SActionTarget { long A, B; SActionTarget() : A(0), B(0) { } };
struct CCreatureActionExit {
    unsigned char _data[0x90];
    CCreatureActionExit(CThing* owner, const CCharString& anim, const SActionTarget& target, long a, bool b, bool c, long d, long e);   // 0x008424F0
    ~CCreatureActionExit();                             // 0x00693EF0
};
struct CIntelligentThing : CBaseIntelligentPointer { };

class CTCBerserk {
public:
    void*                  __vftable;                   // +0x00
    CThing*                ParentThing;                 // +0x04
    unsigned char          _pad08[0x40 - 0x08];
    unsigned long          BerserkingLoopingSound;      // +0x40
    long                   SpellLevel;                  // +0x44
    bool                   JustReleased;                // +0x48
    bool                   AbilityInitialised;          // +0x49
    unsigned char          _pad4a[0x4c - 0x4a];
    CIntelligentThing*     DependentFirst;              // +0x4c
    CIntelligentThing*     DependentLast;               // +0x50
    CIntelligentThing*     DependentEnd;                // +0x54
    CBaseIntelligentPointer StartUpEffect;              // +0x58
    CBaseIntelligentPointer BerserkBodyEffect;          // +0x60
    CBerserkDef*           PDef;                        // +0x68
    void*                  RadialBlurFadeKey;           // +0x6c
    CWorld* GetWorld();                                 // 0x00686A70
    void CleanupAbility(bool inCutscene);
};

void CTCBerserk::CleanupAbility(bool inCutscene)
{
    CThing* thing = ParentThing;
    bool has = thing->HasTC(0x30);
    if (has) {
        CTCCombat* combat = thing->GetTC<CTCCombat>(0x30);
        long move = g_pMainGameComponent->PeekDefinitionManager()->GetDefGlobalIndex(CCharString("HERO_MELEE_COMBAT_ABILITY_BERSERK_CRAZY_MOVE"));
        combat->RemoveMeleeFlourishAbility(move);
    }
    if (IS_THING_ALIVE(BerserkBodyEffect))
        BerserkBodyEffect.GetPItem()->Kill(true);
    has = thing->HasTC(3);
    if (has)
        thing->GetTC<CTCMode3>(3)->Reset(false);
    CAbilityController* controller = thing->Controller;
    if (controller) {
        controller->RemoveAbility((CTCBase*)this);
        controller->SetScale(1.0f);
    }
    for (CIntelligentThing* it = DependentFirst; it != DependentLast; ++it) {
        CThing* dependent = it->GetPItem();
        if (dependent && !dependent->IsDead())
            dependent->Kill(true);
    }
    has = thing->HasTC(0x8c);
    if (has) {
        CTCHaste* haste = thing->GetTC<CTCHaste>(0x8c);
        CTCBase* key = (CTCBase*)this;
        CModifierRange range = haste->Modifiers.equal_range(key);
        haste->Modifiers.erase(range.first, range.second);
        haste->Recalculate();
    }
    GetWorld()->Engine->RadialBlur->FadeOut(inCutscene ? 0 : PDef->RadialBlurFadeTime, RadialBlurFadeKey);
    if (!inCutscene) {
        CBerserkDef* berserkDef = PDef;
        CDefPointer<CSpecialEffectsDef> def;
        long effect = 0;
        if (berserkDef->GetSubDef(&def) && def->GetEffectFromString("BERSERK_EXIT_EFFECT", &effect))
            CreateParticleEffect(effect, thing->GetPosition());
    }
    CThing* owner = ParentThing;
    if (owner->m_presentBits[0] & 0x10000) {
        CTCSoundPlayer* sound = owner->GetTC<CTCSoundPlayer>(0x10);
        sound->StopSample(BerserkingLoopingSound);
        sound->CurrentCriteria = CCharString("");
        if (!inCutscene) {
            switch (SpellLevel) {
            case 0: sound->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_NO_POWERUP", true, 0, 0, -1); break;
            case 1: sound->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_LEVEL_1", true, 0, 0, -1); break;
            case 2: sound->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_LEVEL_2", true, 0, 0, -1); break;
            case 3: sound->PlayAttachedCriteria("HERO_ABILITY_BEZERK_END_LEVEL_3", true, 0, 0, -1); break;
            }
        }
    }
    CTCMode31* mode;
    if (thing->m_presentBits[1] & 0x20000)
        mode = thing->GetTC<CTCMode31>(0x31);
    mode->ClearMode(0x24);
    thing->Reevaluate();
    if (!inCutscene) {
        CCharString anim("STRENGTH_BERSERK_EXIT");
        SActionTarget target;
        thing->AddAction(&CCreatureActionExit(thing, anim, target, 0, true, true, 0, 0));
    }
    GetWorld()->OpinionReactions->ClearOpinionSource(thing, true);
}
