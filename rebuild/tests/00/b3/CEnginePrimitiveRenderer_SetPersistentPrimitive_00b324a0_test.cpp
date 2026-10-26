// behaviour test for 0x00B324A0 CEnginePrimitiveRenderer::SetPersistentPrimitive (body verbatim, callees stubbed)
#include <stdio.h>
// CEnginePrimitiveRenderer::SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags, bool nonPersistent) -- update the handle's internal primitive in place when it already has the right type, else release it, create a new internal primitive through the type's manager, initialise it and register it with the 2D renderer (flag 0x40) or its manager
// Grade: RELOCATION_MATCH -- the handle is the CEnginePrimitiveHandle model of the landed ??_G 0x0041C500: a counted pointer member with a null-safe `GetRefCount()`, Clear = `if (Data.GetRefCount() == 1) notify; Data = 0;` (operator= = release + a second zero store, retail's re-read and doubled store) and Set = Clear, the counted release again (the dead zero-register release), then the raw store of the new primitive
// Leaf: SetPersistentPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B324A0, length 368 bytes (with int3 padding; body 0x160), `ret 10h`.
// PDB: ?SetPersistentPrimitive@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K_N@Z.
// Callees: the internal primitive's virtual slots 1 (Destroy), 3 (OnLastHandleReleased) and 5 (+0x14, the update from a
//   CEnginePrimitive and flags); the type manager's slot 1 (+4, CreateInternalPrimitive(type)) on the type table
//   @ 0x01436E84; __fastcall CEngineScene::UpdatePosition(CEngineInternalPrimitiveBase&, unsigned long) @ 0x00B91840;
//   CEnginePrimitiveRenderer2D::AddPrimitive @ 0x00B4A670 (global @ 0x01436E7C); the manager registration @ 0x00BACDD0.
// Layout: CEnginePrimitive: Type +0; CEngineInternalPrimitiveBase: RefCount +4, Type +8, GridPos.Level +0x24, Enable
//   (bit 0) / NonPersistent (bit 1) of +0x34; CEnginePrimitiveHandle {vptr, Data}.
// Shape: the handle helpers as the counted-pointer model above; `Enable = !(flags & 0x20)` through the 8-bit `not ; and 1` form.
class CEnginePrimitive { public: long Type; };

class CEnginePrimitiveHandle;
class CEngineInternalPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3
    virtual void v004();
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);   // slot 5 (+0x14)
    long          RefCount;                         // +0x04
    long          Type;                             // +0x08
    unsigned char _pad0c[0x18];
    signed char   GridLevel;                        // +0x24 (GridPos.Level)
    unsigned char _pad25[0x0f];
    unsigned char Enable : 1;                       // +0x34
    unsigned char NonPersistent : 1;
};

struct CIVCountedPointer {
    CEngineInternalPrimitiveBase* Object;
    long GetRefCount() const { return Object ? Object->RefCount : 0; }
    __forceinline void Release()
    {
        CEngineInternalPrimitiveBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->RefCount;
        return *this;
    }
};

class CEnginePrimitiveHandle {
public:
    void*             _vptr;
    CIVCountedPointer Data;                         // +0x04
    __forceinline void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data.Object->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline void Set(CEngineInternalPrimitiveBase* primitive)
    {
        Clear();
        Data.Release();
        Data.Object = primitive;
    }
};

class CEnginePrimitiveManager {
public:
    virtual void v00();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);   // slot 1
    void RegisterPrimitive(CEngineInternalPrimitiveBase* primitive, unsigned long flags);   // 0x00BACDD0
};
class CEnginePrimitiveTypeInfo { public:
    unsigned char            _pad[0x10];
    CEnginePrimitiveManager* PrimitiveManagers[47];   // +0x10
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

class CEnginePrimitiveRenderer2D {
public:
    void AddPrimitive(CEngineInternalPrimitiveBase* primitive);   // 0x00B4A670
};
extern CEnginePrimitiveRenderer2D* g_pEnginePrimitiveRenderer2D;   // 0x01436E7C

class CEngineScene {
public:
    static void __fastcall UpdatePosition(CEngineInternalPrimitiveBase& primitive, unsigned long flags);   // 0x00B91840
};

class CEnginePrimitiveRenderer {
public:
    void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags, bool nonPersistent);
};

void CEnginePrimitiveRenderer::SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive,
                                                      unsigned long flags, bool nonPersistent)
{
    long type = primitive.Type;
    CEnginePrimitiveManager* manager = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[type];
    unsigned char enable = (unsigned char)~(flags >> 5) & 1;
    CEngineInternalPrimitiveBase* current = handle.Data.Object;
    if (current) {
        if (current->Type == type) {
            current->Update(primitive, flags);
            current->Enable = enable;
            if (current->GridLevel >= 0)
                CEngineScene::UpdatePosition(*current, flags);
            return;
        }
        handle.Clear();
    }
    if (!manager)
        return;
    CEngineInternalPrimitiveBase* created = manager->CreateInternalPrimitive(type);
    handle.Set(created);
    created->NonPersistent = nonPersistent;
    created->Update(primitive, flags);
    created->Enable = enable;
    if (flags & 0x40)
        g_pEnginePrimitiveRenderer2D->AddPrimitive(created);
    else
        manager->RegisterPrimitive(created, flags);
}

