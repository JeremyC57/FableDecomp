#include "engine/CGameScriptInterface.h"
#include <cstdio>

enum ETCInterfaceType { ETC_DUMMY = 0 };

class CScriptThing;
struct CThingOverlay;

struct CScriptThingVTable
{
    unsigned char m_Pad00[0x2c];
    CThingOverlay* (__fastcall* m_Resolve)(CScriptThing* self);
    unsigned char m_Pad30[0xfc];
    bool (__fastcall* m_IsAvailable)(CScriptThing* self);
};

class CScriptThing
{ public:
    CScriptThingVTable* m_pVTable;
};

class CGSIPickLockableDoor
{
public:
    bool IsLockPickable();
};

class CTCBase;
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> CKeyPair;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    CKeyPair* m_pBegin;
    CKeyPair* m_pEnd;
    const CKeyPair* LowerBound(const ETCInterfaceType& pKey) const;
};

struct CThingOverlay
{
    unsigned char m_Pad00[0x24];
    unsigned int  m_Flags24;
    unsigned char m_Pad28[0x1c];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >    m_InterfaceMap44;
};


static bool g_available = false;
static CThingOverlay* g_thing = 0;
static unsigned long g_available_calls = 0;
static unsigned long g_resolve_calls = 0;
static CScriptThing* g_observed_script_thing = 0;
static CKeyPair* g_lower_bound_result = 0;
static CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* g_observed_map = 0;
static ETCInterfaceType g_observed_key = ETC_DUMMY;
static bool g_pickable = false;
static CGSIPickLockableDoor* g_observed_door = 0;
static unsigned long g_pickable_calls = 0;

static bool __fastcall IsAvailable(CScriptThing* self)
{
    g_observed_script_thing = self;
    ++g_available_calls;
    return g_available;
}

static CThingOverlay* __fastcall Resolve(CScriptThing* self)
{
    g_observed_script_thing = self;
    ++g_resolve_calls;
    return g_thing;
}

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x26;

const CKeyPair* CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::LowerBound(const ETCInterfaceType& pKey) const
{
    g_observed_map = const_cast<CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >*>(this);
    g_observed_key = pKey;
    return g_lower_bound_result;
}

bool CGSIPickLockableDoor::IsLockPickable()
{
    g_observed_door = this;
    ++g_pickable_calls;
    return g_pickable;
}

bool __fastcall CGameScriptInterface_IsEntityPickLockable(
    const CGameScriptInterface* self,
    int edxUnused,
    CScriptThing* pScriptThing);

static void ResetObservations()
{
    g_available_calls = 0;
    g_resolve_calls = 0;
    g_observed_script_thing = 0;
    g_lower_bound_result = 0;
    g_observed_map = 0;
    g_observed_key = ETC_DUMMY;
    g_observed_door = 0;
    g_pickable_calls = 0;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_0089a2d0_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    CGameScriptInterface gsi;
    CScriptThingVTable vtable = {};
    CScriptThing scriptThing = {};
    CThingOverlay thing = {};
    CGSIPickLockableDoor foundDoor;
    CGSIPickLockableDoor endDoor;
    CKeyPair foundEntry = {};
    CKeyPair greaterEntry = {};
    CKeyPair endEntry = {};

    vtable.m_Resolve = Resolve;
    vtable.m_IsAvailable = IsAvailable;
    scriptThing.m_pVTable = &vtable;
    g_thing = &thing;
    thing.m_InterfaceMap44.m_pEnd = &endEntry;
    foundEntry.second = (CTCBase*) &foundDoor;
    endEntry.second = (CTCBase*) &endDoor;

    ResetObservations();
    g_available = false;
    if (CGameScriptInterface_IsEntityPickLockable(
            &gsi, 0, &scriptThing) ||
        g_available_calls != 1 ||
        g_resolve_calls != 0 ||
        g_observed_script_thing != &scriptThing)
        return Fail("unavailable script thing");

    ResetObservations();
    g_available = true;
    thing.m_Flags24 = 0;
    if (CGameScriptInterface_IsEntityPickLockable(
            &gsi, 0, &scriptThing) ||
        g_resolve_calls != 1 ||
        g_observed_map != 0 ||
        g_pickable_calls != 0)
        return Fail("missing door capability");

    ResetObservations();
    thing.m_Flags24 = 0x40;
    foundEntry.first = (ETCInterfaceType)0x26;
    g_lower_bound_result = &foundEntry;
    g_pickable = true;
    if (!CGameScriptInterface_IsEntityPickLockable(
            &gsi, 0, &scriptThing) ||
        g_observed_map != &thing.m_InterfaceMap44 ||
        (int)g_observed_key != 0x26 ||
        g_observed_door != &foundDoor ||
        g_pickable_calls != 1)
        return Fail("pickable door");

    ResetObservations();
    g_lower_bound_result = &foundEntry;
    g_pickable = false;
    if (CGameScriptInterface_IsEntityPickLockable(
            &gsi, 0, &scriptThing) ||
        g_pickable_calls != 1)
        return Fail("non-pickable door");

    ResetObservations();
    greaterEntry.first = (ETCInterfaceType)0x27;
    greaterEntry.second = (CTCBase*) &foundDoor;
    g_lower_bound_result = &greaterEntry;
    g_pickable = true;
    if (!CGameScriptInterface_IsEntityPickLockable(
            &gsi, 0, &scriptThing) ||
        g_observed_door != &endDoor)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_lower_bound_result = &endEntry;
    if (!CGameScriptInterface_IsEntityPickLockable(
            &gsi, 0, &scriptThing) ||
        g_observed_door != &endDoor)
        return Fail("direct sentinel");

    std::printf("CGameScriptInterface_0089a2d0_TEST PASS\n");
    return 0;
}
