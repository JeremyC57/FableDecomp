// Behaviour test for 00833010: src/00833010.cpp copied verbatim; LowerBound, the scheduler hook, the getters,
// the brain ctor and the counted-pointer Reset are stubbed. CThingAICreature::SetNewBrain
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingAICreature::SetNewBrain(CDefPointer<const CAIBrainDef> brainDef) -- tells the idle scheduler (when present) a new brain is coming, builds a brain for this creature from the def over the display engine, world, thing and player managers, and installs it in the counted brain pointer
// Grade: RELOCATION_MATCH (retry helper C) -- the thing-manager argument is read through an inline accessor GetThingManager() (an inlined call is evaluated before the real getter calls), so g_pThingManager loads into ebx ahead of GetPlayerManager and the brain takes ebp, as retail. Was DIFFER(176v178), parked as a whole-program mod/ref gap.
// Leaf: SetNewBrain
// Retail: Fable.exe (TLC, VC7.1) @ 0x00833010, 178 bytes (+14 int3). `ret 4` (the def pointer passed by value).
// Callees: CVectorMap::LowerBound(const long*) 0x0040F020, the idle scheduler's brain-reset 0x008018B0 (manifest
//          "LTextBinTree_RebuildWalkerArray" propagated; named by use), operator new 0x00BFEA1A (0xA8 bytes), CThing::
//          GetPlayerManager() 0x004C7A00 (manifest "CViewBase::GetDisplayEngine" propagated), CWorld* CThing::PeekWorld()
//          0x004C7AA0, CDisplayEngine* CThing::GetDisplayEngine() 0x004C7A70, CAIBrain::CAIBrain(...) 0x0088C160,
//          CCountedPointer<CAIBrain>::Reset(CAIBrain*) 0x00834E40 on PBrain, CDefClassBase virtual slot 1 (Delete).
// Shape: the 0xD3 presence bit is word +0x38 bit 19 (the lookup through the map pointer in edi); the `new` expression's null
//        test skips the constructor; the by-value def copy for the constructor is built on the stack (`push ecx ; mov [esp],edi`)
//        with its add-ref, the manager getters evaluated after it; the parameter's own copy is released at the end.
// Layout: CThing presence bits +0x20, Components +0x44; PBrain +0x1A8 (Ego_r.pdb +0x1D8, 0x30 lower in retail).
class CTCBase;
class CTCIdleScheduler { public:                            // interface type 0xD3
    void OnNewBrain_008018b0();                      // 0x008018B0 (manifest label propagated)
};
class CDefClassBase { public:
    virtual void v00();
    virtual void Delete();                           // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};
struct CAIBrainDef;
struct CDefPointer {                                 // CDefPointer<const CAIBrainDef>
    const CDefClassBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++const_cast<CDefClassBase*>(Object)->RefCount;
    }
    ~CDefPointer()
    {
        if (Object) {
            if (--const_cast<CDefClassBase*>(Object)->RefCount == 0)
                const_cast<CDefClassBase*>(Object)->Delete();
        }
    }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
class CDisplayEngine; class CWorld; class CThingManager; class CPlayerManager; class CThingAICreature;
extern CThingManager* g_pThingManager;               // 0x013B8A1C
inline CThingManager* GetThingManager() { return g_pThingManager; }
class CAIBrain { public:                                    // 0xA8 bytes
    unsigned char _body[0xa8];
    CAIBrain(CDisplayEngine* display, CWorld* world, CThingManager* things, CPlayerManager* players, CThingAICreature* owner, CDefPointer def);   // 0x0088C160
};
struct CBrainPointer {                               // CCountedPointer<CAIBrain>
    void Reset(CAIBrain* brain);                     // 0x00834E40
};
class CThingAICreature { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x1a8 - 0x4c];
    CBrainPointer PBrain;                            // +0x1a8
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    CPlayerManager* GetPlayerManager();              // 0x004C7A00
    CDisplayEngine* GetDisplayEngine();              // 0x004C7A70
    CWorld* PeekWorld();                             // 0x004C7AA0
    void SetNewBrain(CDefPointer brainDef);
};
void CThingAICreature::SetNewBrain(CDefPointer brainDef)
{
    CTCIdleScheduler* scheduler;
    if (GetInterface(&scheduler, 0xd3))
        scheduler->OnNewBrain_008018b0();
    CAIBrain* brain = new CAIBrain(GetDisplayEngine(), PeekWorld(), GetThingManager(), GetPlayerManager(), this, brainDef);
    PBrain.Reset(brain);
}