// ---- stubs ----
static int g_log[64]; static int g_n;
static void LOG(int v) { if (g_n < 64) g_log[g_n++] = v; }
struct TestPrim : CEngineInternalPrimitiveBase {
    int id; int destroyed; int notified; int updates; unsigned long lastFlags;
};
void CEngineInternalPrimitiveBase::v000() { }
void CEngineInternalPrimitiveBase::v002() { }
void CEngineInternalPrimitiveBase::v004() { }
void CEngineInternalPrimitiveBase::Destroy() { ((TestPrim*)this)->destroyed++; LOG(100 + ((TestPrim*)this)->id); }
void CEngineInternalPrimitiveBase::OnLastHandleReleased(CEnginePrimitiveHandle*) { ((TestPrim*)this)->notified++; LOG(200 + ((TestPrim*)this)->id); }
void CEngineInternalPrimitiveBase::Update(const CEnginePrimitive&, unsigned long flags) { ((TestPrim*)this)->updates++; ((TestPrim*)this)->lastFlags = flags; LOG(300 + ((TestPrim*)this)->id); }

static TestPrim g_new;
void CEnginePrimitiveManager::v00() { }
CEngineInternalPrimitiveBase* CEnginePrimitiveManager::CreateInternalPrimitive(long type) { g_new.Type = type; g_new.RefCount = 1; LOG(400 + type); return &g_new; }
static int g_registered; static unsigned long g_regFlags;
void CEnginePrimitiveManager::RegisterPrimitive(CEngineInternalPrimitiveBase* p, unsigned long flags) { g_registered++; g_regFlags = flags; LOG(500); }
static int g_added;
void CEnginePrimitiveRenderer2D::AddPrimitive(CEngineInternalPrimitiveBase*) { g_added++; LOG(600); }
static int g_posUpdates;
void __fastcall CEngineScene::UpdatePosition(CEngineInternalPrimitiveBase&, unsigned long) { g_posUpdates++; LOG(700); }

static CEnginePrimitiveTypeInfo g_info;
static CEnginePrimitiveManager g_mgr;
static CEnginePrimitiveRenderer2D g_r2d;
CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo = &g_info;
CEnginePrimitiveRenderer2D* g_pEnginePrimitiveRenderer2D = &g_r2d;

static int fails;
static void check(int c, const char* what) { if (!c) { fails++; printf("FAIL %s\n", what); } }
static void reset() { g_n = 0; g_registered = g_added = g_posUpdates = 0; g_new.id = 9; g_new.destroyed = g_new.notified = g_new.updates = 0; g_new.Enable = 0; g_new.NonPersistent = 0; }
static void mk(TestPrim& p, int id, long type, long rc, signed char lvl) { p.id = id; p.Type = type; p.RefCount = rc; p.GridLevel = lvl; p.destroyed = p.notified = p.updates = 0; p.Enable = 0; p.NonPersistent = 0; }

int main()
{
    CEnginePrimitiveRenderer r; CEnginePrimitiveHandle h; CEnginePrimitive prim; TestPrim cur;
    g_info.PrimitiveManagers[3] = &g_mgr; g_info.PrimitiveManagers[4] = 0;
    // 1: same type, grid level >= 0 -> update in place + position update
    reset(); mk(cur, 1, 3, 2, 0); h.Data.Object = &cur; prim.Type = 3;
    r.SetPersistentPrimitive(h, prim, 0x11, true);
    check(cur.updates == 1 && cur.lastFlags == 0x11 && cur.Enable == 1 && g_posUpdates == 1 && h.Data.Object == &cur && cur.RefCount == 2, "same type lvl>=0");
    // 2: same type, grid level < 0, flag 0x20 -> disabled, no position update
    reset(); mk(cur, 1, 3, 2, -1); cur.Enable = 1; h.Data.Object = &cur;
    r.SetPersistentPrimitive(h, prim, 0x20, false);
    check(cur.updates == 1 && cur.Enable == 0 && g_posUpdates == 0 && g_registered == 0, "same type lvl<0");
    // 3: different type, unique owner -> notify + destroy, new primitive registered with manager
    reset(); mk(cur, 1, 5, 1, 0); h.Data.Object = &cur; prim.Type = 3;
    r.SetPersistentPrimitive(h, prim, 0x2, true);
    check(cur.notified == 1 && cur.destroyed == 1 && h.Data.Object == &g_new && g_new.RefCount == 1, "replace unique");
    check(g_new.NonPersistent == 1 && g_new.updates == 1 && g_new.Enable == 1 && g_registered == 1 && g_regFlags == 0x2 && g_added == 0, "replace unique new");
    check(g_n == 5 && g_log[0] == 201 && g_log[1] == 101 && g_log[2] == 403 && g_log[3] == 309 && g_log[4] == 500, "replace unique order");
    // 4: different type, shared -> no notify, no destroy, count dropped; flag 0x40 -> 2D renderer
    reset(); mk(cur, 1, 5, 3, 0); h.Data.Object = &cur;
    r.SetPersistentPrimitive(h, prim, 0x60, false);
    check(cur.notified == 0 && cur.destroyed == 0 && cur.RefCount == 2 && h.Data.Object == &g_new, "replace shared");
    check(g_new.NonPersistent == 0 && g_new.Enable == 0 && g_added == 1 && g_registered == 0, "replace shared new");
    // 5: empty handle, manager present -> create
    reset(); h.Data.Object = 0;
    r.SetPersistentPrimitive(h, prim, 0, false);
    check(h.Data.Object == &g_new && g_registered == 1, "empty create");
    // 6: empty handle, no manager -> nothing
    reset(); h.Data.Object = 0; prim.Type = 4;
    r.SetPersistentPrimitive(h, prim, 0, false);
    check(h.Data.Object == 0 && g_n == 0, "empty no manager");
    // 7: different type, no manager -> released only
    reset(); mk(cur, 1, 5, 1, 0); h.Data.Object = &cur; prim.Type = 4;
    r.SetPersistentPrimitive(h, prim, 0, false);
    check(h.Data.Object == 0 && cur.notified == 1 && cur.destroyed == 1 && g_n == 2, "release no manager");
    if (!fails) printf("00b324a0_TEST PASS\n");
    return fails;
}