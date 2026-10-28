// AttachEffectToStaff(CThingCreatureBase&, const CCharString&) -- look the named effect up in the creature's CSpecialEffectsDef, create it at the creature, and attach its particle emitter to the "SHOT_CREATION_DUMMY" helper of the thing in the creature's primary carrying slot (the staff); returns the effect, or 0
// Leaf: ?AttachEffectToStaff@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C9A40, 342 bytes, `__fastcall` (creature in ecx, name in edx), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb
//   ?AttachEffectToStaff@?A0x13e2aa0e@@YIPAVCThing@@AAVCThingCreatureBase@@ABVCCharString@@@Z (anonymous namespace; donor
//   0x009010D0 between CCreatureAction_HobbeSpellcasterAim::FinishAction 0x00901050 = retail 0x008C99C0 and ::Initialise
//   0x00901260 = retail 0x008C9BA0, the same order as retail); callers 0x008C9BA0 ("CHARGE_SPELL") and the landed
//   CCreatureAction_HobbeSpellcasterFire::PerformAction 0x008C9C80 ("FIRE_EFFECT") pass the creature in ecx and the name in
//   edx. Literal 0x0126534C "SHOT_CREATION_DUMMY". The anonymous-namespace hash differs per build: the Leaf is the prefix.
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString(const CCharString&,
//   long&) const 0x00669D30, CVectorMap LowerBound 0x0040F020 (inline TC lookups 0x46 = CTCCarrying, 0x4B = CTCDParticleEmitter,
//   from their GetInterfaceType bodies), CTCCarrying::GetThingInPrimarySlot() const 0x006A1440, CThing::GetPos 0x004C73D0,
//   CTCDParticleEmitter::Create(long, const C3DVector&, bool) 0x006E0880 (static `__fastcall`), CCharString ctor/dtor
//   0x0099EBF0 / 0x0099EAE0, CTCDParticleEmitter::AttachToThing(CThing&, long, const CCharString&, long, float) 0x006E0BE0.
// Shape: CDefClassBase::GetSubDef<CSpecialEffectsDef> is compiled here as a same-TU noinline body (the landed 0x00669580
//   model, as 0x008C6CE0 does for <CTrollBattleDef>): only then does VC7.1 keep the def pointer in ebx and share the
//   constant 1 in bl for the two dead-bit tests, as retail. The def's PBaseDef is used in place (no counted copy: `mov ecx,[edi+70h]`); the carrying TC is a HasTC test inside
//   the `&&` chain and a pick-form GetTC; the emitter is the out-pointer GetInterface into an uninitialised local (retail
//   reads the stale key slot when the effect has no emitter); the helper name is a named local in its own block. The effect
//   index lives in a block that closes before the helper-name string is built (staff / effect_thing hoisted and zeroed), so
//   VC7.1 gives the string the dead index slot [esp+14h] as retail (2026-10-28 retry: the fix, RELOCATION_MATCH).
// Device: AttachEffectToStaff_Use is not a retail function; it only keeps the anonymous-namespace body emitted (GOTCHAS,
//   chunk 390 `_Use` caller).
class C3DVector;
class CTCBase;

class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    void* PStringData;
};

struct CRefCounted {                                         // the def's reference count at +4 (behind the vptr)
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Delete() const;                             // slot 1
};

class CDefPointeeBase : public CDefVirtual, public CRefCounted {
public:
    void Release() const
    {
        if (--m_count == 0)
            Delete();
    }
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};

class CDefClassBase;
template <class T> class CDefPointer;

template <> class CDefPointer<CDefClassBase> {               // CDefinitionManager::GetPDefFromGlobalIndex's result
public:
    CDefPointeeBase* m_p;
    CDefPointer() { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

template <class T> class CDefPointer {
public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }
    CDefPointer(const CDefPointer<CDefClassBase>& other) : m_p((T*)other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
    T* operator->() const { return m_p; }
};

class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};

class CDefClassBase : public CDefPointeeBase {
public:
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual const long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38), descriptive
    unsigned char _pad_0x08[0x1c - 8];
    CDefinitionManager* PDefManager;                         // +0x1C
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};

class CThingBaseDef : public CDefClassBase { };

class CSpecialEffectsDef : public CDefClassBase {
public:
    bool GetEffectFromString(const CCharString& name, long& effect) const;   // 0x00669D30
};

#pragma auto_inline(off)
template <>
bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>* out) const    // 0x00669580 (landed body)
{
    const long* pIndex;
    {
        CCharString name("CSpecialEffectsDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<const CSpecialEffectsDef> result(PDefManager->GetPDefFromGlobalIndex(*pIndex));
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

enum ETCInterfaceType { TC_INTERFACE_NONE = 0 };
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {        // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {                  // CThing::Components
public:
    typedef std::pair<K, V> value_type;
    value_type* LowerBound(const K& key);                    // 0x0040F020 (non-const; the landed body)
    value_type* end() { return this->_M_finish; }
};
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad_0x04[0x20 - 0x04];
    unsigned long UsingComponents[9];                        // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*> Components;       // +0x44
    unsigned char _pad_0x50[0x70 - 0x50];
    CDefPointer<const CThingBaseDef> PBaseDef;               // +0x70
    unsigned char _pad_0x74[0x91 - 0x74];
    unsigned char Dead : 1;                                  // +0x91 bit 0
    const C3DVector& GetPos() const;                         // 0x004C73D0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)type;
        CVectorMap<ETCInterfaceType, CTCBase*>* map = &Components;
        CVectorMap<ETCInterfaceType, CTCBase*>::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            CVectorMap<ETCInterfaceType, CTCBase*>* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*>::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing { };

inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!thing->Dead)
            return true;
    }
    return false;
}

class CTCCarrying {
public:
    CThing* GetThingInPrimarySlot() const;                   // 0x006A1440
};

class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long effect, const C3DVector& pos, bool flag);   // 0x006E0880
    void AttachToThing(CThing& thing, long attach_type, const CCharString& helper_name, long index, float offset);   // 0x006E0BE0
};

#pragma auto_inline(off)
namespace {
CThing* __fastcall AttachEffectToStaff(CThingCreatureBase& creature, const CCharString& effect_name)
{
    CDefPointer<const CSpecialEffectsDef> effects_def;
    CThing* staff = 0;
    CThing* effect_thing = 0;
    {
        long effect = 0;
        if (creature.PBaseDef->GetSubDef(&effects_def) && effects_def->GetEffectFromString(effect_name, effect)
            && creature.HasTC(0x46)) {
            staff = creature.GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
            if (IS_THING_ALIVE(staff))
                effect_thing = CTCDParticleEmitter::Create(effect, creature.GetPos(), false);
        }
    }
    if (IS_THING_ALIVE(effect_thing)) {
        CTCDParticleEmitter* emitter;
        effect_thing->GetInterface(&emitter, 0x4B);
        {
            CCharString helper_name("SHOT_CREATION_DUMMY");
            emitter->AttachToThing(*staff, 10, helper_name, 0, 0.0f);
        }
        return effect_thing;
    }
    return 0;
}
}
#pragma auto_inline(on)

CThing* __fastcall AttachEffectToStaff_Use(CThingCreatureBase& creature, const CCharString& effect_name)
{
    return AttachEffectToStaff(creature, effect_name);
}