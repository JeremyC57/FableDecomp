// CCreatureAction_GeneralInterruptedAttack::Initialise() -- rumble the player's joystick (type 3), run the combat-action Initialise, then kick up a dust effect behind the creature (opposite its facing)
// Leaf: ?Initialise@CCreatureAction_GeneralInterruptedAttack@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087CA30, 210 bytes, thiscall virtual (slot 12), plain `ret`.
// Identity (proven): RTTI vtable of .?AVCCreatureAction_GeneralInterruptedAttack@@ slot 12 = Initialise; Ego_r.pdb / FableWin.pdb
//   ?Initialise@CCreatureAction_GeneralInterruptedAttack@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (PCreature +0x08), CThingCreatureBase::IsPlayerCharacter() const
//   (virtual slot 77, +0x134), CThing::GetJoystickDeviceNumber() const 0x004C7CA0, CJoystick::AddRumble(long, const CJoystick::CInitRumble&, bool)
//   (virtual slot 17, +0x44; CJoystickDX 0x006BDF30 in Ego_r), CCombatActionBase::Initialise() 0x00857520,
//   CTCPhysicsBase::GetFacingVector() const (virtual slot 70, +0x118), CCreatureActionBase::PeekDefinitionManager() const 0x00692910,
//   CCreatureActionBase::GetWorldMap() 0x00692C10, NCombatActionHelpers::CreateDustEffectForCreature 0x008F7950 (__fastcall).
// Globals: g_pInputManager 0x013B8388 (CInputManager*; PJoysticks, a vector<CCountedPointer<CJoystick> >, at retail +0x2C, Ego_r +52).
// Layout: CJoystick::CInitRumble {MaxLevel, Smoothness, AttackTime, DecayTime, Cumulative} (20 bytes, Ego_r) built inline from
//   ??0CInitRumble@CJoystick@@QAE@MMMM_N@Z: (1.0, 0.5, 0.0, 0.2, false).
// Shape: the facing goes through the inline CThing::GetPhysicsTC() accessor (reading +0x60 directly allocates the vtable load into
//   the wrong registers) into a named negated local; the rumble parameters are an inline-constructed CInitRumble temporary.
// ---- Shared model (retail layouts; names from Ego_r.pdb / FableWin.pdb) ----
class CBaseObject;
class CBaseObjectPointer;
class CTCBase;
class CTCPhysicsBase;
#pragma pack(push, 1)   // C3DVector has alignment 1: Ego_r puts CCreatureAction_ProjectileCrouch::FacePos at +301, size 316
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
};
#pragma pack(pop)
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class vector {              // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    _Tp& operator[](unsigned int n) { return *(this->_M_start + n); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                                     // +0x00
    TCompare           Compare;                                  // +0x0C
    bool               Dirty;                                    // +0x0D
    value_type* LowerBound(const K& key);                        // 0x0040F020
    const value_type* LowerBound(const K& key) const;            // 0x004365B0
};
// Same-TU bodies of the two retail LowerBound instances (STLport lower_bound over the sorted pairs), kept out of line:
// VC7.1 then knows the key's address does not escape and shares its frame word with the component out-pointer.
#pragma auto_inline(off)
template <class K, class V, class TCompare>
__declspec(noinline) const typename CVectorMap<K, V, TCompare>::value_type* CVectorMap<K, V, TCompare>::LowerBound(const K& key) const
{
    const value_type* it = Data._M_start;
    long count = Data._M_finish - Data._M_start;
    while (count > 0) {
        long half = count / 2;
        if (it[half].first < key) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
template <class K, class V, class TCompare>
__declspec(noinline) typename CVectorMap<K, V, TCompare>::value_type* CVectorMap<K, V, TCompare>::LowerBound(const K& key)
{
    value_type* it = Data._M_start;
    long count = Data._M_finish - Data._M_start;
    while (count > 0) {
        long half = count / 2;
        if (it[half].first < key) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

// Definition objects: CIVCountedPointeeBase (vptr, IVRefCount +0x04; slot 1 Delete) -> CResource (0x1C) -> CDefClassBase
// (PDefManager +0x1C). Virtual slots from FableWin.pdb in declaration order (the landed CCreatureAction_Drink::Initialise
// 0x00847AC0 calls slot 14 GetPSubDefInfo const).
class CCharString;
class CDefinitionManager;
class CDefString {
public:
    CDefString() : TablePos(0) { }
    CCharString GetString() const;                               // inline over g_DefStringTable (where used)
    long TablePos;                                               // +0x00
};
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();                            // 0
    virtual void Delete() const;                                 // 1
    void AddRef() const { ++IVRefCount; }
    long Release() const
    {
        long count = --IVRefCount;
        if (count == 0)
            Delete();
        return count;
    }
    mutable long IVRefCount;                                     // +0x04
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); }
    void Release()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
    T* operator->() const { return Object; }
    T& operator*() const { return *Object; }
};
class CResource : public CIVCountedPointeeBase {
public:
    virtual bool UnloadResource();                               // 2
    virtual unsigned long GetUnloadableMemory() const;           // 3
    virtual bool IsResourceLoaded() const;                       // 4
    virtual void OnResourceLoaded();                             // 5
    virtual bool CanAbortResourceLoad(long reason) const;        // 6
    virtual void OnResourceLoadAborted();                        // 7
    virtual unsigned long GetDebugMemoryInUse() const;           // 8
    virtual void ResetDebugMemoryInUse();                        // 9
    unsigned char _pad08[0x1C - 0x08];
};
class CDefClassBase : public CResource {
public:
    class CSubDefInfo {
    public:
        long GlobalIndex;
    };
    virtual CDefString GetInstantiationName() const;             // 10
    virtual void SetInstantiationName(CDefString name);          // 11
    virtual void AddSubDef(const CCharString& name, long globalIndex, long flags);   // 12
    virtual void RemoveSubDef(const CCharString& name);          // 13
    // const / non-const overloads take their slots in reverse declaration order: const is slot 14 (+0x38).
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString& name);                   // 15
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString& name) const;       // 14
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
    CDefinitionManager* PDefManager;                             // +0x1C
};
class CThingBaseDef : public CDefClassBase { };
class CThing {                                                   // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    // CThing vtable: slot 0 is the virtual destructor; slots 1-76 are placeholders (not called here, names not modelled);
    // CThingCreatureBase::IsPlayerCharacter is slot 77.
    virtual ~CThing();
    virtual void ThingVSlot01(); virtual void ThingVSlot02(); virtual void ThingVSlot03(); virtual void ThingVSlot04(); virtual void ThingVSlot05(); virtual void ThingVSlot06();
    virtual void ThingVSlot07(); virtual void ThingVSlot08(); virtual void ThingVSlot09(); virtual void ThingVSlot10(); virtual void ThingVSlot11(); virtual void ThingVSlot12();
    virtual void ThingVSlot13(); virtual void ThingVSlot14(); virtual void ThingVSlot15(); virtual void ThingVSlot16(); virtual void ThingVSlot17(); virtual void ThingVSlot18();
    virtual void ThingVSlot19(); virtual void ThingVSlot20(); virtual void ThingVSlot21(); virtual void ThingVSlot22(); virtual void ThingVSlot23(); virtual void ThingVSlot24();
    virtual void ThingVSlot25(); virtual void ThingVSlot26(); virtual void ThingVSlot27(); virtual void ThingVSlot28(); virtual void ThingVSlot29(); virtual void ThingVSlot30();
    virtual void ThingVSlot31(); virtual void ThingVSlot32(); virtual void ThingVSlot33(); virtual void ThingVSlot34(); virtual void ThingVSlot35(); virtual void ThingVSlot36();
    virtual void ThingVSlot37(); virtual void ThingVSlot38(); virtual void ThingVSlot39(); virtual void ThingVSlot40(); virtual void ThingVSlot41(); virtual void ThingVSlot42();
    virtual void ThingVSlot43(); virtual void ThingVSlot44(); virtual void ThingVSlot45(); virtual void ThingVSlot46(); virtual void ThingVSlot47(); virtual void ThingVSlot48();
    virtual void ThingVSlot49(); virtual void ThingVSlot50(); virtual void ThingVSlot51(); virtual void ThingVSlot52(); virtual void ThingVSlot53(); virtual void ThingVSlot54();
    virtual void ThingVSlot55(); virtual void ThingVSlot56(); virtual void ThingVSlot57(); virtual void ThingVSlot58(); virtual void ThingVSlot59(); virtual void ThingVSlot60();
    virtual void ThingVSlot61(); virtual void ThingVSlot62(); virtual void ThingVSlot63(); virtual void ThingVSlot64(); virtual void ThingVSlot65(); virtual void ThingVSlot66();
    virtual void ThingVSlot67(); virtual void ThingVSlot68(); virtual void ThingVSlot69(); virtual void ThingVSlot70(); virtual void ThingVSlot71(); virtual void ThingVSlot72();
    virtual void ThingVSlot73(); virtual void ThingVSlot74(); virtual void ThingVSlot75(); virtual void ThingVSlot76();
    unsigned char      _pad04[0x20 - 0x04];
    unsigned long      UsingComponents[9];                       // +0x20 std::bitset<274>
    TCMap              Components;                               // +0x44
    unsigned char      UpdateComponentsList[4];                  // +0x54 std::list<CTCBase*>
    unsigned char      SerialisationSectionName[4];              // +0x58 CCharString
    void*              PTCDriver;                                // +0x5C CTCDriverBase*
    CTCPhysicsBase*    PhysicsTC;                                // +0x60
    void*              PTCGraphicAppearance;                     // +0x64
    long               WorldFrameCreatedAt;                      // +0x68
    unsigned long      ThingTypeFlags;                           // +0x6C
    CDefPointer<const CThingBaseDef> PBaseDef;                   // +0x70
    unsigned char      _pad74[0x90 - 0x74];
    signed char        PlayerNumber;                             // +0x90
    unsigned char      Dead : 1;                                 // +0x91 bit 0
    bool IsDead() const { return Dead; }
    const CThingBaseDef& PeekBaseDef() const { return *PBaseDef; }
    unsigned long GetPlayerNumber() const { return PlayerNumber; }
    const C3DVector& GetPos() const;                             // 0x004C73D0
    long GetJoystickDeviceNumber() const;                        // 0x004C7CA0
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }         // inline (Ego_r ?GetPhysicsTC@CThing@@QAEPAVCTCPhysicsBase@@XZ)
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::StatGetInterfaceType()))
            return false;
        ETCInterfaceType key = T::StatGetInterfaceType();
        TCMap* map = &Components;
        TCMap::value_type* it = map->LowerBound(key);
        if (it == map->Data.end() || T::StatGetInterfaceType() < it->first)
            it = map->Data.end();
        *out = (T*)it->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (!HasTC(T::StatGetInterfaceType()))
            return false;
        ETCInterfaceType key = T::StatGetInterfaceType();
        const TCMap* map = &Components;
        const TCMap::value_type* it = map->LowerBound(key);
        if (it == map->Data.end() || T::StatGetInterfaceType() < it->first)
            it = map->Data.end();
        *out = (const T*)it->second;
        return true;
    }
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CCreatureActionBase;
class CGameDefinitionManager;
class CWorldMap;
class CThingCreatureBase : public CThingGameObject {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);    // 0x006644F0
    virtual bool IsPlayerCharacter() const = 0;                  // slot 77 (+0x134): CThingPlayerCreature 0x0052ABE0 true, CThingAICreature 0x00835940 false
};

