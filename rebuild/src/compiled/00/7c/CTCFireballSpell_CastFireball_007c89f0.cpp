// CTCFireballSpell::CastFireball(CThing* target) -- release the charged fireball: clamp the accumulated will level to what the stamina allows, take the level's stamina cost from a hero, kill the build-up effect, stop the looping charge sound and play the level's cast criteria, read the fireball attachment dummy for the start position and frame, pick the FIREBALL_<level+1> shot definition, tilt the caster's frame about its side axis by the def's inclination, create the shot at the target and make it undrawable, immune to bullet time and undeflectable
// Leaf: ?CastFireball@CTCFireballSpell@@QAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C89F0, 966 bytes of code + `mov edi,edi` + two 4-entry jump tables (row 1000), `ret 4`.
// Identity: Ego_r.pdb has `?CastFireball@CTCFireballSpell@@QAEXPAVCThing@@@Z` (donor 0x007C7730); the literals are the four
//   HERO_ABILITY_FIREBALL_CAST_* criteria (0x0126EA8C..0x0126EA20) and FIREBALL_1..4 (0x0126EA14..0x0126E9F0). Jump tables
//   checked entry by entry: 0x007C8DB8 -> +0xE6/+0x110/+0x13A/+0x164 (cases 0..3 in order), 0x007C8DC8 -> +0x1C6/+0x1EF/
//   +0x218/+0x241 (cases 0..3 in order); ours give the same offsets.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline CThing::GetTC<T>(T**), PDB
//   `??$GetTC@VCTCFireballSpell@@@CThing@@QAE_NPAPAVCTCFireballSpell@@@Z` proves the out-parameter form), _ftol 0x00BFEA70,
//   CTCHeroStats::ModifyStaminaBy(long) 0x0057B1DB, CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill(bool)
//   0x004C9B80, CTCSoundPlayer::StopAttachedSound(unsigned long) 0x0066ADD0 / PlayAttachedCriteria 0x0066BD00,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, NGraphicAppearance::CTCGraphicAppearance::
//   GetDummyObject 0x004C08C0, CThing::PeekDefinitionManager() const 0x004C7990, CDefinitionManager::
//   GetDefGlobalIndexFromName 0x009AD410, CTCPhysicsBase virtual slots 70 GetFacingVector() (+0x118) and 72 GetLastRHSet()
//   (+0x120), C3DVector::Rotate 0x00A13C60, CRightHandedSet::Rotate 0x00A22F80 (Ego_r `?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z`),
//   NThingShot::CreateShot 0x007C0AD0 (__fastcall), CThing virtual slot 28 SetAsDrawable (+0x70, 0x004C8E50),
//   NThingShot::CTCShotDefinitionBase::SetCanBeDeflected(bool) 0x007BED50.
// Interface types (vtable slot 21): CTCHeroStats 4, CTCSoundPlayer 0x10, NThingShot::CTCShotDefinitionBase 0x77 (vtable
//   0x0126E1A4 slot 21 = 0x004D5472 `push 77h ; pop eax ; ret`).
// Layout: Ego_r.pdb CTCFireballSpell (0x84) with the 12-byte retail STLport vector inside CWillButtonHelperTimer (PDB 0x38,
//   retail 0x34): Timer +0x0C (WillAccumulatedLevel +0x10), then PDB less 4: PDef +0x40, InitLoopingSound +0x44,
//   FireballBuildUpEffect +0x48, FireballAttachmentName +0x6C, MaxLevelAllowableByStamina +0x74. Def: Inclination +0x40,
//   StaminaCost +0x44 (Ego_r less 4: CParentDefClassBase is 0x3C in retail). CThing: UsingComponents +0x20, Components
//   +0x44, PhysicsTC +0x60, PTCGraphicAppearance +0x64, flag bytes +0x91..+0x93 in PDB bitfield order (Dead +0x91 bit 0,
//   IsAffectedByBulletTimeOverride +0x93 bit 5). No engine header for CTCFireballSpell or CThing.
// Shape: the clamp is `if (<0) =0; else if (>max) =max`; ParentThing is re-read per lookup and named once before the dummy
//   read; the frame is reused (dummy frame, then overwritten by GetLastRHSet); the side axis is passed as an explicit
//   C3DVector copy (retail copies it into the dead up-axis temporary); the default CDefPointer temporary releases through
//   CIVCountedPointeeBase::Release() (count returned) then nulls; the 0x77 lookup key lands in the dead `target` slot.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    typedef const _Tp* const_iterator;
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const _Tp& operator[](unsigned int __n) const { return *(begin() + __n); }
};
template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static _WordT _S_maskbit(unsigned int __pos) { return 1UL << (__pos % 32); }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    struct reference {
        typedef _Base_bitset<(_Nb + 31) / 32> _Bitset_base;
        unsigned long* _M_wp;
        unsigned int _M_bpos;
        reference(bitset<_Nb>& __b, unsigned int __pos)
        {
            _M_wp = &__b._M_w[_Bitset_base::_S_whichword(__pos)];
            _M_bpos = __pos % 32;
        }
        ~reference() { }
        operator bool() const { return (*(_M_wp) & _Bitset_base::_S_maskbit(_M_bpos)) != 0; }
    };
    reference operator[](unsigned int __pos) { return reference(*this, __pos); }
};
}   // namespace std

