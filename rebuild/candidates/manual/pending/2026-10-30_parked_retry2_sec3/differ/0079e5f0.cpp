// Grade: DIFFER(920v916) -- 68 difflines (was 189): sound name as a named block-scoped CCharString local before PlaySound and the parent local declared after the switch (ability lookup through ParentThing) give retail's colour arms (def in ebp, begin reloaded per channel) and GetString-before-compare order; left: retail spills ParentThing to the frame at the bits test (frame 4 v 8)
// CTCPhysicalShieldSpell::CTCPhysicalShieldSpell(CThing* owner) -- zero the shield state, fetch the shield def, take the owner's physical-shield ability level as the spell level, colour the shield from that level's start colour, start the level's looping sound, play the CAST effect, show the shield, make a noise and arm the fade-out timers
// Leaf: ??0CTCPhysicalShieldSpell
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079E5F0, 900 bytes + a 4-entry jump table, `ret 4`; no EH frame.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long index);                // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;              // 0x013CA828
struct CTCBaseComponent;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseComponent* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CTCInventoryAbilities {
    long GetAbilityLevel(long ability);               // 0x005EDE93
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type10;                           // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
};
struct CThingCreatureBase : CThing {
    void MakeNoise(const CCharString& noise);         // 0x00662110
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                        // 0x00A01B10
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CDefPointee {
    virtual void v000();
    virtual void Destroy();
    long RefCount;
};
struct CFloatVector {                                 // STLport vector<float>, 12 bytes
    float* _M_start; float* _M_finish; float* _M_end_of_storage;
    const float& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    static __forceinline CRGBColour FromVector(const CFloatVector& v)
    {
        CRGBColour c;
        c.R = (unsigned char)v[0];
        c.G = (unsigned char)v[1];
        c.B = (unsigned char)v[2];
        c.A = (unsigned char)v[3];
        return c;
    }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) {}
    __forceinline explicit CRGBColour(const CFloatVector& v)
    {
        R = (unsigned char)v[0];
        G = (unsigned char)v[1];
        B = (unsigned char)v[2];
        A = (unsigned char)v[3];
    }
};
struct CSpecialAbilitiesPhysicalShieldDef : CDefPointee {
    unsigned char _pad08[0x50 - 0x08];
    long  LoopingSound[4];                            // +0x50
    unsigned char _pad60[0xe8 - 0x60];
    float WobbleRate;                                 // +0xe8
    CFloatVector ColorStartLevel1;                    // +0xec
    CFloatVector ColorStartLevel2;                    // +0xf8
    CFloatVector ColorStartLevel3;                    // +0x104
    CFloatVector ColorStartLevel4;                    // +0x110
    unsigned char _pad11c[0x134 - 0x11c];
    float FadeOutPhysicalShieldTimeSecs;              // +0x134
};
struct CShieldDefPointer {
    const CSpecialAbilitiesPhysicalShieldDef* Object;
    CShieldDefPointer() : Object(0) {}
    ~CShieldDefPointer() { if (Object) { if (--((CDefPointee*)Object)->RefCount == 0) ((CDefPointee*)Object)->Destroy(); Object = 0; } }
    const CSpecialAbilitiesPhysicalShieldDef* operator->() const { return Object; }
};
struct CDefinitionManager {
    long GetDefGlobalIndex(const CCharString& name, bool required);   // 0x009AD390
    bool GetDef(long index, CShieldDefPointer* out);  // 0x0079F0E0
    static CDefinitionManager* __fastcall Get();      // 0x0044C6B0
};
struct CThingPointerSet {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned long value; };
    node* head;
    unsigned long size;
    CThingPointerSet() : head(0)
    {
        head = (node*)malloc(sizeof(node));
        size = 0;
        head->colour = 0;
        head->parent = 0;
        head->left = head;
        head->right = head;
    }
    ~CThingPointerSet();
};
extern long g_FramesPerSecond;                        // 0x01375550
struct CTCBase {
    CTCBase(CThing* owner);                           // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                              // +0x04
    bool    ToBeRemoved;                              // +0x08
    bool    IsInGlobalUpdateTCs;                      // +0x09
};
struct CTCPhysicalShieldSpell : CTCBase {             // vtable 0x0126C53C
    CShieldDefPointer PDef;                           // +0x0c
    long  SpellLevel;                                 // +0x10
    long  ShieldFadeOutTimer;                         // +0x14
    long  CurShieldFadeOutTimer;                      // +0x18
    long  DurationTimer;                              // +0x1c
    float ShieldHealth;                               // +0x20
    float CurWobbleTimer;                             // +0x24
    float WobbleDelta;                                // +0x28
    CRGBColour ShieldColor;                           // +0x2c
    CIntelligentPointerThing ShieldEffect;            // +0x30
    CIntelligentPointerThing SwirlyEffect;            // +0x38
    unsigned long LoopingSound;                       // +0x40
    bool  Pausable;                                   // +0x44
    bool  Paused;                                     // +0x45
    bool  CutSceneCancelled;                          // +0x46
    CThingPointerSet AllowReactionAttackers;          // +0x48
    CTCPhysicalShieldSpell(CThing* owner);
    virtual ~CTCPhysicalShieldSpell();
    unsigned long PlaySound(const CCharString& name, bool looping);   // 0x0079DE00
    void PlayEffect(CBaseIntelligentPointer* effect, const CCharString& effectName, const CCharString& endEffectName,
                    CThingCreatureBase* creature, long attachType);   // 0x0079DEB0
    void PlayShieldEffects(CThingCreatureBase* creature);             // 0x0079E420
};