class CBaseIntelligentPointer {                                  // vptr + PData (8 bytes)
public:
    CBaseIntelligentPointer();                                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                          // 0x00A01C10
    CBaseObject* GetPItem() const;                               // 0x00A01B50
    void SetPItem(CBaseObject* item);                            // 0x00A01B90
    CBaseObjectPointer* PData;                                   // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    explicit CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    virtual ~CIntelligentPointer();
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};

class CCreatureActionBase {                                      // retail sizeof 0xA8 (Ego_r 0xB4: three STL containers)
public:
    virtual ~CCreatureActionBase();
    unsigned char                           _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;           // +0x08
    long             TotalTime;                                  // +0x10
    long             ActionTime;                                 // +0x14
    long             AnimDelayTime;                              // +0x18
    long             FramesRunning;                              // +0x1C
    long             InterruptionGroupID;                        // +0x20
    long             InterruptionPriority;                       // +0x24
    long             ActionID;                                   // +0x28
    CIntelligentPointer<CThing> PItemRemoved;                    // +0x2C
    unsigned char    _pad34[0x60 - 0x34];
    bool             AddedActionAnim;                            // +0x60
    bool             Finished;                                   // +0x61
    bool             PerformedAction;                            // +0x62
    bool             Initialised;                                // +0x63
    unsigned char    _pad64[0xA8 - 0x64];
    bool HasPerformedAction() const;                             // 0x006925A0
    long GetTotalTimeWorldFrames() const;                        // 0x006925B0
    long GetFramesRunning() const;                               // 0x006925C0
    long GetFramesRemaining() const;                             // 0x006925D0
protected:
    const CGameDefinitionManager& PeekDefinitionManager() const; // 0x00692910
    CWorldMap& GetWorldMap();                                    // 0x00692C10 (folded with the const overload)
public:
};
// ---- end shared model ----
class CCharStringData;
class CCharString {
public:
    CCharString(const char* text, long length = -1);             // 0x0099EBF0
    ~CCharString();                                              // 0x0099EAE0
    CCharStringData* PStringData;
};
// CTCBase (retail vtable 0x0125BE0C, 28 slots) and CTCPhysicsBase (vtable 0x0125D2C4): slot names from FableWin.pdb /
// Ego_r.pdb, aligned on the retail stub bodies (`ret 8` OnSerialise, pure 21/22, the `ret 4` / `mov al,1` / `xor al,al`
// defaults); the editor-only CTCBase virtuals (EditFrameUpdate, EditorDrawPreparePrimitivesForRendering,
// Get/SetFromPropertiesStruct) are not in the retail table. Only the slots the bodies below call are needed.
class CCharString;
class C2DVector;
class CRightHandedSet;
class CPersistContext;
class CTCParamsBase;
class CEnginePrimitivePrepareInfo;
namespace NTCMessages { class CMessageBase; }
class CTCBase {
public:
    virtual ~CTCBase();                                                          // 0
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
    CThing*       PThing;                                                        // +0x04
    unsigned char _pad08[4];
};
class CTCPhysicsBase : public CTCBase {
protected:
    virtual C2DVector GetOffsetVector() const;                                   // 28
    virtual C3DVector GetOffsetVector3D() const;                                 // 29
public:
    virtual void SetPosition(const C3DVector& position);                         // 30
    virtual void ResetPosition(const C3DVector& position);                       // 31
    virtual void ResetRHSet(const CRightHandedSet& set);                         // 32
    virtual void SetPositionToNavigateTo(const C3DVector& position);             // 33
    virtual void SetForcedToMovePos(const C3DVector& position, bool forced);     // 34
    virtual void ResetMovementState();                                           // 35
    virtual void EnablePhysics(bool enable);                                     // 36
    // slots 37-68: placeholders (not called here, names not modelled)
    virtual void PhysicsVSlot37(); virtual void PhysicsVSlot38(); virtual void PhysicsVSlot39(); virtual void PhysicsVSlot40(); virtual void PhysicsVSlot41(); virtual void PhysicsVSlot42();
    virtual void PhysicsVSlot43(); virtual void PhysicsVSlot44(); virtual void PhysicsVSlot45(); virtual void PhysicsVSlot46(); virtual void PhysicsVSlot47(); virtual void PhysicsVSlot48();
    virtual void PhysicsVSlot49(); virtual void PhysicsVSlot50(); virtual void PhysicsVSlot51(); virtual void PhysicsVSlot52(); virtual void PhysicsVSlot53(); virtual void PhysicsVSlot54();
    virtual void PhysicsVSlot55(); virtual void PhysicsVSlot56(); virtual void PhysicsVSlot57(); virtual void PhysicsVSlot58(); virtual void PhysicsVSlot59(); virtual void PhysicsVSlot60();
    virtual void PhysicsVSlot61(); virtual void PhysicsVSlot62(); virtual void PhysicsVSlot63(); virtual void PhysicsVSlot64(); virtual void PhysicsVSlot65(); virtual void PhysicsVSlot66();
    virtual void PhysicsVSlot67(); virtual void PhysicsVSlot68();
    // the two overloads share one slot group in reverse declaration order: () -> 70 (0x006B1410), (float) -> 69 (0x006B1430);
    // Ego_r.pdb emits them as 0x006EE000 GetFacingVector() then 0x006EE020 GetFacingVector(float)
    virtual C3DVector GetFacingVector() const;                                   // 70 (+0x118)
    virtual C3DVector GetFacingVector(float time) const;                         // 69 (+0x114)
    C3DVector     Position;                                                      // +0x0C
};
class CSoundPair {
public:
    CSoundPair() : FirstSound(0), LastSound(0) { }
    long FirstSound;                                             // +0x00
    long LastSound;                                              // +0x04
};
class CMeleeCombatAbilityDef : public CDefClassBase { };
class CCombatActionBase : public CCreatureActionBase {           // retail sizeof 0x114 (Ego_r 0x12C)
public:
    CCombatActionBase(CThingCreatureBase& creature, CThing& target, long priority, const CSoundPair& sounds);   // 0x00857F30
    CCombatActionBase(CThingCreatureBase& creature, long priority, const CSoundPair& sounds);                  // 0x00858030
protected:
    static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase& creature, const CCharString& name);    // 0x008561A0
