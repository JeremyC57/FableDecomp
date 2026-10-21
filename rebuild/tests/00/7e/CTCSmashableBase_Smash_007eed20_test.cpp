// Behaviour test for 007eed20: src/007eed20.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCSmashableBase::Smash()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCSmashableBase::Smash() -- smash the parent thing: when its smashable def names a replacement object, create that object at the thing's position and orientation (same map level), hand a live replacement's smashable component the last hit frame, then kill the thing
// Leaf: Smash
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EED20, 275 bytes (plain `ret`).
// Callees: the physics body's GetRHSet (slot 72, +0x120, const CRightHandedSet&), CCharString(const char*, long) @ 0x0099EBF0
//   ("" @ 0x0122D70E), CThing::GetPosition() const @ 0x004C73D0, Create(long, const C3DVector*, long, const CRightHandedSet*,
//   unsigned long, const CCharString&) @ 0x00703210 (`__fastcall`), ~CCharString @ 0x0099EAE0, CVectorMap::LowerBound
//   @ 0x0040F020 (interface 0x4E), CThing::Kill(bool) @ 0x004C9B80; the def's virtual slot 1 on the last release.
// Layout: Ego_r.pdb CTCSmashableBase (the landed OnInitialActivate 0x007EECF0 model): LastWorldFrameHit +0x0C, Definition
//   +0x14; CSmashableDef ReplacementObject +0x28; retail CThing interface bitset +0x20 (0x4E = word +0x28 bit 14), map +0x44,
//   map level byte +0x90, dead bit +0x91, PhysicsTC +0x60.
// Shape: `GetDef()->ReplacementObject` through the by-value def temporary (null-tested add-ref, release from the def's own
//   Release) read twice; the orientation is a local copy, the empty name a named local; the created object's component is
//   the HasTC + end/hit ternary lookup.
class CTCBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetRHSet() const;    // slot 72 (+0x120)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad64[0x90 - 0x64];
    signed char     MapLevel;                           // +0x90
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
    __forceinline unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (HasTC(type)) {
            *out = GetTC<T>(type);
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CThingObject : public CThing {};
CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long mapLevel, const CRightHandedSet* orientation,
                                unsigned long flags, const CCharString& scriptName);   // 0x00703210
struct CSmashableDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long          m_refCount;                           // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
    unsigned char _pad08[0x25 - 0x08];
    bool          Smashable;                            // +0x25
    long          ReplacementObject;                    // +0x28 CDefIndex
};
struct CDefPointer {
    CSmashableDef* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
    ~CDefPointer() { m_pDef->Release(); }
    CSmashableDef* operator->() const { return m_pDef; }
};
struct CTCSmashableComponent {                          // interface 0x4E (named descriptively)
    unsigned char _pad00[0x0c];
    long          LastWorldFrameHit;                    // +0x0c
};
struct CTCSmashableBase {
    void*       __vftable;                              // +0x00 (CTCBase)
    CThing*     ParentThing;                            // +0x04
    bool        ToBeRemoved;                            // +0x08
    bool        IsInGlobalUpdateTCs;                    // +0x09
    long        LastWorldFrameHit;                      // +0x0c
    bool        Smashed;                                // +0x10
    CDefPointer Definition;                             // +0x14
    CDefPointer GetDef() const { return Definition; }
    void Smash();
};

void CTCSmashableBase::Smash()
{
    long replacement = GetDef()->ReplacementObject;
    if (replacement > 0) {
        CRightHandedSet orientation = ParentThing->PhysicsTC->GetRHSet();
        CThingObject* object;
        {
            CCharString scriptName("", -1);
            CThing* thing = ParentThing;
            long objectDef = GetDef()->ReplacementObject;
            object = Create(objectDef, &thing->GetPosition(), thing->MapLevel, &orientation, 0, scriptName);
        }
        CTCSmashableComponent* smashable;
        if (object && !(object->m_flags91 & 1) && object->GetInterface(&smashable, 0x4e))
            smashable->LastWorldFrameHit = LastWorldFrameHit;
    }
    ParentThing->Kill(true);
}