enum ETCInterfaceType { };
class CTCBase;
class CCharString { public:
    void* PStringData;                                      // +0x00
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Rotate(const C3DVector& axis, float angle);       // 0x00A13C60
};
class CRightHandedSet { public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0C
    CRightHandedSet() { }
    void Rotate(const C3DVector& axis, float angle);       // 0x00A22F80
};
template <class T> class CArray : public std::vector<T> { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    value_type* LowerBound(const K& key);                     // 0x0040F020 for the TC map
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public: };
class CTCPhysicsBase;
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& position,
        CRightHandedSet& frame, float scale) const;         // 0x004C08C0
};
}
class CThing { public:                                      // partial view, retail offsets; vtable 0x01239ABC
    virtual void CUnprovenSlot00_01239abc(); virtual void CUnprovenSlot01_01239abc(); virtual void CUnprovenSlot02_01239abc(); virtual void CUnprovenSlot03_01239abc();
    virtual void CUnprovenSlot04_01239abc(); virtual void CUnprovenSlot05_01239abc(); virtual void CUnprovenSlot06_01239abc(); virtual void CUnprovenSlot07_01239abc();
    virtual void CUnprovenSlot08_01239abc(); virtual void CUnprovenSlot09_01239abc(); virtual void CUnprovenSlot10_01239abc(); virtual void CUnprovenSlot11_01239abc();
    virtual void CUnprovenSlot12_01239abc(); virtual void CUnprovenSlot13_01239abc(); virtual void CUnprovenSlot14_01239abc(); virtual void CUnprovenSlot15_01239abc();
    virtual void CUnprovenSlot16_01239abc(); virtual void CUnprovenSlot17_01239abc(); virtual void CUnprovenSlot18_01239abc(); virtual void CUnprovenSlot19_01239abc();
    virtual void CUnprovenSlot20_01239abc(); virtual void CUnprovenSlot21_01239abc(); virtual void CUnprovenSlot22_01239abc(); virtual void CUnprovenSlot23_01239abc();
    virtual void CUnprovenSlot24_01239abc(); virtual void CUnprovenSlot25_01239abc(); virtual void CUnprovenSlot26_01239abc(); virtual void CUnprovenSlot27_01239abc();
    virtual void SetAsDrawable(bool drawable);              // 28 (+0x70, 0x004C8E50)
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      _pad_0x04[0x20 - 0x04];
    std::bitset<274>   UsingComponents;                     // +0x20
    TCMap              Components;                          // +0x44
    unsigned char      _pad_0x54[0x60 - 0x54];
    CTCPhysicsBase*    PhysicsTC;                           // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char      _pad_0x68[0x91 - 0x68];
    unsigned char      Dead : 1;                            // +0x91 bit 0
    unsigned char      CreatedByOwnerThingCreation : 1;
    unsigned char      IncludeInThingScans : 1;
    unsigned char      Activated : 1;
    unsigned char      GloballyLoaded : 1;
    unsigned char      InLimbo : 1;
    unsigned char      KillLocked : 1;
    unsigned char      CreatedBySerialisation : 1;
    unsigned char      DeletedOnOwnerLevelOutOfScope : 1;   // +0x92
    unsigned char      Drawable : 1;
    unsigned char      Dying : 1;
    unsigned char      ForceFrameUpdate : 1;
    unsigned char      LevelPersistent : 1;
    unsigned char      GameStatePersisted : 1;
    unsigned char      PersistOnUnloadWhenInLimbo : 1;
    unsigned char      PausedDuringCutscenes : 1;
    unsigned char      NotPausedDuringCutscenes : 1;        // +0x93
    unsigned char      DeathPersistent : 1;
    unsigned char      KillOnLevelUnload : 1;
    unsigned char      ActivatedScript : 1;
    unsigned char      LevelIsUnloading : 1;
    unsigned char      IsAffectedByBulletTimeOverride : 1;  // +0x93 bit 5
    bool IsDead() const { return Dead; }
    void Kill(bool immediately);                            // 0x004C9B80
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x004C7990
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            TCMap* map = &Components;
            TCMap::value_type* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CPersistContext;
class CTCParamsBase;
class CEnginePrimitivePrepareInfo;
namespace NTCMessages { class CMessageBase; }
class CTCBase : public CBaseClassNonCopyable { public:     // retail vtable: 28 slots (editor-only virtuals absent)
    virtual ~CTCBase();                                                          // 0  (0x00686830)
protected:
    virtual void OnSerialise(CPersistContext& context, bool loading);            // 1
public:
    virtual void OnRemove();                                                     // 2
    virtual void ConstructFromParams(const CTCParamsBase& params);               // 3
    virtual void OnCreate();                                                     // 4
    virtual void InitialActivate();                                              // 5
    virtual void OnInitialActivate();                                            // 6
    virtual void RenderUpdate(float elapsed);                                    // 7
    virtual bool HasFrameUpdate() const;                                         // 8
    virtual void FrameUpdate();                                                  // 9
    virtual void BulletTimeFrameUpdate();                                        // 10
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 11
    virtual void Copy(const CTCBase& other);                                     // 12
    virtual void OnDie();                                                        // 13
    virtual void OnKill();                                                       // 14
    virtual void OnLevelUnload();                                                // 15
    virtual void OnSetInLimbo(bool inLimbo);                                     // 16
    virtual bool Validate(CCharString& error) const;                             // 17
    virtual bool IsFreeForDeletion() const;                                      // 18
    virtual bool IsSerialiseCreatable() const;                                   // 19
    virtual bool IsSerialisableForHeroState() const;                             // 20
    virtual ETCInterfaceType GetInterfaceType() const = 0;                       // 21
    virtual CCharString GetClassTypeName() const = 0;                            // 22
    virtual bool IsComponentSerialisable() const;                                // 23
    virtual bool IsDriver() const;                                               // 24
    virtual bool IsCopyable() const;                                             // 25
    virtual void GlobalUpdate(bool force);                                       // 26
    virtual void HandleMessage(const NTCMessages::CMessageBase& message);        // 27
    CThing* ParentThing;                                                         // +0x04
    bool    ToBeRemoved;                                                         // +0x08
    bool    IsInGlobalUpdateTCs;                                                 // +0x09
};
class CTCPhysicsBase : public CTCBase { public:            // vtable 0x0125D2C4
    // slots 28-68 are not called here; no PDB-proven names in slot order (placeholders, NOTES.md "Descriptive")
    virtual void CUnprovenSlot28_0125d2c4(); virtual void CUnprovenSlot29_0125d2c4(); virtual void CUnprovenSlot30_0125d2c4(); virtual void CUnprovenSlot31_0125d2c4();
    virtual void CUnprovenSlot32_0125d2c4(); virtual void CUnprovenSlot33_0125d2c4(); virtual void CUnprovenSlot34_0125d2c4(); virtual void CUnprovenSlot35_0125d2c4();
    virtual void CUnprovenSlot36_0125d2c4(); virtual void CUnprovenSlot37_0125d2c4(); virtual void CUnprovenSlot38_0125d2c4(); virtual void CUnprovenSlot39_0125d2c4();
    virtual void CUnprovenSlot40_0125d2c4(); virtual void CUnprovenSlot41_0125d2c4(); virtual void CUnprovenSlot42_0125d2c4(); virtual void CUnprovenSlot43_0125d2c4();
    virtual void CUnprovenSlot44_0125d2c4(); virtual void CUnprovenSlot45_0125d2c4(); virtual void CUnprovenSlot46_0125d2c4(); virtual void CUnprovenSlot47_0125d2c4();
    virtual void CUnprovenSlot48_0125d2c4(); virtual void CUnprovenSlot49_0125d2c4(); virtual void CUnprovenSlot50_0125d2c4(); virtual void CUnprovenSlot51_0125d2c4();
    virtual void CUnprovenSlot52_0125d2c4(); virtual void CUnprovenSlot53_0125d2c4(); virtual void CUnprovenSlot54_0125d2c4(); virtual void CUnprovenSlot55_0125d2c4();
    virtual void CUnprovenSlot56_0125d2c4(); virtual void CUnprovenSlot57_0125d2c4(); virtual void CUnprovenSlot58_0125d2c4(); virtual void CUnprovenSlot59_0125d2c4();
    virtual void CUnprovenSlot60_0125d2c4(); virtual void CUnprovenSlot61_0125d2c4(); virtual void CUnprovenSlot62_0125d2c4(); virtual void CUnprovenSlot63_0125d2c4();
    virtual void CUnprovenSlot64_0125d2c4(); virtual void CUnprovenSlot65_0125d2c4(); virtual void CUnprovenSlot66_0125d2c4(); virtual void CUnprovenSlot67_0125d2c4();
    virtual void CUnprovenSlot68_0125d2c4();
    // overloads sit in reverse declaration order: () -> slot 70 (0x006B1410), (float) -> slot 69 (0x006B1430)
    virtual C3DVector GetFacingVector() const;                                   // 70 (+0x118)
    virtual C3DVector GetFacingVector(float time) const;                         // 69 (+0x114)
    virtual CRightHandedSet GetRHSet(float time) const;                          // 71 (+0x11C, 0x006B1450)
    virtual const CRightHandedSet& GetLastRHSet() const;                         // 72 (+0x120, 0x006B1290)
};

class CTCHeroStats : public CTCBase { public:              // interface type 4
    enum { INTERFACE_TYPE = 4 };
    void ModifyStaminaBy(long amount);                      // 0x0057B1DB
};
class CTCSoundPlayer : public CTCBase { public:            // interface type 0x10
    enum { INTERFACE_TYPE = 0x10 };                         // GetInterfaceType 0x0066C290 (vtable 0x0125B1C4 slot 21)
    void StopAttachedSound(unsigned long sound);            // 0x0066ADD0
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d);   // 0x0066BD00
};
namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase { public:     // vtable 0x0126E1A4, interface type 0x77 (slot 21 0x004D5472)
    enum { INTERFACE_TYPE = 0x77 };
    void SetCanBeDeflected(bool canBeDeflected);            // 0x007BED50
};
}

