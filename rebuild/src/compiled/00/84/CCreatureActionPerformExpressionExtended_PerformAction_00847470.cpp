// CCreatureAction_PerformExpressionExtended::PerformAction() -- tell the target's CTCEntityEvents that the creature performed its expression at it (a new CEventExpressionPerformedTo lasting until frame-running + FrameCount)
// Leaf: ?PerformAction@CCreatureAction_PerformExpressionExtended@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00847470, 169 bytes, thiscall virtual, plain `ret`.
// Identity (proven): RTTI vtable 0x012744FC (.?AVCCreatureAction_PerformExpressionExtended@@) slot 3 = PerformAction; Ego_r.pdb /
//   FableWin.pdb ?PerformAction@CCreatureAction_PerformExpressionExtended@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020
//   (inlined CThing::GetTC, 0x42 = CTCEntityEvents), operator new 0x00BFEA1A, CCreatureActionBase::GetFramesRunning() const
//   0x006925C0 (returns FramesRunning +0x1C; between GetTotalTimeWorldFrames 0x006925B0 and GetFramesRemaining 0x006925D0 as in
//   both PDBs), NEntityEvents::CEventExpressionPerformedTo::CEventExpressionPerformedTo(CThingCreatureBase*, long,
//   const CDefPointer<const CExpressionDef>&, long, long) 0x0084B010, CTCEntityEvents::AddEvent(NEntityEvents::CEventBase*) 0x008113C0.
// Layout: engine/CCreatureAction_PerformExpressionExtended.h (retail): PExpressionDef +0xA8, PPerformToThing +0xAC, FrameCount +0xBC.
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
void* __cdecl operator new(unsigned int size);                   // 0x00BFEA1A
class CExpressionDef : public CDefClassBase { };
namespace NEntityEvents {
class CEventBase {
public:
    virtual ~CEventBase();
};
class CEventExpressionPerformedTo : public CEventBase {
public:
    CEventExpressionPerformedTo(CThingCreatureBase* performedBy, long lifeTime,
                                const CDefPointer<const CExpressionDef>& expression, long extraData1, long extraData2);   // 0x0084B010
    unsigned char _pad04[0x20 - 0x04];
};
}   // namespace NEntityEvents
class CTCEntityEvents {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x42; }
    void AddEvent(NEntityEvents::CEventBase* event);             // 0x008113C0
};
class CCreatureAction_PerformExpressionExtended : public CCreatureActionBase {
public:
    virtual void PerformAction();
    CDefPointer<const CExpressionDef> PExpressionDef;            // +0xA8
    CIntelligentPointer<CThing>       PPerformToThing;           // +0xAC
    long                              ExpressionItemDefIndex;    // +0xB4
    bool                              EmoteIconAdded;            // +0xB8
    long                              FrameCount;                // +0xBC
    bool                              ReplacedAction;            // +0xC0
    unsigned long                     SoundEffectID;             // +0xC4
};

void CCreatureAction_PerformExpressionExtended::PerformAction()
{
    CTCEntityEvents* events;
    if (PPerformToThing.Get() && PPerformToThing->GetTC(&events))
        events->AddEvent(new NEntityEvents::CEventExpressionPerformedTo(PCreature.Get(), GetFramesRunning() + FrameCount,
                                                                        PExpressionDef, -1, -1));
}