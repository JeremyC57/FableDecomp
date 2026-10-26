// Behaviour test for 00730630: src/00730630.cpp copied verbatim (the constructor and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCDoor::CTCDoor(CThing*)
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCDoor::CTCDoor(CThing*) -- CTCBase(thing), not jammed, closed and still, no creature/update stamps, then the door def fetched from the parent's definition and the parent's cutscene pause behaviour set to 2
// Leaf: ??0CTCDoor
//
// Retail 0x00730630, 129 bytes (`ret 4`). Callees: CTCBase::CTCBase(CThing*) @ 0x00686800,
// CTCBase::GetDefinitionManager() @ 0x006869D0 (manifest "_Cons_val<...>", propagated),
// CDefinitionManager::GetDef(long, CDefPointer*) @ 0x0042AF3C, CDefClassBase::GetSubDef<CDoorDef>
// @ 0x00655D28 (manifest "GetSubDef<CAbilityDef>", a byte-twin template name), CThing::
// SetCutscenePauseBehaviour(long) @ 0x004C78A0; the def's virtual slot 1 when the temporary def pointer
// drops its last reference. Vtable 0x01266DE4 = CTCDoor (rtti-vtables.tsv).
// Layout = engine/CTCDoor.h (Ego_r.pdb, 0x24): Jammed +0xc, CurrentOpen/OldOpen/Opening/Closing
// +0x10..+0x13, LastCreatureFoundWF +0x14, UpdateID +0x18, TriggerType +0x1c (left uninitialised),
// PDef +0x20. The generated header is a flat record with no CTCBase base or constructor, so the class
// is modelled locally with the same offsets.
// Shape: one zero register (ebx) for the initialisers, PDef addressed once in edi (it is the
// GetSubDef argument); the temporary def pointer's zero store lands after the GetDef pushes, its object
// is loaded after the GetSubDef push (`push edi ; mov edi,[esp+14h]`) and the hand-written release
// compares it with the zero register.
// Retry 10b: same-TU GetDef/GetSubDef stand-ins that write the out slot inline (no escape) move the zero store
// after the pushes; the parent read into a named local first gives retail's eax/edx roles for the def index.
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                                // +0x8c
    void SetCutscenePauseBehaviour(long behaviour);         // 0x004C78A0
};

template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};

class CDoorDef;

class CDefClassBase { public:
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long RefCount;                                          // +0x04
    bool GetSubDef(CDefPointer<const CDoorDef>* out);       // 0x00655D28
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CDoorDef>* out)
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
    CTCBase(CThing* thing);                                 // 0x00686800
    CDefinitionManager* GetDefinitionManager();             // 0x006869D0
};

class CTCDoor : public CTCBase { public:                                  // vtable 0x01266DE4
    long                        Jammed;                     // +0x0c
    bool                        CurrentOpen;                // +0x10
    bool                        OldOpen;                    // +0x11
    bool                        Opening;                    // +0x12
    bool                        Closing;                    // +0x13
    long                        LastCreatureFoundWF;        // +0x14
    long                        UpdateID;                   // +0x18
    long                        TriggerType;                // +0x1c EDoorTriggerType
    CDefPointer<const CDoorDef> PDef;                       // +0x20
    virtual void v00();
    CTCDoor(CThing* thing);
};

CTCDoor::CTCDoor(CThing* thing)
    : CTCBase(thing)
    , Jammed(0)
    , CurrentOpen(false)
    , OldOpen(false)
    , Opening(false)
    , Closing(false)
    , LastCreatureFoundWF(0)
    , UpdateID(0)
    , PDef()
{
    CDefPointer<CDefClassBase> def;
    CThing* parent = ParentThing;
    GetDefinitionManager()->GetDef(parent->DefIndex, &def);
    CDefClassBase* object = def.Object;
    object->GetSubDef(&PDef);
    ParentThing->SetCutscenePauseBehaviour(2);
    if (object != 0 && --object->RefCount == 0)
        object->DestroySelf();
}

// ---- stubs ----
static void* g_def = 0; static void* g_sub = 0; static int g_destroyed = 0; static long g_index = -1; static long g_pause = -1;
static CThing* g_baseThing = 0;
void* FindDef_CDefinitionManager_GetDef_0() { return g_def; }
void* FindDef_CDefClassBase_GetSubDef_1() { return g_sub; }
void CDefClassBase::v00() {}
void CDefClassBase::DestroySelf() { ++g_destroyed; }
static CDefinitionManager g_mgr;
CDefinitionManager* CTCBase::GetDefinitionManager() { return &g_mgr; }
CTCBase::CTCBase(CThing* thing) { g_baseThing = thing; ParentThing = thing; ToBeRemoved = false; IsInGlobalUpdateTCs = false; }
void CTCBase::v00() {}
void CTCDoor::v00() {}
void CThing::SetCutscenePauseBehaviour(long behaviour) { g_pause = behaviour; }
int main()
{
    static CThing parent; parent.DefIndex = 12;
    static CDefClassBase def; static int doorDef;
    static unsigned char store[sizeof(CTCDoor)];
    memset(store, 0xcd, sizeof(store));
    def.RefCount = 1; g_def = &def; g_sub = &doorDef;
    CTCDoor* door = new (store) CTCDoor(&parent);
    CHECK(g_baseThing == &parent); CHECK(door->ParentThing == &parent);
    CHECK(door->Jammed == 0); CHECK(!door->CurrentOpen && !door->OldOpen && !door->Opening && !door->Closing);
    CHECK(door->LastCreatureFoundWF == 0); CHECK(door->UpdateID == 0);
    CHECK((void*)door->PDef.Object == (void*)&doorDef); CHECK(g_pause == 2);
    CHECK(def.RefCount == 0); CHECK(g_destroyed == 1);
    // def with other holders: not destroyed; no def at all: no release
    def.RefCount = 3; g_pause = -1; memset(store, 0xcd, sizeof(store));
    door = new (store) CTCDoor(&parent);
    CHECK(def.RefCount == 2); CHECK(g_destroyed == 1); CHECK(g_pause == 2);
    g_def = 0; g_sub = 0; memset(store, 0xcd, sizeof(store));
    door = new (store) CTCDoor(&parent);
    CHECK(door->PDef.Object == 0); CHECK(g_destroyed == 1);
    if (fails == 0) printf("00730630_TEST PASS\n"); else printf("00730630_TEST FAIL\n");
    return fails;
}