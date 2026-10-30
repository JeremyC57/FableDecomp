// CTCGhostSword::AddSword(const C3DVector& position) -- summon one ghostly sword for the hero: play the appear sound, create a GHOSTLY_SWORD creature at the position in the hero's faction and ally list (able to move through solid objects, on the summoned list), attach its particle tracker / appear and alive effects, give it an on-die effect that expires after the ability duration, then register it and make the summon noise
// Grade: DIFFER(1531v1519): 157 diff lines (was 190) -- the CDefOut/effect section is its own {} block, so swordOut and effect share the earlier string-temporary slots at +0x18/+0x10 as retail; residue is frame 0x44 v 0x3c (8 bytes below the parent spill in the first HasTC block) and the scratch-register rotation that follows
// Leaf: ?AddSword@CTCGhostSword
//
// Retail 0x007C57C0, 1520 bytes (8-aligned ebp frame, `ret 4`).
// Callees: CVectorMap::LowerBound 0x0040F020, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0 / operator= 0x0099EFB0,
//   CTCSoundInfo::PlaySound 0x0066BD00, CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName
//   0x009AD410, CThingAICreature::Create 0x00833800 (__fastcall), CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 /
//   GetPItem 0x00A01B50 / ~ 0x00A01C10, CTCEnemy::SetFaction 0x0076C810 / AddAlly 0x0076D270,
//   CTCCreatureNavigation::SetAsAbleToMoveThroughSolidObjects 0x006CB9D0, the summoned-creature list's Add 0x006AEDC0,
//   CThing::AddTC 0x004C9D60, CDefClassBase::GetSubDef<CGhostSwordDef> 0x00669580, the def's effect lookup 0x00669D30,
//   CreateParticleEffect 0x0072AB70 (__fastcall), CTCDParticleEmitter::AttachToThing 0x006E0BE0, vector<CIntelligentPointer>
//   push_back 0x004AD520 / 0x007B0000, __ftol2 0x00BFEA70, CThingGameObject::SetAddToComboMultiplierOnHit 0x008389A0,
//   vector<long> InsertGrow 0x00440990, CThingCreatureBase::MakeNoise 0x00662110.
// Layout: CThingAICreatureInit is the Ego_r.pdb record (landed CGameScriptInterface::CreateCreature 0x008A9100). Retail
//   CTCGhostSword: ParentThing +0x04, def +0x0C (durations per level +0x3C, on-die delay +0x50), Swords +0x10, Effects +0x1C,
//   the sword id +0x2C pushed onto +0x30, level +0x3C; CThing player byte +0x90, script name +0x74, flag bytes +0x93/+0x15C;
//   CTCEnemy faction (CCharString*) +0x18; CTCEffectOnDie effect +0x10, delay +0x14, end delay +0x18, sound +0x1C.
#include <new>

struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CTCBase {
    virtual void v00();
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x74 - 0x4c];
    CCharString    ScriptName;                          // +0x74
    unsigned char  _pad78[0x90 - 0x78];
    signed char    Player;                              // +0x90
    unsigned char  Flags91;                             // +0x91
    unsigned char  _pad92;
    unsigned char  Flag93_0 : 5;                        // +0x93
    unsigned char  Flag93_5 : 1;
    unsigned char  _pad94[0x15c - 0x94];
    unsigned char  Flag15C_0 : 4;                       // +0x15C
    unsigned char  Flag15C_4 : 1;
    unsigned char  Flag15C_5 : 1;
    unsigned char  Flag15C_6 : 1;
    CTCBase* AddTC(const CCharString& name, long a1, long a2);   // 0x004C9D60
    void SetScriptName(const CCharString& name) { ScriptName = name; }
    void SetAddToComboMultiplierOnHit(bool on);         // 0x008389A0
    void MakeNoise(const CCharString& name);            // 0x00662110
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
struct CThingPhysical;
struct CThingBuilding;
struct CThingAICreatureInit {                           // Ego_r.pdb, 28 bytes
    CThingPhysical* OriginalThing;                      // +0x00
    CThingPhysical* PreviousThing;                      // +0x04
    CCharString ScriptName;                             // +0x08
    CCharString OverridingBrainName;                    // +0x0C
    bool PlayerCharacter;                               // +0x10
    bool Drawable;                                      // +0x11
    bool PlaceAtEndOfList;                              // +0x12
    bool EnableCreatureAutoPlacing;                     // +0x13
    CThingBuilding* HomeBuilding;                       // +0x14
    CThingBuilding* WorkBuilding;                       // +0x18
    CThingAICreatureInit(const CCharString& scriptName = "", const CCharString& brainName = "")
        : OriginalThing(0), PreviousThing(0), ScriptName(scriptName), OverridingBrainName(brainName),
          PlayerCharacter(false), Drawable(true), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) {}
};
struct CThingAICreature : CThing {
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                               CThingAICreatureInit* init);   // 0x00833800
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void* GetPItem() const;                             // 0x00A01B50
    void SetPItem(void* item);                          // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { SetPItem(item); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template<class T> struct CIPVector {
    T* first; T* last; T* end;
    void push_back(const T& value);
};
struct SScalarTag { SScalarTag() { } };
struct CLongVector {
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00440990
    void push_back(const long& value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        } else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct CTCSoundInfo {
    void PlaySound(const CCharString& name, bool a1, long a2, long a3, long a4);   // 0x0066BD00
};
struct CTCEnemy {
    unsigned char _pad00[0x18];
    CCharString*  Faction;                              // +0x18
    CCharString GetFaction() const { return *Faction; }
    void SetFaction(const CCharString& faction);        // 0x0076C810
    void AddAlly(CThing* ally);                         // 0x0076D270
};
struct CTCCreatureNavigation { void SetAsAbleToMoveThroughSolidObjects(bool on); };   // 0x006CB9D0
struct CTCSummoner { void AddSummoned(CThing* thing); };   // 0x006AEDC0
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long boneType, const CCharString& bone, long a3, long a4);   // 0x006E0BE0
};
struct CTCEffectOnDie : CTCBase {
    unsigned char _pad04[0x10 - 0x04];
    long          Effect;                               // +0x10
    long          Delay;                                // +0x14
    long          EndDelay;                             // +0x18
    CCharString   Sound;                                // +0x1C
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& position);   // 0x0072AB70
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template<class T> struct CDefOut {
    T* Object;
    CDefOut() : Object(0) {}
};
struct CGhostSwordDef : CDefClassBase {
    bool LookupEffect(const CCharString& name, long* effect) const;   // 0x00669D30
};
struct CGhostSwordAbilityDef {
    unsigned char _pad00[0x3c];
    float Durations[5];                                 // +0x3C (per level)
    float OnDieEndDelay;                                // +0x50
    template<class T> bool GetSubDef(CDefOut<T>* out) const;   // 0x00669580
};
struct CDefinitionManager { long GetDefGlobalIndexFromName(const CCharString& name); };   // 0x009AD410
struct CGameDefinitionManager { static CDefinitionManager* Get(); };   // 0x0044C6B0
extern long g_FramesPerSecond;                          // 0x01375550

class CTCGhostSword {
public:
    virtual void v00();
    CThing*                             ParentThing;    // +0x04
    unsigned char                       _pad08[0x4];
    CGhostSwordAbilityDef*              Def;            // +0x0C
    CIPVector<CIntelligentPointer<CThingAICreature> > Swords;   // +0x10
    CIPVector<CIntelligentPointer<CThing> > Effects;    // +0x1C
    unsigned char                       _pad28[0x4];
    long                                SwordId;        // +0x2C
    CLongVector                         SwordIds;       // +0x30
    long                                Level;          // +0x3C
    void AddSword(const C3DVector& position);
};

