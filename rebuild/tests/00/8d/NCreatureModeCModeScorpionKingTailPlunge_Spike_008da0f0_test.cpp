#include <stdio.h>
#include <string.h>
// NCreatureMode::CModeScorpionKingTailPlunge::Spike() -- the tail plunge lands: create the spike thing at the target position for the scorpion's player, trigger its type-0x7D component, and spawn the def's spike special effect there
// Leaf: Spike
// Shape: same-TU out-only stand-ins for GetDef<CThingObjectDef> (0x0042B040), GetSubDef<CSpecialEffectsDef> and
//   GetEffectFromString: no out slot escapes, so VC7.1 packs the name/temporaries at +10h/+14h and the GetDef out at +18h
//   and releases the def from one load (`dec [ecx+4] ; jne`) as retail.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA0F0, 0x1E7 bytes (padded to 496), `ret`.
// Callees: CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0 (static
//   __fastcall), CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CDefinitionManager::GetDef(long, CDefPointer*)
//   0x0042B040, CThingObject::Create 0x00703210 (static __fastcall), CVectorMap::LowerBound 0x0040F020 (inline lookup 0x7D),
//   the 0x7D component's 0x00788388 (no arguments), CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CreateEffectThing 0x006E0880 (__fastcall); def release slot 1.
// Layout: Ego_r.pdb CModeScorpionKingTailPlunge (packed): PCreature +0x84, bRecovering +0x88, TargetPos +0x89 (matches
//   retail); CModeBase ParentThing +4; the creature's player byte +0x90 (signed). The 0x1C-byte creation record built and
//   dropped at the top {0, 0, CCharString x2, false, true, true, false, 0, 0} is a local model (no PDB match pinned).
// Shape: as the landed StrikeResponseScorpionKing::Initialise 0x008C4B80 (stand-in GetSubDef / GetEffectFromString bodies
//   that only write their out slots) and ScorpionTailPlunge::PerformAction 0x008C4810 (def index from a scoped name).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class CRightHandedSet;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual ~CDefVirtual();                     // slot 0
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> struct CCountedDefPointer {
    T* m_p;
    CCountedDefPointer() : m_p(0) { }
    ~CCountedDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CThingObjectDef : public CDefBase { public: };
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                    // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);            // 0x009AD410
    bool GetDef(long globalIndex, CCountedDefPointer<CThingObjectDef>* out);   // 0x0042B040
};
void* FindDef_CDefinitionManager_GetDef(const void* manager, long globalIndex);   // stand-in callee
// Same-TU stand-in for the GetDef<CThingObjectDef> instantiation (0x0042B040, a template member compiled in the caller's
// TU): it writes the out slot only, so the def pointer's address does not escape.
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long globalIndex, CCountedDefPointer<CThingObjectDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef(this, globalIndex);
    if (!def)
        return false;
    out->m_p = (CThingObjectDef*)def;
    return true;
}
#pragma auto_inline(on)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CTCSpike {                               // interface type 0x7D
    void Trigger();                             // 0x00788388
};
class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                        // +0x70
    unsigned char  _pad74[0x90 - 0x74];
    signed char    Player;                      // +0x90
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
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* component = 0;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
};
class CThingCreatureBase : public CThing { public: };
class CThingObject : public CThing { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                           const CRightHandedSet* orientation, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct SCreationRecord {                        // 0x1c bytes
    long        A;
    long        B;
    CCharString Name1;
    CCharString Name2;
    bool        F0, F1, F2, F3;
    long        C;
    long        D;
    SCreationRecord(const CCharString& name1, const CCharString& name2)
        : A(0), B(0), Name1(name1), Name2(name2), F0(false), F1(true), F2(true), F3(false), C(0), D(0) { }
};

struct CModeBase {
    virtual ~CModeBase();
    CThing* ParentThing;                        // +0x04
};
namespace NCreatureMode {
#pragma pack(push, 1)
class CModeScorpionKingTailPlunge : public CModeBase { public:
    unsigned char       _pad08[0x84 - 0x08];
    CThingCreatureBase* PCreature;              // +0x84
    bool                bRecovering;            // +0x88
    C3DVector           TargetPos;              // +0x89
    void Spike();
};
#pragma pack(pop)

void CModeScorpionKingTailPlunge::Spike()
{
    long spikeIndex;
    {
        CCharString name("OBJECT_SCORPION_KING_SPIKE");
        spikeIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    SCreationRecord record("", "");
    CCountedDefPointer<CThingObjectDef> spikeDef;
    CGameDefinitionManager::Get().GetDef(spikeIndex, &spikeDef);
    C3DVector position = TargetPos;
    CThing* spike = CThingObject::Create(spikeIndex, position, PCreature->Player, 0, 0, "");
    spike->GetTCIfPresent<CTCSpike>(0x7d)->Trigger();
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    ParentThing->PDef->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        bool found = effects->GetEffectFromString("SCORPION_KING_SPIKE", effectType);
        if (found)
            CreateEffectThing(effectType, &position, false);
        if (--effects->m_refCount == 0)
            effects->Destroy();
    }
}
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static char g_log[512];
static void L(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static int g_strings, g_strDtors;
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; g_strings++; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; g_strings++; }
CCharString::~CCharString() { g_strDtors++; }
static int g_destroys;
CDefVirtual::~CDefVirtual() {}
void CDefVirtual::Destroy() { g_destroys++; }
static long g_lookup; static const char* g_lookupName;
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void*, const CCharString& name) { g_lookupName = (const char*)name.PStringData; L("lookup"); return g_lookup; }
static void* g_subDef;
void* Find_CDefClassBase_GetSubDef(const void*) { L("subdef"); return g_subDef; }
static void* g_objDef; static long g_getDefIndex;
void* FindDef_CDefinitionManager_GetDef(const void*, long globalIndex) { g_getDefIndex = globalIndex; L("getdef"); return g_objDef; }
static CGameDefinitionManager g_gdm;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_gdm; }
static const char* g_indexName;
long CGameDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { g_indexName = (const char*)name.PStringData; L("index"); return 31; }
static void* g_triggered;
void CTCSpike::Trigger() { g_triggered = this; L("trigger"); }
static CThing* g_spike; static long g_createIndex, g_createPlayer; static float g_createPos[3]; static const void* g_createOrient; static unsigned long g_createUid; static const char* g_createScript;
CThingObject* __fastcall CThingObject::Create(long defIndex, const C3DVector& position, long player, const CRightHandedSet* orientation, unsigned long uid, const CCharString& scriptName)
{ g_createIndex = defIndex; g_createPos[0] = position.X; g_createPos[1] = position.Y; g_createPos[2] = position.Z; g_createPlayer = player; g_createOrient = orientation; g_createUid = uid; g_createScript = (const char*)scriptName.PStringData; L("create"); return (CThingObject*)g_spike; }
static long g_effType; static float g_effPos[3]; static int g_effFlag;
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag) { g_effType = type; g_effPos[0] = position->X; g_effPos[1] = position->Y; g_effPos[2] = position->Z; g_effFlag = flag; L("effect"); return 0; }
CModeBase::~CModeBase() {}
static unsigned char g_modeBuf[0x100], g_parentBuf[0x100], g_creatureBuf[0x100], g_spikeBuf[0x100];
static void Reset() { g_log[0] = 0; g_strings = g_strDtors = g_destroys = 0; g_triggered = 0; }
using namespace NCreatureMode;
int main()
{
    CModeScorpionKingTailPlunge* mode = (CModeScorpionKingTailPlunge*)g_modeBuf;
    CThing* parent = (CThing*)g_parentBuf; mode->ParentThing = parent;
    CThingCreatureBase* creature = (CThingCreatureBase*)g_creatureBuf; creature->Player = -2; mode->PCreature = creature;
    mode->TargetPos.X = 1.0f; mode->TargetPos.Y = 2.0f; mode->TargetPos.Z = 3.0f;
    static CDefClassBase cdef; parent->PDef = &cdef;
    static CSpecialEffectsDef sdef; static CThingObjectDef odef; static CTCSpike spikeTC;
    g_spike = (CThing*)g_spikeBuf;
    CInterfaceMap::SEntry e[] = { { 0x10, 0 }, { 0x7d, (CTCBase*)&spikeTC } };
    g_spike->m_interfaces.first = e; g_spike->m_interfaces.last = e + 2; g_spike->m_presentBits[0x7d >> 5] = 1UL << (0x7d & 31);
    // full path: def found and released, spike triggered, effect created
    g_objDef = &odef; odef.m_refCount = 2; g_subDef = &sdef; sdef.m_refCount = 2; g_lookup = 9; Reset();
    mode->Spike();
    CHECK(strcmp(g_log, "index;getdef;create;trigger;subdef;lookup;effect;") == 0);
    CHECK(strcmp(g_indexName, "OBJECT_SCORPION_KING_SPIKE") == 0 && g_getDefIndex == 31 && g_createIndex == 31 && g_createPlayer == -2);
    CHECK(g_createPos[0] == 1.0f && g_createPos[1] == 2.0f && g_createPos[2] == 3.0f && g_createOrient == 0 && g_createUid == 0 && strcmp(g_createScript, "") == 0);
    CHECK(g_triggered == &spikeTC && strcmp(g_lookupName, "SCORPION_KING_SPIKE") == 0 && g_effType == 9 && g_effPos[2] == 3.0f && g_effFlag == 0);
    CHECK(odef.m_refCount == 1 && sdef.m_refCount == 1 && g_destroys == 0 && g_strings == g_strDtors);
    // effect lookup fails, last references released (both destroyed)
    odef.m_refCount = 1; sdef.m_refCount = 1; g_lookup = -1; Reset();
    mode->Spike();
    CHECK(strcmp(g_log, "index;getdef;create;trigger;subdef;lookup;") == 0 && g_destroys == 2 && g_strings == g_strDtors);
    // no def, no sub-def
    g_objDef = 0; g_subDef = 0; Reset();
    mode->Spike();
    CHECK(strcmp(g_log, "index;getdef;create;trigger;subdef;") == 0 && g_destroys == 0);
    if (fails == 0) printf("008da0f0_TEST PASS\n"); else printf("008da0f0_TEST FAIL\n");
    return fails;
}