// ---- stubs and driver ----
CThingManager* g_pThingManager = (CThingManager*)0x7000;
static int g_hook = 0; static CTCIdleScheduler* g_hookThis = 0;
void CTCIdleScheduler::OnNewBrain_008018b0() { ++g_hook; g_hookThis = this; }
CPlayerManager* CThingAICreature::GetPlayerManager() { return (CPlayerManager*)0x3000; }
CDisplayEngine* CThingAICreature::GetDisplayEngine() { return (CDisplayEngine*)0x1000; }
CWorld* CThingAICreature::PeekWorld() { return (CWorld*)0x2000; }
static int g_ctor = 0; static void* g_ctorThis = 0; static void* g_args[5]; static const CDefClassBase* g_ctorDef = 0; static long g_refInCtor = 0;
CAIBrain::CAIBrain(CDisplayEngine* d, CWorld* w, CThingManager* t, CPlayerManager* p, CThingAICreature* o, CDefPointer def)
{ ++g_ctor; g_ctorThis = this; g_args[0] = d; g_args[1] = w; g_args[2] = t; g_args[3] = p; g_args[4] = o; g_ctorDef = def.Object; g_refInCtor = def.Object ? def.Object->RefCount : -1; }
static int g_reset = 0; static CAIBrain* g_resetBrain = 0; static CBrainPointer* g_resetThis = 0;
void CBrainPointer::Reset(CAIBrain* brain) { ++g_reset; g_resetBrain = brain; g_resetThis = this; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void CDefClassBase::v00() { }
void CDefClassBase::Delete() { }
struct CTestDef : CDefClassBase { int deleted; void v00() { } void Delete() { ++deleted; } };
static void reset() { g_hook = 0; g_hookThis = 0; g_ctor = 0; g_reset = 0; g_resetBrain = 0; memset(g_args, 0, sizeof g_args); g_ctorDef = 0; g_refInCtor = 0; g_ctorThis = 0; }
int main()
{
    static CThingAICreature c; memset(&c, 0, sizeof c);
    CTCIdleScheduler sched;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0xd3, (CTCBase*)&sched } };
    c.m_interfaces.first = entries; c.m_interfaces.last = entries + 2;
    CTestDef def; def.RefCount = 1; def.deleted = 0;
    const CDefClassBase* raw = &def;
    const CDefPointer& src = *(const CDefPointer*)&raw;          // a holder without its own release
    // no scheduler: brain built over the managers with an add-ref'd def, installed; the parameter copy released
    reset(); c.SetNewBrain(src);
    CHECK(g_hook == 0 && g_ctor == 1 && g_ctorThis != 0);
    CHECK(g_args[0] == (void*)0x1000 && g_args[1] == (void*)0x2000 && g_args[2] == (void*)0x7000 && g_args[3] == (void*)0x3000 && g_args[4] == (void*)&c);
    CHECK(g_ctorDef == &def && g_refInCtor == 3);
    CHECK(g_reset == 1 && g_resetBrain == (CAIBrain*)g_ctorThis && g_resetThis == &c.PBrain);
    CHECK(def.RefCount == 1 && def.deleted == 0);
    // scheduler present (bit 0xd3): told first
    c.m_presentBits[0xd3 >> 5] |= 1UL << (0xd3 & 31);
    reset(); c.SetNewBrain(src);
    CHECK(g_hook == 1 && g_hookThis == &sched && g_ctor == 1 && g_reset == 1 && def.RefCount == 1);
    // (the allocation-failure arm -- new returns null, the ctor is skipped -- is not driven: that needs a replacement global
    // operator new, which this lane does not declare; the real CRT allocator is used)
    // last reference released at the end deletes the def
    def.RefCount = 0;
    reset(); c.SetNewBrain(src);
    CHECK(g_ctor == 1 && g_refInCtor == 2 && def.RefCount == 0 && def.deleted == 1);
    // null def
    const CDefClassBase* none = 0;
    reset(); c.SetNewBrain(*(const CDefPointer*)&none);
    CHECK(g_ctor == 1 && g_ctorDef == 0 && g_reset == 1);
    if (fails == 0) printf("00833010_TEST PASS\n");
    return fails;
}