void CTCGhostSword::AddSword(const C3DVector& position)
{
    CThing* parent = ParentThing;
    if (parent->HasTC(0x10)) {
        CTCSoundInfo* sound = parent->GetTC<CTCSoundInfo>(0x10);
        sound->PlaySound("HERO_ABILITY_GHOSTLY_SWORD_APPEAR", true, 0, 0, -1);
    }
    long defIndex = CGameDefinitionManager::Get()->GetDefGlobalIndexFromName("GHOSTLY_SWORD");
    CThingAICreatureInit init;
    CIntelligentPointer<CThingAICreature> sword(CThingAICreature::Create(defIndex, position, parent->Player, &init));
    sword.GetPItem()->SetScriptName("HeroSummonnedCreature");
    CTCEnemy* parentEnemy = 0;
    CTCEnemy* swordEnemy = 0;
    CThing* swordThing = sword.GetPItem();
    if (swordThing->HasTC(0x49))
        swordEnemy = swordThing->GetTC<CTCEnemy>(0x49);
    if (parent->HasTC(0x49))
        parentEnemy = parent->GetTC<CTCEnemy>(0x49);
    swordEnemy->SetFaction(parentEnemy->GetFaction());
    swordEnemy->AddAlly(parent);
    CThing* navThing = sword.GetPItem();
    if (navThing->HasTC(7))
        navThing->GetTC<CTCCreatureNavigation>(7)->SetAsAbleToMoveThroughSolidObjects(true);
    if (parent->HasTC(0x7a))
        parent->GetTC<CTCSummoner>(0x7a)->AddSummoned(sword.GetPItem());
    sword.GetPItem()->Flag15C_6 = true;
    if (!sword.GetPItem()->HasTC(0xd8))
        sword.GetPItem()->AddTC("CTCParticleAttachmentTracker", 0, 0);
    {
    CDefOut<CGhostSwordDef> swordOut;
    long effect = 0;
    bool found = Def->GetSubDef(&swordOut);
    CGhostSwordDef* swordDef = swordOut.Object;
    if (found) {
        if (swordDef->LookupEffect("GHOST_SWORD_APPEARING", &effect))
            CreateParticleEffect(effect, position);
        if (swordDef->LookupEffect("GHOST_SWORD_ALIVE_EFFECT", &effect)) {
            CThing* aliveEffect = CreateParticleEffect(effect, position);
            if (aliveEffect && !(aliveEffect->Flags91 & 1)) {
                CTCDParticleEmitter* emitter;
                if (aliveEffect->HasTC(0x4b))
                    emitter = aliveEffect->GetTC<CTCDParticleEmitter>(0x4b);
                if (emitter)
                    emitter->AttachToThing(sword.GetPItem(), 10, "EMITTER_BASE", 0, 0);
            }
            Effects.push_back(CIntelligentPointer<CThing>(aliveEffect));
        }
        CTCEffectOnDie* onDie = (CTCEffectOnDie*)sword.GetPItem()->AddTC("CTCEffectOnDie", 1, 0);
        if (swordDef->LookupEffect("GHOST_SWORD_ON_DIE_EFFECT", &effect))
            onDie->Effect = effect;
        onDie->Delay = (long)(g_FramesPerSecond * Def->Durations[Level]);
        onDie->Sound = "HERO_ABILITY_GHOSTLY_SWORD_DISAPPEAR";
        onDie->EndDelay = (long)(g_FramesPerSecond * Def->OnDieEndDelay);
    }
    sword.GetPItem()->Flag15C_4 = false;
    sword.GetPItem()->Flag15C_5 = false;
    sword.GetPItem()->Flag93_5 = false;
    sword.GetPItem()->SetAddToComboMultiplierOnHit(false);
    Swords.push_back(sword);
    SwordIds.push_back(SwordId);
    parent->MakeNoise("SIMPLE_NOISE");
    if (swordDef)
        swordDef->Release();
    }
}
