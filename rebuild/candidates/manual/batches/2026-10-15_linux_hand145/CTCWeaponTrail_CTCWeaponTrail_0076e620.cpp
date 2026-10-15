// CTCWeaponTrail::CTCWeaponTrail(CThing* parent) -- CTCBase(parent), the CTCWeaponTrail vtable, NoTrails = -1, an empty projectile-trail primitive handle, five empty vectors, the projectile trail list (sentinel allocated) and WeaponTrailActive = false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076E620, length 128 bytes.
// Leaf: ??0CTCWeaponTrail@@QAE@PAVCThing@@@Z
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseClass::CBaseClass() @ 0x0099A2D0 (the handle's base),
//          malloc @ 0x00BFEA0E (the list sentinel, 0x24-byte node).
// Label: the catalog's "CList::CList" is propagated -- the vtable stored, 0x0126A5B4, is CTCWeaponTrail
// (rtti-vtables.tsv). Ego_r.pdb layout shifted for the shipped containers (vector 12 bytes, vector<bool> 12,
// list 4): NoTrails +0x0c, NoProjectileTrails +0x10, WeaponTrailGraphicIndex +0x14, ProjectileWeaponTrailHandle
// +0x18, WeaponTrailHandles +0x20, TrailType +0x2c, ActiveTrail +0x30, InactiveTicks +0x3c, Trail +0x48,
// HelperNames +0x54, ProjectileWeaponTrail +0x60, MinProjectileTrailTimer +0x64, WeaponTrailActive +0x68.
// Shape: initialisers in declaration order sharing the `xor ebx,ebx` zero (after the handle's vtable store, as in
// the landed CTCEmoteIcon 0x0071AFE0); the list head is nulled, then the self-linked sentinel stored.
struct CThing;
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing* thing);                        // 0x00686800
};
struct CBaseClass {
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
};
struct CEnginePersistentPrimitiveBase;
struct CIVCountedPointer {
    CEnginePersistentPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) { }
};
struct CEnginePrimitiveHandle : CBaseClass {       // vtable 0x0122F598
    CIVCountedPointer Data;                        // +0x04
    virtual void v00();
    CEnginePrimitiveHandle() { }
};
template <class T> struct CRetailVector {          // the shipped vector: {first, last, end}
    T* m_first;
    T* m_last;
    T* m_end;
    CRetailVector() : m_first(0), m_last(0), m_end(0) { }
};
template <class T> struct CRetailList {            // the shipped list: one sentinel-node pointer
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};
struct CWeaponTrailEntry { unsigned char _body[0x1c]; };   // CEnginePrimitiveWeaponTrail::CWeaponTrailEntry
struct CCharStringPair { void* first; void* second; };
struct CTCWeaponTrail : CTCBase {                  // vtable 0x0126A5B4
    long                                        NoTrails;                    // +0x0c
    long                                        NoProjectileTrails;          // +0x10
    long                                        WeaponTrailGraphicIndex;     // +0x14
    CEnginePrimitiveHandle                      ProjectileWeaponTrailHandle; // +0x18
    CRetailVector<CEnginePrimitiveHandle>       WeaponTrailHandles;          // +0x20
    long                                        TrailType;                   // +0x2c EWeaponTrailType
    CRetailVector<unsigned long>                ActiveTrail;                 // +0x30 vector<bool>
    CRetailVector<long>                         InactiveTicks;               // +0x3c
    CRetailVector<CRetailList<CWeaponTrailEntry> > Trail;                    // +0x48
    CRetailVector<CCharStringPair>              HelperNames;                 // +0x54
    CRetailList<CWeaponTrailEntry>              ProjectileWeaponTrail;       // +0x60
    long                                        MinProjectileTrailTimer;     // +0x64
    bool                                        WeaponTrailActive;           // +0x68
    virtual void v00();
    CTCWeaponTrail(CThing* parent);
};

CTCWeaponTrail::CTCWeaponTrail(CThing* parent)
    : CTCBase(parent), NoTrails(-1), WeaponTrailActive(false)
{
}
