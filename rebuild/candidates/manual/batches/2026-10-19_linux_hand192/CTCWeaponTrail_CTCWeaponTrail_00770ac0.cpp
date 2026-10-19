// CTCWeaponTrail::~CTCWeaponTrail() -- destructor: the projectile trail list, the helper-name pairs, the per-trail entry lists, the inactive-tick and active-flag vectors, the weapon-trail handle vector, the projectile trail handle, then the CTCBase base
// Leaf: ??1CTCWeaponTrail
// Retail: Fable.exe (TLC, VC7.1) @ 0x00770AC0, 231 bytes (+ int3 padding); called only from the deleting destructor 0x00770AA0
//   (vtable 0x0126A5B4 slot 0 = CTCWeaponTrail, the class whose ctor is 0x0076E620).
// Label correction: the catalog's "CEngineInternalAttachedPrimitiveInfo::~CEngineInternalAttachedPrimitiveInfo" is propagated;
//   the vtable that reaches this body is CTCWeaponTrail's.
// Callees: the ProjectileWeaponTrail list dtor 0x004DB5BB, the HelperNames vector dtor 0x00770090, the `__fastcall`
//   _Destroy_range of the Trail lists 0x00770350 (first ecx, last edx, the empty allocator tag by const reference in the high
//   byte of the `push ecx` frame word), free 0x00BFEA14, CBaseClass::~CBaseClass 0x0099A2E0 (the handle's root), ~CTCBase
//   0x00686830; each WeaponTrailHandles element is destroyed through its virtual dtor (slot 0 with 0).
// Layout (retail; Ego_r.pdb CTCWeaponTrail with its containers 12 bytes instead of 16/20): CTCBase 0x0c bytes, NoTrails +0x0c,
//   NoProjectileTrails +0x10, WeaponTrailGraphicIndex +0x14, ProjectileWeaponTrailHandle +0x18, WeaponTrailHandles +0x20,
//   TrailType +0x2c, ActiveTrail +0x30, InactiveTicks +0x3c, Trail +0x48, HelperNames +0x54, ProjectileWeaponTrail +0x60.
// Shape: CTCWeaponTrail is `__declspec(novtable)` in this TU (the dtor stores no vtable); members die in reverse order with the
//   STLport vector dtors inlined (the handle loop over a by-value [first, last) pair, then `if (start) free(start)`); the
//   projectile handle is the landed CEnginePrimitiveHandle model with a `__forceinline` dtor.
extern "C" void __cdecl free(void* block);             // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };

struct CBaseClass {                                    // vtable 0x01231710
    virtual ~CBaseClass();                             // 0x0099A2E0
};

struct CEnginePrimitiveHandle;
struct CEnginePersistentPrimitiveBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                                   // +0x04
};

template <class T> struct CIVCountedPointer {
    T* m_p;
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_p = 0;
        }
    }
    CIVCountedPointer& operator=(T* p) { Release(); m_p = p; return *this; }
    T* operator->() const { return m_p; }
    long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }
};

struct CEnginePrimitiveHandle : CBaseClass {           // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
    __forceinline virtual ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

struct SHandleVector {                                 // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    ~SHandleVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

template <class T> struct SPodVector {                 // STLport vector of a POD element (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~SPodVector()
    {
        if (_M_start)
            free(_M_start);
    }
};

struct CWeaponTrailEntryList {                         // list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> (12 bytes)
    void* _M_node[3];
};
void __fastcall _Destroy_range(CWeaponTrailEntryList* first, CWeaponTrailEntryList* last, const SScalarTag& tag);   // 0x00770350

struct STrailVector {                                  // vector<list<CWeaponTrailEntry>> (12 bytes)
    CWeaponTrailEntryList* _M_start;
    CWeaponTrailEntryList* _M_finish;
    CWeaponTrailEntryList* _M_end_of_storage;
    ~STrailVector()
    {
        _Destroy_range(_M_start, _M_finish, SScalarTag());
        if (_M_start)
            free(_M_start);
    }
};

struct SHelperNameVector {                             // vector<pair<CCharString, CCharString>> (12 bytes)
    void* _M_words[3];
    ~SHelperNameVector();                              // 0x00770090
};

struct SProjectileTrailList {                          // list<CWeaponTrailEntry> (retail 4 bytes: the node pointer)
    void* _M_node;
    ~SProjectileTrailList();                           // 0x004DB5BB
};

struct CTCBase {
    virtual ~CTCBase();                                // 0x00686830
    unsigned char _body[8];
};

struct __declspec(novtable) CTCWeaponTrail : CTCBase { // vtable 0x0126A5B4
    long                        NoTrails;              // +0x0c
    long                        NoProjectileTrails;    // +0x10
    long                        WeaponTrailGraphicIndex;   // +0x14
    CEnginePrimitiveHandle      ProjectileWeaponTrailHandle;   // +0x18
    SHandleVector               WeaponTrailHandles;    // +0x20
    long                        TrailType;             // +0x2c
    SPodVector<unsigned long>   ActiveTrail;           // +0x30 (vector<bool> words)
    SPodVector<long>            InactiveTicks;         // +0x3c
    STrailVector                Trail;                 // +0x48
    SHelperNameVector           HelperNames;           // +0x54
    SProjectileTrailList        ProjectileWeaponTrail; // +0x60
    long                        MinProjectileTrailTimer;   // +0x64
    bool                        WeaponTrailActive;     // +0x68
    ~CTCWeaponTrail();
};

CTCWeaponTrail::~CTCWeaponTrail()
{
}