public:
    virtual void Initialise();                                   // slot 12, 0x00857520
protected:
    virtual void FinishAction();                                 // slot 4, 0x00859A20
    void SetPMeleeAbility(CDefPointer<const CMeleeCombatAbilityDef> def) { PMeleeAbilityDef = def; }
public:
    void SetCombatAnimation(const CCharString& animName);        // 0x008574D0
    CIntelligentPointer<CThing>              PTarget;            // +0xA8
    CIntelligentPointer<const CThing>        PCameraPoint;       // +0xB0
    CDefPointer<const CMeleeCombatAbilityDef> PMeleeAbilityDef;  // +0xB8
    unsigned char _padBC[0x114 - 0xBC];
};
template <class T> class CCountedPointer {
public:
    T*    Data;                                                  // +0x00
    void* Info;                                                  // +0x04 CCPPointerInfo*
    T* operator->() const { return Data; }
};
class CJoystick {
public:
    class CInitRumble {
    public:
        CInitRumble(float maxLevel, float smoothness, float attackTime, float decayTime, bool cumulative)
            : MaxLevel(maxLevel), Smoothness(smoothness), AttackTime(attackTime), DecayTime(decayTime), Cumulative(cumulative) { }
        float MaxLevel;                                          // +0x00
        float Smoothness;                                        // +0x04
        float AttackTime;                                        // +0x08
        float DecayTime;                                         // +0x0C
        bool  Cumulative;                                        // +0x10
    };
    // slots 0-16: placeholders (not called here, names not modelled)
    virtual void JoystickVSlot00(); virtual void JoystickVSlot01(); virtual void JoystickVSlot02(); virtual void JoystickVSlot03();
    virtual void JoystickVSlot04(); virtual void JoystickVSlot05(); virtual void JoystickVSlot06(); virtual void JoystickVSlot07();
    virtual void JoystickVSlot08(); virtual void JoystickVSlot09(); virtual void JoystickVSlot10(); virtual void JoystickVSlot11();
    virtual void JoystickVSlot12(); virtual void JoystickVSlot13(); virtual void JoystickVSlot14(); virtual void JoystickVSlot15();
    virtual void JoystickVSlot16();
    virtual void AddRumble(long type, const CInitRumble& rumble, bool replace) = 0;   // slot 17 (+0x44)
};
class CInputManager {
public:
    unsigned char _pad00[0x2C];
    std::vector<CCountedPointer<CJoystick>, std::allocator<CCountedPointer<CJoystick> > > PJoysticks;   // +0x2C
};
extern CInputManager* g_pInputManager;                           // 0x013B8388
namespace NCombatActionHelpers {
void __fastcall CreateDustEffectForCreature(CThingCreatureBase& creature, const CWorldMap& worldMap, const CGameDefinitionManager& definitionManager,
    const C3DVector& direction, std::vector<CThing*, std::allocator<CThing*> >* hitThings);   // 0x008F7950
}
class CCreatureAction_GeneralInterruptedAttack : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCreatureAction_GeneralInterruptedAttack::Initialise()
{
    if (PCreature->IsPlayerCharacter())
        g_pInputManager->PJoysticks[PCreature->GetJoystickDeviceNumber()]->AddRumble(3, CJoystick::CInitRumble(1.0f, 0.5f, 0.0f, 0.2f, false), true);
    CCombatActionBase::Initialise();
    C3DVector direction = -PCreature->GetPhysicsTC()->GetFacingVector();
    NCombatActionHelpers::CreateDustEffectForCreature(*PCreature, GetWorldMap(), PeekDefinitionManager(), direction, 0);
}