// ---- stubs and driver ----
#include <new>
static int g_strCtors = 0, g_strDtors = 0;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; ++g_strCtors; }
CCharString::~CCharString() { ++g_strDtors; }
static CRightHandedSet g_rh = { { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } };
const CRightHandedSet& CTCPhysicsBase::GetRHSet() const { return g_rh; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static C3DVector g_pos = { 4.0f, 5.0f, 6.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
static CThing* g_killed; static bool g_killArg; static int g_kills = 0;
void CThing::Kill(bool immediately) { g_killed = this; g_killArg = immediately; ++g_kills; }
static CThingObject* g_createResult; static long g_cDef; static const C3DVector* g_cPos; static long g_cLevel; static CRightHandedSet g_cOrient; static unsigned long g_cFlags; static const char* g_cScript; static int g_creates = 0;
CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long mapLevel, const CRightHandedSet* orientation,
                                unsigned long flags, const CCharString& scriptName)
{ g_cDef = defIndex; g_cPos = position; g_cLevel = mapLevel; g_cOrient = *orientation; g_cFlags = flags; g_cScript = (const char*)scriptName.PStringData; ++g_creates; return g_createResult; }
static int g_destroyed = 0;
void CSmashableDef::v00() {}
void CSmashableDef::Destroy() { ++g_destroyed; }
struct CPhysImpl : CTCPhysicsBase {};
void CTCPhysicsBase::v00() {}
void CTCPhysicsBase::v01() {}
void CTCPhysicsBase::v02() {}
void CTCPhysicsBase::v03() {}
void CTCPhysicsBase::v04() {}
void CTCPhysicsBase::v05() {}
void CTCPhysicsBase::v06() {}
void CTCPhysicsBase::v07() {}
void CTCPhysicsBase::v08() {}
void CTCPhysicsBase::v09() {}
void CTCPhysicsBase::v10() {}
void CTCPhysicsBase::v11() {}
void CTCPhysicsBase::v12() {}
void CTCPhysicsBase::v13() {}
void CTCPhysicsBase::v14() {}
void CTCPhysicsBase::v15() {}
void CTCPhysicsBase::v16() {}
void CTCPhysicsBase::v17() {}
void CTCPhysicsBase::v18() {}
void CTCPhysicsBase::v19() {}
void CTCPhysicsBase::v20() {}
void CTCPhysicsBase::v21() {}
void CTCPhysicsBase::v22() {}
void CTCPhysicsBase::v23() {}
void CTCPhysicsBase::v24() {}
void CTCPhysicsBase::v25() {}
void CTCPhysicsBase::v26() {}
void CTCPhysicsBase::v27() {}
void CTCPhysicsBase::v28() {}
void CTCPhysicsBase::v29() {}
void CTCPhysicsBase::v30() {}
void CTCPhysicsBase::v31() {}
void CTCPhysicsBase::v32() {}
void CTCPhysicsBase::v33() {}
void CTCPhysicsBase::v34() {}
void CTCPhysicsBase::v35() {}
void CTCPhysicsBase::v36() {}
void CTCPhysicsBase::v37() {}
void CTCPhysicsBase::v38() {}
void CTCPhysicsBase::v39() {}
void CTCPhysicsBase::v40() {}
void CTCPhysicsBase::v41() {}
void CTCPhysicsBase::v42() {}
void CTCPhysicsBase::v43() {}
void CTCPhysicsBase::v44() {}
void CTCPhysicsBase::v45() {}
void CTCPhysicsBase::v46() {}
void CTCPhysicsBase::v47() {}
void CTCPhysicsBase::v48() {}
void CTCPhysicsBase::v49() {}
void CTCPhysicsBase::v50() {}
void CTCPhysicsBase::v51() {}
void CTCPhysicsBase::v52() {}
void CTCPhysicsBase::v53() {}
void CTCPhysicsBase::v54() {}
void CTCPhysicsBase::v55() {}
void CTCPhysicsBase::v56() {}
void CTCPhysicsBase::v57() {}
void CTCPhysicsBase::v58() {}
void CTCPhysicsBase::v59() {}
void CTCPhysicsBase::v60() {}
void CTCPhysicsBase::v61() {}
void CTCPhysicsBase::v62() {}
void CTCPhysicsBase::v63() {}
void CTCPhysicsBase::v64() {}
void CTCPhysicsBase::v65() {}
void CTCPhysicsBase::v66() {}
void CTCPhysicsBase::v67() {}
void CTCPhysicsBase::v68() {}
void CTCPhysicsBase::v69() {}
void CTCPhysicsBase::v70() {}
void CTCPhysicsBase::v71() {}

int main()
{
    static unsigned char physStore[sizeof(CPhysImpl)]; CPhysImpl* phys = new (physStore) CPhysImpl;
    static CThing parent; memset(&parent, 0, sizeof parent); parent.PhysicsTC = phys; parent.MapLevel = 3;
    static CThingObject made; memset(&made, 0, sizeof made);
    CTCSmashableComponent comp; memset(&comp, 0, sizeof comp);
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x4e, (CTCBase*)&comp } };
    made.m_interfaces.first = entries; made.m_interfaces.last = entries + 2;
    made.m_presentBits[0x4e >> 5] = 1UL << (0x4e & 31);
    static unsigned char defStore[sizeof(CSmashableDef)]; CSmashableDef* def = new (defStore) CSmashableDef;
    def->m_refCount = 1; def->ReplacementObject = 77;
    static unsigned char sStore[sizeof(CTCSmashableBase)]; CTCSmashableBase& s = *(CTCSmashableBase*)sStore; s.ParentThing = &parent; s.LastWorldFrameHit = 1234; s.Definition.m_pDef = def;
    g_createResult = &made;
    s.Smash();
    CHECK(g_creates == 1); CHECK(g_cDef == 77); CHECK(g_cPos == &g_pos); CHECK(g_cLevel == 3); CHECK(g_cFlags == 0);
    CHECK(g_cOrient.Up.Z == 1.0f && g_cOrient.Forward.Y == 1.0f); CHECK(g_cScript && g_cScript[0] == 0);
    CHECK(comp.LastWorldFrameHit == 1234); CHECK(g_killed == &parent); CHECK(g_killArg == true); CHECK(g_kills == 1);
    CHECK(def->m_refCount == 1); CHECK(g_destroyed == 0); CHECK(g_strCtors == 1 && g_strDtors == 1);
    // dead replacement: no hand-over
    comp.LastWorldFrameHit = 0; made.m_flags91 = 1; s.Smash(); CHECK(comp.LastWorldFrameHit == 0); CHECK(g_creates == 2);
    // no smashable component on the replacement
    made.m_flags91 = 0; made.m_presentBits[0x4e >> 5] = 0; s.Smash(); CHECK(comp.LastWorldFrameHit == 0);
    // creation failed
    g_createResult = 0; s.Smash(); CHECK(g_creates == 4); CHECK(g_kills == 4);
    // no replacement object
    def->ReplacementObject = 0; s.Smash(); CHECK(g_creates == 4); CHECK(g_kills == 5); CHECK(def->m_refCount == 1);
    if (fails == 0) printf("007eed20_TEST PASS\n"); else printf("007eed20_TEST FAIL\n");
    return fails;
}