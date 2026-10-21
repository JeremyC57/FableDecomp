// Behaviour test for 008da2e0: src/008da2e0.cpp copied verbatim; OnActivate runs the stand base, reads the scorpion-king
// battle sub-def's exhausted time, clears the type-9 component's active flag (the map's end entry when the type is not in
// the map), sets RecoveryCount = time * frames-per-second, runs CModeBase::OnActivate, sets physics mode 8, clears bit 0 of
// the thing's +0xbc flags and resets the creature's action unless one is being performed; the def is released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NCreatureMode::CModeScorpionKingExhausted::OnActivate() -- the scorpion king tires: stand-mode activation, clear the type-9 component's active flag, recovery countdown = the battle def's exhaustion time in frames, re-apply the weapon filter, switch its physics to mode 8, clear thing flag bit 0 at +0xbc and, when no action is running, reset the creature's current action
// Leaf: OnActivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA2E0, 195 bytes (`ret`).
// Callees: CModeStandBase::OnActivate 0x008621A0 and CTCCreatureModeManager::CModeBase::OnActivate 0x0073AAC0 (both qualified);
// CDefClassBase::GetSubDef(CDefPointer<const CScorpionKingBattleDef>*) 0x008C5A50 (manifest "GetSubDef<CAICreatureWillPowerIndicatorDef>"
// is a byte-twin label); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined HasTC/GetTC, type 9 = bit 9 of +0x20);
// __ftol2 0x00BFEA70; the physics object's (+0x60) virtual slot 43 (+0xac); CThingCreatureBase::IsPerformingAnyAction 0x006622A0;
// PCreature's virtual slot 28 (+0x70); the def's virtual slot 1 (delete at zero references).
// Globals: the frames-per-second long 0x01375550.
// Layout (PDB CModeScorpionKingExhausted 0x80, matches retail): CModeStandBase 0x68, RecoveryCount +0x68, Filter +0x6c,
// PCreature +0x78, bRecovering +0x7c. Thing: def +0x70, physics +0x60, flags +0xbc. The battle def's float at +0x40 is the
// exhaustion time (its PDB name is not pinned down: the def's base sits below the PDB's).
// Shape: GetSubDef has a same-TU out-slot-only stand-in body, so the out word is reused by the LowerBound key; the thing's
// def class is read before the out CDefPointer is zeroed; the def is copied out of it into a holder (ebp) read once for the
// time and released at the end without a null test (`dec [ebp+4] ; jne`); the component is HasTC + a returned GetTC with
// the end/hit choice as one ternary (the landed 0x008D8F30 idiom; null when absent, written unconditionally); ParentThing
// is read into a local once for the flag clear and the IsPerformingAnyAction call.
struct CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

struct CDefPointeeBase {
    virtual void Destroy();                // slot 0
    virtual void Delete();                 // slot 1 (+0x04)
    long m_refCount;                       // +0x04
};

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};

template <class T> struct CDefHolder {     // the def taken out of the out pointer, released when the function returns
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (--Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};

struct CScorpionKingBattleDef : CDefPointeeBase {
    unsigned char _pad08[0x40 - 0x08];
    float         ExhaustedTime;           // +0x40
};

struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CScorpionKingBattleDef>* out);   // 0x008C5A50
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CScorpionKingBattleDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CScorpionKingBattleDef*)def;
    return true;
}
#pragma auto_inline(on)

struct CPhysicsObject {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042();
    virtual void SetPhysicsMode(long mode);   // slot 43 (+0xac)
};

struct CTCActiveFlag {                     // the type-9 component (as CModeOutOfActionBase::OnDeactivate 0x008D8F30)
    unsigned char _pad00[0x10];
    bool          m_active;                // +0x10
};

struct CThing {
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];      // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;          // +0x44
    unsigned char   _pad4c[0x14];
    CPhysicsObject* PPhysics;              // +0x60
    unsigned char   _pad64[0x0c];
    CDefClassBase*  PDef;                  // +0x70
    unsigned char   _pad74[0xbc - 0x74];
    unsigned char   m_flagsBC;             // +0xbc
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
};

struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction();          // 0x006622A0
};

struct CCreatureController {               // what PCreature's slot 28 is called on
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void ResetCurrentAction(bool immediately);   // slot 28 (+0x70)
};

