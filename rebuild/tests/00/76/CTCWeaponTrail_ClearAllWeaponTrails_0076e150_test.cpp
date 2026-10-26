// Behaviour test for 0076e150: src/0076e150.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCWeaponTrail::ClearAllWeaponTrails()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

// ---- stubs and driver ----
#include <cstdlib>
#include <new>
static int g_destroyed[8]; static int g_lastReleased[8]; static const void* g_lastHandle[8];
struct CTestPrimitive : CEnginePrimitive { int Id; };
void CEnginePrimitive::v00() { }
void CEnginePrimitive::Destroy() { ++g_destroyed[((CTestPrimitive*)this)->Id]; }
void CEnginePrimitive::v02() { }
void CEnginePrimitive::OnLastHandleReleased(CEnginePrimitiveHandle* handle)
{
    int id = ((CTestPrimitive*)this)->Id; ++g_lastReleased[id]; g_lastHandle[id] = handle;
}
CEnginePrimitiveHandle::~CEnginePrimitiveHandle() { }
void CTCWeaponTrail::v00() { }
static STrailNode* MakeList(int count)
{
    STrailNode* head = (STrailNode*)malloc(sizeof(STrailNode));
    head->_M_next = head; head->_M_prev = head;
    for (int i = 0; i < count; ++i) {
        STrailNode* n = (STrailNode*)malloc(sizeof(STrailNode));
        n->_M_next = head; n->_M_prev = head->_M_prev; head->_M_prev->_M_next = n; head->_M_prev = n;
    }
    return head;
}
int main()
{
    CTestPrimitive prims[4];
    for (int i = 0; i < 4; ++i) { memset(&prims[i], 0, sizeof(prims[i])); new (&prims[i]) CTestPrimitive(); prims[i].Id = i; }
    prims[0].RefCount = 1;   // projectile: last handle
    prims[1].RefCount = 2;   // shared: just a decrement
    prims[2].RefCount = 1;   // last handle in the vector
    CTCWeaponTrail* t = (CTCWeaponTrail*)calloc(1, sizeof(CTCWeaponTrail));
    t->ProjectileWeaponTrailHandle.P = &prims[0];
    CEnginePrimitiveHandle* handles = (CEnginePrimitiveHandle*)calloc(3, sizeof(CEnginePrimitiveHandle));
    handles[0].P = &prims[1]; handles[1].P = 0; handles[2].P = &prims[2];
    t->WeaponTrailHandles._M_start = handles; t->WeaponTrailHandles._M_finish = handles + 3; t->WeaponTrailHandles._M_end_of_storage = handles + 3;
    bool active[3] = { true, true, true };
    t->ActiveTrail._M_start = active; t->ActiveTrail._M_finish = active + 3; t->ActiveTrail._M_end_of_storage = active + 3;
    long ticks[2] = { 7, 9 };
    t->InactiveTicks._M_start = ticks; t->InactiveTicks._M_finish = ticks + 2; t->InactiveTicks._M_end_of_storage = ticks + 2;
    CTrailList trails[2]; trails[0]._M_node = MakeList(3); trails[1]._M_node = MakeList(0);
    t->Trail._M_start = trails; t->Trail._M_finish = trails + 2; t->Trail._M_end_of_storage = trails + 2;
    t->ProjectileWeaponTrail._M_node = MakeList(2);
    t->ClearAllWeaponTrails();
    CHECK(g_lastReleased[0] == 1 && g_lastHandle[0] == &t->ProjectileWeaponTrailHandle);
    CHECK(prims[0].RefCount == 0 && g_destroyed[0] == 1 && t->ProjectileWeaponTrailHandle.P == 0);
    CHECK(g_lastReleased[1] == 0 && prims[1].RefCount == 1 && g_destroyed[1] == 0 && handles[0].P == 0);
    CHECK(handles[1].P == 0);
    CHECK(g_lastReleased[2] == 1 && g_lastHandle[2] == &handles[2] && prims[2].RefCount == 0 && g_destroyed[2] == 1 && handles[2].P == 0);
    CHECK(g_lastReleased[3] == 0 && g_destroyed[3] == 0);
    CHECK(!active[0] && !active[1] && !active[2]);
    CHECK(ticks[0] == 0 && ticks[1] == 0);
    STrailNode* h = t->ProjectileWeaponTrail._M_node;
    CHECK(h->_M_next == h && h->_M_prev == h);
    CHECK(trails[0]._M_node->_M_next == trails[0]._M_node && trails[0]._M_node->_M_prev == trails[0]._M_node);
    CHECK(trails[1]._M_node->_M_next == trails[1]._M_node && trails[1]._M_node->_M_prev == trails[1]._M_node);
    // a second clear on the emptied state touches nothing
    t->ClearAllWeaponTrails();
    CHECK(g_destroyed[0] == 1 && g_destroyed[2] == 1 && g_lastReleased[0] == 1 && g_lastReleased[2] == 1);
    if (fails == 0) printf("0076e150_TEST PASS\n");
    return fails;
}
