// CEngineInternalPrimitiveWeaponTrail::~CEngineInternalPrimitiveWeaponTrail() -- free the creation-time array, destroy Trail, then the inlined root destructor
// Retail 0x00BCDE40, 35 bytes (+13 int3). Called only from the deleting destructor 0x00BCDE20
// (CEngineInternalPrimitiveWeaponTrail vtable 0x012A6B24 slot 0); the manifest label
// "CTCInventoryItem::OnDie" is propagated.
// Members die in reverse order: TrailElementCreationTime (CArray<float>, retail +0x68) frees its
// block with `free` 0x00BFEA14 under a null test (`add esp,4`); Trail (CArray<CArray<C3DVector>>,
// +0x5C) runs its out-of-line destructor 0x00BCDAD0 (the landed CRetailVector_SBufferEntry12 shape);
// then the only vtable store is the root CIVCountedPointeeBase's (0x012354D0) from its inline empty
// destructor, and the function returns -- the derived class is `__declspec(novtable)` and nothing
// between it and the root has a destructor body.
// Retail offsets (Ego_r.pdb: Trail +0x5C, TrailElementCreationTime +0x6C; retail CArrays are 12 bytes).
// Leaf: ??1CEngineInternalPrimitiveWeaponTrail

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct C3DVector;
struct CTrailSegment {                             // CArray<C3DVector>
    C3DVector* m_pData;
    long m_count;
    long m_capacity;
};

struct CTrailArray {                               // CArray<CArray<C3DVector>>
    CTrailSegment* _M_start;
    CTrailSegment* _M_finish;
    CTrailSegment* _M_end_of_storage;
    ~CTrailArray();                                // 0x00BCDAD0
};

struct CFloatArray {                               // CArray<float>
    float* m_pData;
    long m_count;
    long m_capacity;
    ~CFloatArray() { if (m_pData) free(m_pData); }
};

struct CIVCountedPointeeBase {                     // vtable 0x012354D0
    virtual void v00();
    ~CIVCountedPointeeBase() { }
};

struct __declspec(novtable) CEngineInternalPrimitiveWeaponTrail : CIVCountedPointeeBase {
    unsigned char _pad_04[0x58];                   // primitive base members, TrailType +0x48 ... SkipPoints +0x58
    CTrailArray Trail;                             // +0x5C
    CFloatArray TrailElementCreationTime;          // +0x68
    float CurrentTime;                             // +0x74
    float FallOffTime;                             // +0x78
    virtual void v00();
    ~CEngineInternalPrimitiveWeaponTrail();
};

CEngineInternalPrimitiveWeaponTrail::~CEngineInternalPrimitiveWeaponTrail()
{
}