CTCPhysicalShieldSpell::CTCPhysicalShieldSpell(CThing* owner)
    : CTCBase(owner), SpellLevel(0), ShieldFadeOutTimer(0), CurShieldFadeOutTimer(0), DurationTimer(-1),
      ShieldHealth(0), CurWobbleTimer(0), WobbleDelta(0), ShieldEffect(0), SwirlyEffect(0), Pausable(true),
      Paused(false), CutSceneCancelled(false)
{
    long index = CDefinitionManager::Get()->GetDefGlobalIndex("SPECIAL_ABILITIES_PHYSICAL_SHIELD_DEF", true);
    CDefinitionManager::Get()->GetDef(index, &PDef);
    WobbleDelta = PDef->WobbleRate;
    if (ParentThing->m_presentBits[3] & 0x8000) {
        long type = 0x6f;
        const CInterfaceMap* map = &ParentThing->m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || 0x6f < it->type)
            it = map->last;
        SpellLevel = ((CTCInventoryAbilities*)it->component)->GetAbilityLevel(4);
    }
    switch (SpellLevel) {
    case 0:
        ShieldColor = CRGBColour(PDef->ColorStartLevel1);
        break;
    case 1:
        ShieldColor = CRGBColour(PDef->ColorStartLevel2);
        break;
    case 2:
        ShieldColor = CRGBColour(PDef->ColorStartLevel3);
        break;
    case 3:
        ShieldColor = CRGBColour(PDef->ColorStartLevel4);
        break;
    default:
        ShieldColor = CRGBColour(PDef->ColorStartLevel1);
        break;
    }
    CThing* parent = ParentThing;
    {
        CCharString soundName = g_DefStringTable.GetString(PDef->LoopingSound[SpellLevel]);
        LoopingSound = PlaySound(soundName, parent->m_type10 == 2);
    }
    PlayEffect(0, "CAST", "", (CThingCreatureBase*)parent, 8);
    PlayShieldEffects((CThingCreatureBase*)parent);
    ((CThingCreatureBase*)parent)->MakeNoise("SIMPLE_NOISE");
    CurShieldFadeOutTimer = ShieldFadeOutTimer = (long)(g_FramesPerSecond * PDef->FadeOutPhysicalShieldTimeSecs);
}