extern long g_framesPerSecond;             // 0x01375550

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
    void OnActivate();                     // 0x0073AAC0
};

struct CModeStandBase : CModeBase {        // retail sizeof 0x68
    unsigned char _stand[0x4c];
    void OnActivate();                     // 0x008621A0
};

struct CAnimFilterOfGroup {                // vtable 0x01267568
    virtual void v00();
    void*         Group;                   // +4 (CCharString)
    unsigned long GroupCRC;                // +8
};

namespace NCreatureMode {

struct CModeScorpionKingExhausted : CModeStandBase {
    long                  RecoveryCount;   // +0x68
    CAnimFilterOfGroup    Filter;          // +0x6c
    CCreatureController*  PCreature;       // +0x78
    bool                  bRecovering;     // +0x7c
    void OnActivate();
};

void CModeScorpionKingExhausted::OnActivate()
{
    CModeStandBase::OnActivate();
    CDefClassBase* thingDef = ParentThing->PDef;
    CDefPointer<CScorpionKingBattleDef> out;
    thingDef->GetSubDef(&out);
    CDefHolder<CScorpionKingBattleDef> def(out.Object);
    float exhaustedTime = def->ExhaustedTime;
    CTCActiveFlag* flag = 0;
    if (ParentThing->HasTC(9))
        flag = ParentThing->GetTC<CTCActiveFlag>(9);
    flag->m_active = false;
    RecoveryCount = (long)(exhaustedTime * g_framesPerSecond);
    CModeBase::OnActivate();
    ParentThing->PPhysics->SetPhysicsMode(8);
    CThingCreatureBase* parent = ParentThing;
    parent->m_flagsBC &= ~1;
    if (!parent->IsPerformingAnyAction())
        PCreature->ResetCurrentAction(false);
}

}

// ---- harness ----
static char g_log[32]; static int g_n = 0;
static void L(char c) { if (g_n < 31) g_log[g_n++] = c; g_log[g_n] = 0; }
static int g_deleted = 0; static void* g_def = 0; static const void* g_findOwner = 0;
static long g_mode = -1; static int g_resetImm = -1; static bool g_performing = false; static const void* g_perfThis = 0;
long g_framesPerSecond = 30;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void* Find_CDefClassBase_GetSubDef(const void* owner) { g_findOwner = owner; if (g_def) ++((CDefPointeeBase*)g_def)->m_refCount; return g_def; }
void CDefPointeeBase::Destroy() { }
void CDefPointeeBase::Delete() { ++g_deleted; }
void CPhysicsObject::v000() { }
void CPhysicsObject::v001() { }
void CPhysicsObject::v002() { }
void CPhysicsObject::v003() { }
void CPhysicsObject::v004() { }
void CPhysicsObject::v005() { }
void CPhysicsObject::v006() { }
void CPhysicsObject::v007() { }
void CPhysicsObject::v008() { }
void CPhysicsObject::v009() { }
void CPhysicsObject::v010() { }
void CPhysicsObject::v011() { }
void CPhysicsObject::v012() { }
void CPhysicsObject::v013() { }
void CPhysicsObject::v014() { }
void CPhysicsObject::v015() { }
void CPhysicsObject::v016() { }
void CPhysicsObject::v017() { }
void CPhysicsObject::v018() { }
void CPhysicsObject::v019() { }
void CPhysicsObject::v020() { }
void CPhysicsObject::v021() { }
void CPhysicsObject::v022() { }
void CPhysicsObject::v023() { }
void CPhysicsObject::v024() { }
void CPhysicsObject::v025() { }
void CPhysicsObject::v026() { }
void CPhysicsObject::v027() { }
void CPhysicsObject::v028() { }
void CPhysicsObject::v029() { }
void CPhysicsObject::v030() { }
void CPhysicsObject::v031() { }
void CPhysicsObject::v032() { }
void CPhysicsObject::v033() { }
void CPhysicsObject::v034() { }
void CPhysicsObject::v035() { }
void CPhysicsObject::v036() { }
void CPhysicsObject::v037() { }
void CPhysicsObject::v038() { }
void CPhysicsObject::v039() { }
void CPhysicsObject::v040() { }
void CPhysicsObject::v041() { }
void CPhysicsObject::v042() { }
void CCreatureController::v000() { }
void CCreatureController::v001() { }
void CCreatureController::v002() { }
void CCreatureController::v003() { }
void CCreatureController::v004() { }
void CCreatureController::v005() { }
void CCreatureController::v006() { }
void CCreatureController::v007() { }
void CCreatureController::v008() { }
void CCreatureController::v009() { }
void CCreatureController::v010() { }
void CCreatureController::v011() { }
void CCreatureController::v012() { }
void CCreatureController::v013() { }
void CCreatureController::v014() { }
void CCreatureController::v015() { }
void CCreatureController::v016() { }
void CCreatureController::v017() { }
void CCreatureController::v018() { }
void CCreatureController::v019() { }
void CCreatureController::v020() { }
void CCreatureController::v021() { }
void CCreatureController::v022() { }
void CCreatureController::v023() { }
void CCreatureController::v024() { }
void CCreatureController::v025() { }
void CCreatureController::v026() { }
void CCreatureController::v027() { }
void CPhysicsObject::SetPhysicsMode(long mode) { g_mode = mode; L('P'); }
void CCreatureController::ResetCurrentAction(bool immediately) { g_resetImm = immediately; L('R'); }
bool CThingCreatureBase::IsPerformingAnyAction() { g_perfThis = this; L('I'); return g_performing; }
void CModeBase::v00() { }
void CModeBase::OnActivate() { L('B'); }
void CModeStandBase::OnActivate() { L('S'); }
void CAnimFilterOfGroup::v00() { }

