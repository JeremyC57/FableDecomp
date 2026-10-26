// Behaviour test for 008ec870: src/008ec870.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCDegradableBase::OnCreate()
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCDegradableBase::OnCreate() -- clear the last hit frame, fetch the parent thing's definition, take its degradable sub-def, clear the current stage and remember the parent as the physical thing
// Leaf: OnCreate
//
// Retail 0x008EC870, 98 bytes (plain `ret`; vtable 0x0126A6A4 slot 4, Ego_r.pdb
// `?OnCreate@CTCDegradableBase@@MAEXXZ`; the catalog "CTCDegradableBarrier::vfunc_4" is the slot name).
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0 (manifest "_Cons_val<...>", propagated),
// CDefinitionManager::GetDef(long, CDefPointer*) @ 0x0042AF3C (manifest name is a byte-twin template),
// CDefClassBase::GetSubDef<CDegradableDef>(CDefPointer*) @ 0x008ECDA0 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>", propagated); the def's virtual slot 1 when the
// temporary def pointer drops its last reference.
// Layout = Ego_r.pdb CTCDegradableBase (0x1c): LastWorldFrameHit +0xc, PDefinition +0x10,
// PCurrentDegradableInfo +0x14, PParentThingPhysical +0x18; CThing's definition index is the word at +0x8c.
// Shape: the temporary CDefPointer is zeroed after the argument pushes (declared, then the call chain);
// its object is read into a local (edi) right after GetDef, and the release is written by hand on that
// local (null test, `dec [edi+4] ; jne`, virtual destroy).
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                                // +0x8c
};

class CDefClassBase;
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};

class CDegradableDef;
class CDegradableInfo;
class CThingPhysical;

class CDefClassBase { public:
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long RefCount;                                          // +0x04
    bool GetSubDef(CDefPointer<const CDegradableDef>* out); // 0x008ECDA0
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CDegradableDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CDefinitionManager* GetDefinitionManager();             // 0x006869D0
};

class CTCDegradableBase : public CTCBase { public:
    long                             LastWorldFrameHit;     // +0x0c
    CDefPointer<const CDegradableDef> PDefinition;          // +0x10
    const CDegradableInfo*           PCurrentDegradableInfo;// +0x14
    CThingPhysical*                  PParentThingPhysical;  // +0x18
    void OnCreate();
};

void CTCDegradableBase::OnCreate()
{
    LastWorldFrameHit = 0;
    CDefPointer<CDefClassBase> def;
    GetDefinitionManager()->GetDef(ParentThing->DefIndex, &def);
    CDefClassBase* object = def.Object;
    object->GetSubDef(&PDefinition);
    PCurrentDegradableInfo = 0;
    PParentThingPhysical = (CThingPhysical*)ParentThing;
    if (object != 0 && --object->RefCount == 0)
        object->DestroySelf();
}

// ---- stubs ----
static void* g_def = 0; static void* g_sub = 0; static int g_destroyed = 0; static int g_findDef = 0, g_findSub = 0;
void* FindDef_CDefinitionManager_GetDef_0() { ++g_findDef; return g_def; }
void* FindDef_CDefClassBase_GetSubDef_1() { ++g_findSub; return g_sub; }
void CDefClassBase::v00() {}
void CDefClassBase::DestroySelf() { ++g_destroyed; }
void CTCBase::v00() {}
static CDefinitionManager g_mgr;
CDefinitionManager* CTCBase::GetDefinitionManager() { return &g_mgr; }
struct CTCDegradableTest : CTCDegradableBase { };
int main()
{
    static CThing parent; parent.DefIndex = 7;
    static CDefClassBase def; static int subDef;
    static unsigned char store[sizeof(CTCDegradableBase)];
    CTCDegradableBase& tc = *(CTCDegradableBase*)store;
    tc.ParentThing = &parent; tc.LastWorldFrameHit = 55; tc.PCurrentDegradableInfo = (const CDegradableInfo*)1; tc.PParentThingPhysical = 0;
    // def found with two references: the sub-def is fetched, the def survives
    def.RefCount = 2; g_def = &def; g_sub = &subDef;
    tc.OnCreate();
    CHECK(tc.LastWorldFrameHit == 0); CHECK(tc.PCurrentDegradableInfo == 0); CHECK(tc.PParentThingPhysical == (CThingPhysical*)&parent);
    CHECK((void*)tc.PDefinition.Object == (void*)&subDef); CHECK(def.RefCount == 1); CHECK(g_destroyed == 0); CHECK(g_findDef == 1); CHECK(g_findSub == 1);
    // last reference dropped: the def destroys itself
    tc.LastWorldFrameHit = 3; g_sub = 0; tc.PDefinition.Object = 0;
    tc.OnCreate();
    CHECK(def.RefCount == 0); CHECK(g_destroyed == 1); CHECK(tc.PDefinition.Object == 0); CHECK(tc.LastWorldFrameHit == 0);
    // no def: nothing released, the stage and physical pointer still reset
    g_def = 0; g_destroyed = 0; tc.PCurrentDegradableInfo = (const CDegradableInfo*)1; tc.PParentThingPhysical = 0;
    tc.OnCreate();
    CHECK(g_destroyed == 0); CHECK(g_findDef == 3); CHECK(tc.PCurrentDegradableInfo == 0); CHECK(tc.PParentThingPhysical == (CThingPhysical*)&parent);
    if (fails == 0) printf("008ec870_TEST PASS\n"); else printf("008ec870_TEST FAIL\n");
    return fails;
}