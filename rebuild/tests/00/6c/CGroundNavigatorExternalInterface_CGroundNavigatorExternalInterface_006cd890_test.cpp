// Behaviour test for 006cd890: src/006cd890.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CGroundNavigatorExternalInterface::CGroundNavigatorExternalInterface(CThingCreatureBase* creature)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGroundNavigatorExternalInterface::CGroundNavigatorExternalInterface(CThingCreatureBase* creature) -- the ground navigator's view of a creature: register the creature's navigation obstacle record in the global obstacle list (once), then take the radius from the creature's CCreatureNavigationDef sub-def, padded by 0.1 for creatures of kind 2
// Leaf: ??0CGroundNavigatorExternalInterface
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CD890, 221 bytes, `ret 4`.
// Callees: the component-map LowerBound @ 0x0040F020 (kind 7), malloc @ 0x00BFEA0E (the 12-byte list node),
//   CDefClassBase::GetSubDef<CCreatureNavigationDef>(CDefPointer*) @ 0x006CD2F0 (on the creature's def, +0x70), the
//   def's virtual slot 1 (+4, Delete) when its count drops to zero. Float pool 0x0122EE58 = 0.1f.
// Globals: the obstacle list @ 0x013D2834 (STLport list<CNavigatorObstacles*>, its sentinel pointer).
// Vtable: CGroundNavigatorExternalInterface 0x0125EC9C (the base CNavigatorExternalInterface store is dead and removed).
// Layout: engine/CGroundNavigatorExternalInterface.h (Creature +4, Radius +8) flattens the vptr into a `void*`, so the
//   polymorphic class is modelled locally as in the landed copy constructor 0x006CE670 (same obstacle registration);
//   CCreatureNavigationDef::Radius is +0x30 (Ego_r.pdb +0x30).
// Shape: Radius cleared in the initialiser list (immediate float zero); the record pointer is the inline
//   GetPNavigatorObstacles (null without a kind-7 component) followed by the inlined find / push_back of the copy ctor;
//   the sub-def out pointer is zeroed after the def fetch, then copied into a holder whose destructor releases it with
//   no null test (`dec [ecx+4] ; jne`), so the def stays in ecx for the radius read; the radius stays on the FPU for
//   the conditional 0.1 pad.
#include <new>
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
extern "C" void* malloc(unsigned int size);        // 0x00BFEA0E

struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);   // 0x0040F020 (the key by address)
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};

struct CDefPointeeBase {
    virtual void Destroy();                        // slot 0
    virtual void Delete();                         // slot 1 (+0x04)
    long m_refCount;                               // +0x04
    void Release() { if (DecRefToZero(m_refCount)) Delete(); }
};
struct CCreatureNavigationDef : CDefPointeeBase {
    unsigned char _pad08[0x30 - 0x08];
    float         Radius;                          // +0x30
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {             // releases the fetched def without a null test
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        Object->Release();
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CCreatureNavigationDef>* out);   // 0x006CD2F0
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureNavigationDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThingCreatureBase {
    unsigned char  _pad00[0x10];
    unsigned char  m_navigatorKind;                // +0x10 (2 = gets the 0.1 radius pad)
    unsigned char  _pad11[0x0f];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x20];
    CDefClassBase* PDef;                           // +0x70
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool GetInterface(T** out, long kind)   // the out-parameter lookup: the component stays in eax
    {
        if (HasTC(kind)) {
            *out = (T*)m_tcs.Find(kind);
            return true;
        }
        return false;
    }
};
struct CNavigatorObstacles;
struct CTCCreatureNavigation_Retail {              // component kind 7
    unsigned char _pad[0x40];
    unsigned char m_obstacles[4];                  // +0x40 the obstacle record
};
struct _List_node {
    _List_node*          _M_next;
    _List_node*          _M_prev;
    CNavigatorObstacles* _M_data;
};
struct CObstaclesList {                            // STLport list<CNavigatorObstacles*>: the sentinel node pointer
    _List_node* _M_node;
};
extern CObstaclesList g_registeredObstacles;       // 0x013D2834 (the obstacle records of every live ground navigator)
struct CNavigatorExternalInterface {               // vtable 0x01241D74
    virtual void v00();
    virtual ~CNavigatorExternalInterface() {}
};
struct CGroundNavigatorExternalInterface : CNavigatorExternalInterface {   // vtable 0x0125EC9C
    CThingCreatureBase* Creature;                  // +0x04
    float               Radius;                    // +0x08
    __forceinline CNavigatorObstacles* GetPNavigatorObstacles()   // inline twin of 0x006CD7B0
    {
        CTCCreatureNavigation_Retail* navigation;
        if (Creature->GetInterface(&navigation, 7))
            return (CNavigatorObstacles*)navigation->m_obstacles;
        return 0;
    }
    virtual void v00();
    CGroundNavigatorExternalInterface(CThingCreatureBase* creature);
    virtual ~CGroundNavigatorExternalInterface();
};

