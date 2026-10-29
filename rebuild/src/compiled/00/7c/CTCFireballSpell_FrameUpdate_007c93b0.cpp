// CTCFireballSpell::FrameUpdate() -- per frame: count down the release delay, tick the will-button timer, spawn the build-up effect
// once (attached to the fireball helper), run the level-up effects, drive the held state and remove the spell when finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C93B0, 444 bytes (+ int3 padding), `ret` (vtable 0x0126E964 slot 9).
// Leaf: ?FrameUpdate@CTCFireballSpell
// Callees: CWillButtonHelperTimer::FrameUpdate 0x008EDDD0, GetSubDef<CSpecialEffectsDef> 0x00669580, GetEffectFromString 0x00669D30,
// GetPos 0x004C73D0, CreateParticleEffect 0x0072AB70, CBaseIntelligentPointer 0x00A01B10/B90/B50/C10, LowerBound 0x0040F020,
// AttachToThing 0x006E0BE0, ProcessLevellingUpFX 0x007C8DE0, the held-state helper 0x007C9180, CThing::RemoveTC 0x004C9840.
// Shape: the Kraken/OnDie def lookup (out slot read once into a named pointer released by hand), an implicit-literal `&&` temp,
// the effect through an intelligent-pointer temporary + operator=, an uninitialised emitter local; first try.
// Names: catalog "FrameUpdate" sits on 0x007C9180, but this row is the vtable slot-9 override; 0x007C9180 is a non-virtual helper.
// Layout: retail CTCFireballSpell = Ego_r.pdb minus 4 after the timer; timer CurrentState +0x20, WillIncrementedFlag +0x38.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:          // the out slot GetSubDef fills
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20 UsingComponents
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)((CTCBase_Retail*)it->second);
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
    void RemoveTC(long interfaceType);          // 0x004C9840
    __forceinline unsigned long HasTC(long kind) const { return m_presentBits[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase_Retail* FindTC(long kind)
    {
        ETCInterfaceType type = (ETCInterfaceType)(kind);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return ((CTCBase_Retail*)((it == map->last || kind < it->first) ? map->last : it)->second);
    }
};
class CThingCreatureBase : public CThing { public:
    void MakeNoise(const CCharString& noise);   // 0x00662110
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& position);   // 0x0072AB70
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CTCSoundPlayer { public:                         // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CWillButtonHelperTimer {                 // retail 0x34 bytes
    void FrameUpdate();                         // 0x008EDDD0
    long ButtonHeldDownTimer;                   // +0x00
    long WillAccumulatedLevel;                  // +0x04
    unsigned char PCreature[8];                 // +0x08 CIntelligentPointer<CThing>
    long MyAbility;                             // +0x10
    long CurrentState;                          // +0x14
    void* WillPowerUpAccumulatedTimings[3];     // +0x18
    float MaxNumberLevels;                      // +0x24
    float CurMaxLevels;                         // +0x28
    bool WillIncrementedFlag;                   // +0x2c
    bool UpdateAccLevel;
    bool IsAICasting;
    bool IsAIHoldingDown;
    long LevelToCast;                           // +0x30
};

class CTCFireballSpell {
public:
    virtual void v00();
    CThingCreatureBase* ParentThing;            // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CWillButtonHelperTimer Timer;               // +0x0c
    CDefClassBase* PDef;                        // +0x40 CDefPointer<CSpecialAbilitiesFireballSpellDef const>
    unsigned long InitLoopingSound;             // +0x44
    CIntelligentPointer<CThing> FireballBuildUpEffect;  // +0x48
    bool ImmediateCastMode;                     // +0x50
    bool OneAnimCastMode;                       // +0x51
    unsigned char PTarget[8];                   // +0x52
    unsigned char PItemRemoved[8];              // +0x5a
    long WeaponState;                           // +0x64
    bool LeftHandAnim;                          // +0x68
    CCharString FireballAttachmentName;         // +0x6c
    bool StartEffect;                           // +0x70
    long MaxLevelAllowableByStamina;            // +0x74
    long ReleaseDelayFrameCount;                // +0x78
    bool SpellFinished;                         // +0x7c
    void ProcessLevellingUpFX();                // 0x007C8DE0
    void UpdateButtonHeld();                    // 0x007C9180 (catalog "FrameUpdate"; non-virtual, called only from here)
    void FrameUpdate();
};

void CTCFireballSpell::FrameUpdate()
{
    if (SpellFinished)
        --ReleaseDelayFrameCount;
    Timer.FrameUpdate();
    CThingCreatureBase* parent = ParentThing;
    if (StartEffect) {
        StartEffect = false;
        CDefPointer<CSpecialEffectsDef> effectsSlot;
        long effectType = 0;
        bool found = PDef->GetSubDef(effectsSlot) && effectsSlot.m_p->GetEffectFromString("BUILDUP_EFFECT", effectType);
        CSpecialEffectsDef* effects = effectsSlot.m_p;
        if (found) {
            FireballBuildUpEffect = CIntelligentPointer<CThing>(CreateParticleEffect(effectType, parent->GetPos()));
            if (IS_THING_ALIVE(FireballBuildUpEffect)) {
                CTCDParticleEmitter* emitter;
                CThing* effect = FireballBuildUpEffect.GetPItem();
                if (effect->HasTC(0x4b))
                    emitter = (CTCDParticleEmitter*)effect->FindTC(0x4b);
                emitter->AttachToThing(*ParentThing, 10, FireballAttachmentName, 0, 0.0f);
            }
        }
        ReleaseDef(effects);
    }
    if (Timer.WillIncrementedFlag && Timer.WillAccumulatedLevel <= MaxLevelAllowableByStamina)
        ProcessLevellingUpFX();
    switch (Timer.CurrentState) {
    case 2:
        UpdateButtonHeld();
        break;
    case 3:
        if (SpellFinished && ReleaseDelayFrameCount <= 0)
            parent->RemoveTC(0x9d);
        break;
    }
}