// CTCWeaponTrail::ClearAllWeaponTrails() -- releases the projectile trail handle and every weapon trail handle, empties the projectile trail and every weapon trail point list, and resets the active flags and inactive tick counters
// Leaf: ClearAllWeaponTrails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076E150, 267 bytes (plain `ret`).
// Callees: the primitive's OnLastHandleReleased (slot 3, +0x0C) and Destroy (slot 1) through the inline
//          CEnginePrimitiveHandle::Clear() (the landed CTCTavernGame::ClearBox 0x005EC5C0 model); free 0x00BFEA14 (list nodes).
// Layout (retail, 12-byte vectors, one-pointer lists; Ego_r.pdb names): ProjectileWeaponTrailHandle +0x18,
//          WeaponTrailHandles (8-byte handles) +0x20, TrailType +0x2c, ActiveTrail (vector<bool>, one byte per flag in
//          this STLport build) +0x30, InactiveTicks (vector<long>) +0x3c, Trail (vector<list<CWeaponTrailEntry> >) +0x48,
//          HelperNames +0x54, ProjectileWeaponTrail (list) +0x60.
// Shape: the projectile handle's clear is spelled out in this function (GetReferenceCount() == 1 -> OnLastReleased(),
//        then Release()) while the handle loop calls the inline Clear() that nests the same two members: the inline depth
//        decides the register (spelled out: primitive in eax, `mov ecx,eax` for the virtual call; nested: a direct ecx
//        load), and the 2-byte longer head gives retail's `mov edi,edi` loop filler. The zero lives in ebx; the inline list clear() re-reads the list's node pointer after every free; the two fills
//        re-read their end pointers after every store (the stores may alias the vector); the handle loop and the
//        list-vector loop re-read end() each pass.
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

class CEnginePrimitiveHandle;
class CEnginePrimitive { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0xc)
    long RefCount;                              // +0x04
};
class CEnginePrimitiveHandle { public:
    virtual ~CEnginePrimitiveHandle();
    CEnginePrimitive* P;                        // +0x04
    unsigned long GetReferenceCount() const { return P ? P->RefCount : 0; }
    void OnLastReleased() { P->OnLastHandleReleased(this); }
    void Release()
    {
        if (CEnginePrimitive* p = P) {
            if (--p->RefCount == 0)
                p->Destroy();
            P = 0;
        }
        P = 0;
    }
    void Clear()
    {
        if (GetReferenceCount() == 1)
            OnLastReleased();
        Release();
    }
};
struct CHandleVector {                          // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
};
struct CWeaponTrailEntry { float Data[8]; };
struct STrailNode {
    STrailNode*       _M_next;
    STrailNode*       _M_prev;
    CWeaponTrailEntry _M_data;
};
struct CTrailList {                             // STLport list<CWeaponTrailEntry> (one node pointer)
    STrailNode* _M_node;
    void clear()
    {
        STrailNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            STrailNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
template<class T> struct CRetailVector {        // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};

class CTCWeaponTrail { public:
    virtual void v00();
    void*         ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[4];
    long          NoTrails;                     // +0x0c
    long          NoProjectileTrails;           // +0x10
    long          WeaponTrailGraphicIndex;      // +0x14
    CEnginePrimitiveHandle ProjectileWeaponTrailHandle;   // +0x18
    CHandleVector WeaponTrailHandles;           // +0x20
    long          TrailType;                    // +0x2c
    CRetailVector<bool> ActiveTrail;            // +0x30
    CRetailVector<long> InactiveTicks;          // +0x3c
    CRetailVector<CTrailList> Trail;            // +0x48
    CRetailVector<void*> HelperNames;           // +0x54 (vector<pair<CCharString, CCharString> >)
    CTrailList    ProjectileWeaponTrail;        // +0x60
    void ClearAllWeaponTrails();
};

void CTCWeaponTrail::ClearAllWeaponTrails()
{
    if (ProjectileWeaponTrailHandle.GetReferenceCount() == 1)
        ProjectileWeaponTrailHandle.OnLastReleased();
    ProjectileWeaponTrailHandle.Release();
    for (CEnginePrimitiveHandle* h = WeaponTrailHandles._M_start; h != WeaponTrailHandles._M_finish; ++h)
        h->Clear();
    ProjectileWeaponTrail.clear();
    for (bool* active = ActiveTrail._M_start; active != ActiveTrail._M_finish; ++active)
        *active = false;
    for (long* ticks = InactiveTicks._M_start; ticks != InactiveTicks._M_finish; ++ticks)
        *ticks = 0;
    for (CTrailList* trail = Trail._M_start; trail != Trail._M_finish; ++trail)
        trail->clear();
}