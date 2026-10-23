// CTCDivineWrath::CTCDivineWrath(CThing* thing) -- set up the Divine Wrath spell: members cleared, the ability flag on the caster, the spell def and the caster's ability level, per-level frame timers, the environment theme transition, the caster's lighting channel, the ring emitter (scaled between the small and large ring effects) and the two hand effects, then the start and loop sounds
// Grade: DIFFER(2000v2034) -- frame 0x20 v 0x1c (retail zeroes a condition-temp flag at entry, spills def/layer counter to the dead param slot); hand-effect block order; PlaySound arg order
// Leaf: ??0CTCDivineWrath
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078ECD0, 2048 bytes, `ret 4`, no EH frame.
// Callees: CTCBase ctor 0x00686800, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor
// 0x00A01C10, CWillButtonHelperTimer ctor 0x008EDFF0, malloc 0x00BFEA0E, LowerBound 0x0040F020 / const 0x004365B0,
// CTCBase::GetDefinitionManager 0x006869D0 / GetWorld 0x00686A70, CDefinitionManager::FindDefIndex 0x009AD390 /
// GetDefGlobalIndexFromName 0x009AD410 / GetDef 0x005F8B1B, CTCInventoryAbilities::GetAbilityLevel 0x005EDE93, __ftol2,
// CEnvironment::TransitionToTheme 0x006B3800, CTCGraphicAppearance::SetLightingChannel 0x004BFB40, GetSubDef 0x00669580,
// CEffectsDef::CalcAndLookup 0x00669D30 (inlined at the HAND_EFFECT site), emitter creator 0x006E11F0, CThing::GetPosition
// 0x004C73D0, CreateParticleEffect 0x0072AB70, CTCDParticleEmitter::AttachToThing 0x006E0BE0 / SetAlwaysUpdate 0x006E0590,
// CDefStringTable::GetString 0x009D49B0, CTCDivineWrath::PlaySound 0x0078EBF0.
// Layout: retail (no PDB fit): PDef +0x0c, PTarget +0x10, WillTimer +0x1c, Level +0x50, timers +0x54..+0x60, the packed
// creature pointer +0x6d, a map header +0x78, ring emitter +0x9c, saved lighting +0xa4, lighting channel +0xa8, hand effects
// +0xac / +0xb4, loop sound +0xbc, +0xc0.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);               // 0x00BFEA0E
struct C3DVector { float X, Y, Z; };
struct CStringData { const char* Data; unsigned long Length; };
struct CCharString {
    CStringData* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
extern const unsigned long g_CRCTable[256];                  // 0x0129A168
extern long g_FramesPerSecond;                               // 0x01375550
extern CCharString g_LeftHandDummy;                          // 0x013BAC30
extern CCharString g_RightHandDummy;                         // 0x013BAC34
struct CDefStringTable { CCharString GetString(long index); };   // 0x009D49B0
extern CDefStringTable g_DefStringTable;                     // 0x013CA828
struct CFloatVector { float* _M_start; float* _M_finish; float* _M_end; float* begin() const { return _M_start; } };
struct CLongVector { long* _M_start; long* _M_finish; long* _M_end; };
struct SMapNode {
    unsigned char Color;
    SMapNode* Parent;
    SMapNode* Left;                                          // +0x08
    SMapNode* Right;                                         // +0x0c
    unsigned long Key;                                       // +0x10
    long Value;                                              // +0x14
};
struct CEffectsDef {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +0x04
    unsigned char _pad08[0x28 - 0x08];
    SMapNode* Effects;                                       // +0x28 map header
    bool CalcAndLookup(const CCharString& name, long* out) const;   // 0x00669D30
    __forceinline bool Lookup(const CCharString& name, long* out) const
    {
        const char* data;
        unsigned long length;
        if (!name.PStringData) {
            length = 0;
            data = "";
        } else {
            length = name.PStringData->Length;
            data = name.PStringData->Data;
        }
        unsigned long crc = 0;
        if (length) {
            for (unsigned long i = 0; i < length; i++)
                crc = g_CRCTable[(unsigned char)data[i] ^ (crc & 0xff)] ^ (crc >> 8);
        } else
            crc = 0;
        SMapNode* y = Effects;
        for (SMapNode* x = Effects->Parent; x; ) {
            if (!(x->Key < crc)) {
                y = x;
                x = x->Left;
            } else
                x = x->Right;
        }
        if (y == Effects || crc < y->Key)
            y = Effects;
        if (y == Effects)
            return false;
        *out = y->Value;
        return true;
    }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CDivineWrathDef {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +0x04
    unsigned char _pad08[0x3c - 0x08];
    CLongVector StartSounds;                                 // +0x3c
    CLongVector LoopSounds;                                  // +0x48
    unsigned char _pad54[0x88 - 0x54];
    CFloatVector DurationA;                                  // +0x88
    unsigned char _pad94[0xa0 - 0x94];
    CFloatVector DurationB;                                  // +0xa0
    CFloatVector RingRadius;                                 // +0xac
    CFloatVector DurationC;                                  // +0xb8
    unsigned char _padc4[0xdc - 0xc4];
    CFloatVector PulseRate;                                  // +0xdc
    unsigned char _pade8[0xf4 - 0xe8];
    float MinRingRadius;                                     // +0xf4
    float MaxRingRadius;                                     // +0xf8
    bool GetSubDef(CDefPointer<CEffectsDef>* out) const;     // 0x00669580
};
struct CDefinitionManager {
    long FindDefIndex(const CCharString& name, bool a);      // 0x009AD390
    long GetDefGlobalIndexFromName(const CCharString& name); // 0x009AD410
    bool GetDef(long index, CDefPointer<CDivineWrathDef>* out);   // 0x005F8B1B
};
struct CEnvironment { void TransitionToTheme(long layer, long mode, long theme, float time); };   // 0x006B3800
struct CWorld { unsigned char _pad[0x18]; CEnvironment* Environment; };
struct SLightingChannel {
    unsigned char A, B, C, D;
    SLightingChannel() : A(0), B(0), C(0) { }
    SLightingChannel(unsigned char a, unsigned char b, unsigned char c) : A(a), B(b), C(c) { }
};
struct CTCGraphicAppearance {
    unsigned char _pad00[0x40];
    SLightingChannel LightingChannel;                        // +0x40
    void SetLightingChannel(const SLightingChannel& channel, bool a, bool b);   // 0x004BFB40
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
    const SEntry* LowerBound(const long* type) const;        // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* Appearance;                        // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Flags91;                                   // +0x91 bit 0 Dead
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline const T* GetTCConst(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (const T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCOrNull(long interfaceType)
    {
        T* tc = 0;
        if (HasTC(interfaceType))
            tc = GetTC<T>(interfaceType);
        return tc;
    }
};
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
    void SetAlwaysUpdate(bool always);                       // 0x006E0590
};
CThing* __fastcall CreateEmitter(long smallIndex, long largeIndex, float scale, const C3DVector& pos, long flags);   // 0x006E11F0
CThing* __fastcall CreateParticleEffect(long index, const C3DVector& pos);   // 0x0072AB70
struct CTCInventoryAbilities { long GetAbilityLevel(long ability) const; };   // 0x005EDE93
struct CTCSpellFlags { unsigned char _pad[0x0c]; bool Casting; unsigned char _pad0d[3]; long Spell; };   // interface 0xa7
struct CTCFlag75 { unsigned char _pad[0x20]; bool Enabled; };                                          // interface 0x75
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    CBaseIntelligentPointer();                               // 0x00A01B10
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CThing* item);                             // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {          // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {       // vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) { SetPItem(item); }
};
struct CWillButtonHelperTimer {
    unsigned char _body[0x34];
    CWillButtonHelperTimer(long ability, CThing* thing);     // 0x008EDFF0
};
struct STreeHeader {
    SMapNode* Header;
    long Count;
    STreeHeader() : Header(0)
    {
        Header = (SMapNode*)malloc(0x2c);
        Count = 0;
        Header->Color = 0;
        Header->Parent = 0;
        Header->Left = Header;
        Header->Right = Header;
    }
};
class CTCBaseImpl {
public:
    CTCBaseImpl(CThing* thing);                              // 0x00686800
    virtual ~CTCBaseImpl();
    CThing* ParentThing;                                     // +0x04
    bool    ToBeRemoved;                                     // +0x08
    bool    IsInGlobalUpdateTCs;                             // +0x09
    unsigned char _pad0a[2];
    CDefinitionManager* GetDefinitionManager();              // 0x006869D0
    CWorld* GetWorld();                                      // 0x00686A70
};
class CTCDivineWrath : public CTCBaseImpl {
public:
    CDefPointer<CDivineWrathDef> PDef;                       // +0x0c
    CIntelligentPointerThing PTarget;                        // +0x10
    bool Finished;                                           // +0x18
    unsigned char _pad19[3];
    CWillButtonHelperTimer WillTimer;                        // +0x1c
    long Level;                                              // +0x50
    long TimerA;                                             // +0x54
    long TimerB;                                             // +0x58
    long TimerC;                                             // +0x5c
    long Counter60;                                          // +0x60
    bool Flag64;                                             // +0x64
    unsigned char _pad65[3];
    long Counter68;                                          // +0x68
    bool Flag6c;                                             // +0x6c
    CIntelligentPointerCreature PCreature;                   // +0x6d
    bool Flag75;                                             // +0x75
    bool Flag76;                                             // +0x76
    unsigned char _pad77;
    STreeHeader HitThings;                                   // +0x78
    unsigned char _pad80[4];
    long Value84, Value88, Value8c;                          // +0x84
    float PulseFraction;                                     // +0x90
    long Value94;                                            // +0x94
    long PulseFrames;                                        // +0x98
    CIntelligentPointerThing PRing;                          // +0x9c
    SLightingChannel SavedLighting;                          // +0xa4
    SLightingChannel Lighting;                               // +0xa8
    CIntelligentPointerThing PLeftHand;                      // +0xac
    CIntelligentPointerThing PRightHand;                     // +0xb4
    unsigned long LoopSound;                                 // +0xbc
    CIntelligentPointerThing PExtra;                         // +0xc0
    unsigned long PlaySound(const CCharString& name, bool loop);   // 0x0078EBF0
    CTCDivineWrath(CThing* thing);
};
#pragma pack(pop)

CTCDivineWrath::CTCDivineWrath(CThing* thing)
    : CTCBaseImpl(thing), PTarget(0), Finished(false), WillTimer(0x12, ParentThing), Level(0), TimerA(0), TimerB(0),
      TimerC(0), Counter60(0), Flag64(false), Counter68(0), Flag6c(false), PCreature(0), Flag75(false), Flag76(false),
      Value84(0), Value88(0), Value8c(0), PulseFraction(0), Value94(0), PulseFrames(0), PRing(0), Lighting(5, 5, 0xff)
{
    LoopSound = 0;
    if (ParentThing->HasTC(0xa7)) {
        CTCSpellFlags* flags = ParentThing->GetTC<CTCSpellFlags>(0xa7);
        flags->Casting = true;
        flags->Spell = 0x12;
    }
    long defIndex = GetDefinitionManager()->FindDefIndex("SPECIAL_ABILITIES_DIVINE_WRATH_DEF", true);
    GetDefinitionManager()->GetDef(defIndex, &PDef);
    if (ParentThing->HasTC(0x6f))
        Level = ParentThing->GetTCConst<CTCInventoryAbilities>(0x6f)->GetAbilityLevel(0x12);
    CDivineWrathDef* def = PDef.Object;
    TimerA = (long)(g_FramesPerSecond * def->DurationA._M_start[Level]);
    TimerB = (long)(g_FramesPerSecond * def->DurationB._M_start[Level]);
    TimerC = (long)(g_FramesPerSecond * def->DurationC._M_start[Level]);
    long theme = GetDefinitionManager()->GetDefGlobalIndexFromName("ENVIRONMENT_DIVINE_WRATH");
    if (theme > 0) {
        for (long layer = 0; layer < 6; layer++) {
            if (layer != 5)
                GetWorld()->Environment->TransitionToTheme(layer, 2, theme, *(PDef->DurationC.begin() + Level));
        }
    }
    if (ParentThing->Appearance) {
        SavedLighting = ParentThing->Appearance->LightingChannel;
        ParentThing->Appearance->SetLightingChannel(Lighting, true, true);
    }
    if (ParentThing->HasTC(0x75))
        ParentThing->GetTC<CTCFlag75>(0x75)->Enabled = false;
    def = PDef.Object;
    float pulse = def->PulseRate._M_start[Level] / g_FramesPerSecond;
    PulseFrames = (long)pulse;
    PulseFraction = pulse - PulseFrames;
    CDefPointer<CEffectsDef> effects;
    long smallRing = 0;
    long largeRing = 0;
    if (def->GetSubDef(&effects)) {
        if (effects->CalcAndLookup(CCharString("SMALL_RING_EFFECT"), &smallRing)
            && effects->CalcAndLookup(CCharString("LARGE_RING_EFFECT"), &largeRing)) {
            float scale = (def->RingRadius._M_start[Level] - def->MinRingRadius) / (def->MaxRingRadius - def->MinRingRadius);
            PRing = CIntelligentPointerThing(CreateEmitter(smallRing, largeRing, scale, ParentThing->GetPosition(), 0));
            if (PRing.GetPItem() && !(PRing.GetPItem()->Flags91 & 1)) {
                CTCDParticleEmitter* emitter = PRing.GetPItem()->GetTCOrNull<CTCDParticleEmitter>(0x4b);
                emitter->AttachToThing(ParentThing, 8, "", 0, 0.0f);
                emitter->SetAlwaysUpdate(true);
            }
        }
    }
    long handEffect = 0;
    if (effects->Lookup(CCharString("HAND_EFFECT"), &handEffect)) {
        PLeftHand = CIntelligentPointerThing(CreateParticleEffect(handEffect, ParentThing->GetPosition()));
        PRightHand = CIntelligentPointerThing(CreateParticleEffect(handEffect, ParentThing->GetPosition()));
        if (PLeftHand.GetPItem() && !(PLeftHand.GetPItem()->Flags91 & 1) && PLeftHand.GetPItem()->HasTC(0x4b)) {
            CTCDParticleEmitter* emitter = PLeftHand.GetPItem()->GetTC<CTCDParticleEmitter>(0x4b);
            if (emitter)
                emitter->AttachToThing(ParentThing, 0xa, g_LeftHandDummy, 0, 0.0f);
        }
        if (PRightHand.GetPItem() && !(PRightHand.GetPItem()->Flags91 & 1)) {
            CTCDParticleEmitter* emitter = PRightHand.GetPItem()->GetTCOrNull<CTCDParticleEmitter>(0x4b);
            emitter->AttachToThing(ParentThing, 0xa, g_RightHandDummy, 0, 0.0f);
        }
    }
    PlaySound(g_DefStringTable.GetString(PDef->StartSounds._M_start[Level]), false);
    LoopSound = PlaySound(g_DefStringTable.GetString(PDef->LoopSounds._M_start[Level]), false);
}