class CBaseObjectPointer;
class CBaseObject;
#pragma pack(push, 1)
class CBaseIntelligentPointer { public:                     // vtable 0x0129C95C, 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return GetPointer(); }
    T* operator->() const { return GetPointer(); }
private:
    T* GetPointer() const { return (T*)GetPItem(); }
};
#pragma pack(pop)
template <class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    if (p.Get() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
    long Release() const { long count = (long)--IVRefCount; if (count == 0) Delete(); return count; }
};
class CResource : public CIVCountedPointeeBase { public: };
class CDefPointeeBase : public CResource { public: };
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;                                // +0x00
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { Object->Release(); Object = 0; } }
    const T* operator->() const { return (const T*)Object; }
};
class CMeleeCombatAbilityDef;
namespace NThingShot {
CThing* __fastcall CreateShot(long defIndex, CThing* shooter, CThing* origin, const CThing* target, float accuracy,
    float a, float b, const CDefPointer<const CMeleeCombatAbilityDef>& ability, const C3DVector* position,
    const CRightHandedSet* frame);                          // 0x007C0AD0
}

class CSpecialAbilitiesFireballSpellDef { public:         // retail offsets (Ego_r less 4)
    unsigned char      _pad_0x00[0x3C];                     // CParentDefClassBase (retail 0x3C)
    float              ReleaseDelay;                        // +0x3C
    float              Inclination;                         // +0x40
    std::vector<float> StaminaCost;                         // +0x44
    float              TargettingArcAngle;                  // +0x50
};
enum EHeroAbility { };
enum EButtonHeldState { };
enum EWEAPON_STATE { };
class CWillButtonHelperTimer { public:                     // retail 0x34 (STLport vector 0xC)
    long                       ButtonHeldDownTimer;         // +0x00
    long                       WillAccumulatedLevel;        // +0x04
    CIntelligentPointer<CThing> PCreature;                  // +0x08
    EHeroAbility               MyAbility;                   // +0x10
    EButtonHeldState           CurrentState;                // +0x14
    std::vector<float>         WillPowerUpAccumulatedTimings;   // +0x18
    float                      MaxNumberLevels;             // +0x24
    float                      CurMaxLevels;                // +0x28
    bool                       WillIncrementedFlag;         // +0x2C
    bool                       UpdateAccLevel;              // +0x2D
    bool                       IsAICasting;                 // +0x2E
    bool                       IsAIHoldingDown;             // +0x2F
    long                       LevelToCast;                 // +0x30
};
class CTCFireballSpell : public CTCBase { public:          // interface type 0x9D
    CWillButtonHelperTimer                              Timer;                      // +0x0C
    CDefPointer<const CSpecialAbilitiesFireballSpellDef> PDef;                      // +0x40
    unsigned long                                       InitLoopingSound;           // +0x44
    CIntelligentPointer<CThing>                         FireballBuildUpEffect;      // +0x48
    bool                                                ImmediateCastMode;          // +0x50
    bool                                                OneAnimCastMode;            // +0x51
    CIntelligentPointer<CThing>                         PTarget;                    // +0x52
    CIntelligentPointer<CThing>                         PItemRemoved;               // +0x5A
    EWEAPON_STATE                                       WeaponState;                // +0x64
    bool                                                LeftHandAnim;               // +0x68
    CCharString                                         FireballAttachmentName;     // +0x6C
    bool                                                StartEffect;                // +0x70
    long                                                MaxLevelAllowableByStamina; // +0x74
    long                                                ReleaseDelayFrameCount;     // +0x78
    bool                                                SpellFinished;              // +0x7C
    void CastFireball(CThing* target);
};