CGroundNavigatorExternalInterface::CGroundNavigatorExternalInterface(CThingCreatureBase* creature)
    : Creature(creature), Radius(0.0f)
{
    CNavigatorObstacles* obstacles = GetPNavigatorObstacles();
    _List_node* sentinel = g_registeredObstacles._M_node;
    _List_node* it = sentinel->_M_next;
    while (it != sentinel && it->_M_data != obstacles)
        it = it->_M_next;
    if (it == sentinel) {
        _List_node* node = (_List_node*)malloc(sizeof(_List_node));
        new (&node->_M_data) CNavigatorObstacles*(obstacles);
        _List_node* prev = sentinel->_M_prev;
        node->_M_next = sentinel;
        node->_M_prev = prev;
        prev->_M_next = node;
        sentinel->_M_prev = node;
    }
    {
        CDefPointer<CCreatureNavigationDef> out;
        Creature->PDef->GetSubDef(&out);
        CDefHolder<CCreatureNavigationDef> def(out.Object);
        Radius = def->Radius;
        if (Creature->m_navigatorKind == 2)
            Radius += 0.1f;
    }
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_deleted = 0;
void* FindDef_CDefClassBase_GetSubDef_0() { return g_def; }
void CDefPointeeBase::Destroy() {}
void CDefPointeeBase::Delete() { ++g_deleted; }
STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind) { STCEntry_Retail* it = m_pFirst; while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it; return it; }
CObstaclesList g_registeredObstacles;
void CNavigatorExternalInterface::v00() {}
void CGroundNavigatorExternalInterface::v00() {}
CGroundNavigatorExternalInterface::~CGroundNavigatorExternalInterface() {}
static int count(_List_node* s) { int n = 0; for (_List_node* p = s->_M_next; p != s; p = p->_M_next) ++n; return n; }
int main()
{
    _List_node sentinel; sentinel._M_next = sentinel._M_prev = &sentinel; g_registeredObstacles._M_node = &sentinel;
    CCreatureNavigationDef def; def.m_refCount = 5; def.Radius = 1.5f; g_def = &def;
    CDefClassBase base; base.m_refCount = 1;
    CTCCreatureNavigation_Retail nav;
    STCEntry_Retail entries[2]; entries[0].m_kind = 7; entries[0].m_pTC = (CTCBase_Retail*)&nav; entries[1].m_kind = 9; entries[1].m_pTC = 0;
    static CThingCreatureBase c; memset(&c, 0, sizeof(c)); c.PDef = &base; c.m_tcs.m_pFirst = entries; c.m_tcs.m_pLast = entries + 2;
    c.m_tcMask[0] = 1UL << 7; c.m_navigatorKind = 2;
    {
        CGroundNavigatorExternalInterface g(&c);
        CHECK(g.Creature == &c); CHECK(g.Radius > 1.59f && g.Radius < 1.61f); CHECK(def.m_refCount == 4);
        CHECK(count(&sentinel) == 1); CHECK(sentinel._M_next->_M_data == (CNavigatorObstacles*)nav.m_obstacles);
    }
    {   // already registered: not added again; kind != 2: no pad
        c.m_navigatorKind = 1; CGroundNavigatorExternalInterface g(&c);
        CHECK(count(&sentinel) == 1); CHECK(g.Radius == 1.5f); CHECK(def.m_refCount == 3);
    }
    {   // no navigation component: a null obstacle record is registered
        c.m_tcMask[0] = 0; CGroundNavigatorExternalInterface g(&c);
        CHECK(count(&sentinel) == 2); CHECK(sentinel._M_prev->_M_data == 0);
    }
    def.m_refCount = 1; { CGroundNavigatorExternalInterface g(&c); CHECK(g_deleted == 1); CHECK(count(&sentinel) == 2); }
    if (fails == 0) printf("006cd890_TEST PASS\n"); else printf("006cd890_TEST FAIL\n");
    return fails;
}