static void reset() { g_n = 0; g_log[0] = 0; g_deleted = 0; g_findOwner = 0; g_mode = -1; g_resetImm = -1; g_perfThis = 0; }

int main()
{
    CDefClassBase defClass;
    CScorpionKingBattleDef def; def.m_refCount = 1; def.ExhaustedTime = 2.5f;
    CTCActiveFlag comp3, comp9, comp12, endComp;
    comp9.m_active = true; comp3.m_active = true; comp12.m_active = true; endComp.m_active = true;
    CInterfaceMap::SEntry entries[4] = { { 3, (CTCBase*)&comp3 }, { 9, (CTCBase*)&comp9 }, { 12, (CTCBase*)&comp12 },
                                         { 99, (CTCBase*)&endComp } };
    CPhysicsObject phys;
    CCreatureController ctrl;
    CThingCreatureBase thing; memset(&thing, 0, sizeof thing);
    thing.m_presentBits[0] = (1UL << 9) | (1UL << 3) | (1UL << 12);
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    thing.PPhysics = &phys; thing.PDef = &defClass; thing.m_flagsBC = 0x13;
    NCreatureMode::CModeScorpionKingExhausted mode; memset(&mode, 0, sizeof mode);
    *(void**)&mode = 0;
    mode.ParentThing = &thing; mode.PCreature = &ctrl; mode.RecoveryCount = -1;

    // type 9 present, not performing: flag cleared, count set, action reset
    reset(); g_def = &def; g_performing = false;
    mode.OnActivate();
    CHECK(strcmp(g_log, "SBPIR") == 0);
    CHECK(g_findOwner == &defClass && !comp9.m_active && comp3.m_active && comp12.m_active && endComp.m_active);
    CHECK(mode.RecoveryCount == 75 && g_mode == 8 && thing.m_flagsBC == 0x12 && g_perfThis == &thing && g_resetImm == 0);
    CHECK(def.m_refCount == 1 && g_deleted == 0);

    // performing: no reset
    reset(); comp9.m_active = true; g_performing = true; g_framesPerSecond = 10; def.ExhaustedTime = 1.75f;
    mode.OnActivate();
    CHECK(strcmp(g_log, "SBPI") == 0 && g_resetImm == -1 && !comp9.m_active && mode.RecoveryCount == 17);

    // type 9 flagged but not in the map: the end entry's component is written
    reset(); comp9.m_active = true; entries[1].type = 10; g_performing = false;
    mode.OnActivate();
    CHECK(comp9.m_active && !endComp.m_active && strcmp(g_log, "SBPIR") == 0);
    entries[1].type = 9;

    // last reference: the def is deleted at the end
    reset(); def.m_refCount = 0;
    mode.OnActivate();
    CHECK(def.m_refCount == 0 && g_deleted == 1);

    if (fails == 0) printf("008da2e0_TEST PASS\n");
    return fails;
}