void CTCFireballSpell::CastFireball(CThing* target)
{
    long level = Timer.WillAccumulatedLevel;
    if (level < 0)
        level = 0;
    else if (level > MaxLevelAllowableByStamina)
        level = MaxLevelAllowableByStamina;
    CTCHeroStats* heroStats;
    if (ParentThing->GetTC(&heroStats))
        heroStats->ModifyStaminaBy((long)-PDef->StaminaCost[level]);
    if (IS_THING_ALIVE(FireballBuildUpEffect))
        FireballBuildUpEffect->Kill(true);
    CTCSoundPlayer* soundPlayer;
    if (ParentThing->GetTC(&soundPlayer)) {
        soundPlayer->StopAttachedSound(InitLoopingSound);
        switch (level) {
        case 0:
            soundPlayer->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_CAST_NO_POWERUP", true, false, false, -1);
            break;
        case 1:
            soundPlayer->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_CAST_LEVEL_1", true, false, false, -1);
            break;
        case 2:
            soundPlayer->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_CAST_LEVEL_2", true, false, false, -1);
            break;
        case 3:
            soundPlayer->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_CAST_LEVEL_3", true, false, false, -1);
            break;
        }
    }
    CThing* parent = ParentThing;
    C3DVector position;
    CRightHandedSet frame;
    parent->PTCGraphicAppearance->GetDummyObject(FireballAttachmentName, 0, 0, position, frame, 1.0f);
    long shotIndex = 0;
    switch (level) {
    case 0:
        shotIndex = parent->PeekDefinitionManager().GetDefGlobalIndexFromName("FIREBALL_1");
        break;
    case 1:
        shotIndex = parent->PeekDefinitionManager().GetDefGlobalIndexFromName("FIREBALL_2");
        break;
    case 2:
        shotIndex = parent->PeekDefinitionManager().GetDefGlobalIndexFromName("FIREBALL_3");
        break;
    case 3:
        shotIndex = parent->PeekDefinitionManager().GetDefGlobalIndexFromName("FIREBALL_4");
        break;
    }
    C3DVector side = parent->PhysicsTC->GetFacingVector();
    side.Rotate(C3DVector(0.0f, 0.0f, 1.0f), 0.25f);
    frame = parent->PhysicsTC->GetLastRHSet();
    frame.Rotate(C3DVector(side), PDef->Inclination / 360.0f);
    CThing* shot = NThingShot::CreateShot(shotIndex, parent, 0, target, 100.0f, 1.0f, 0.0f,
        CDefPointer<const CMeleeCombatAbilityDef>(), &position, &frame);
    if (shot && !shot->IsDead()) {
        shot->SetAsDrawable(false);
        shot->IsAffectedByBulletTimeOverride = false;
        NThingShot::CTCShotDefinitionBase* shotDefinition;
        if (shot->GetTC(&shotDefinition))
            shotDefinition->SetCanBeDeflected(false);
    }
}