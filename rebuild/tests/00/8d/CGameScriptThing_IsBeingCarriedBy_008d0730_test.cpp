// Behaviour test for 008d0730: src/008d0730.cpp copied verbatim; a script thing is "carried by" a name when its live thing
// holds a carryable component (kind 0x47) with a carrying component and the name is empty or equals the carrier's script name.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGameScriptThing::IsBeingCarriedBy(const CCharString& name) const -- true when this live thing is being carried (its carryable component, kind 0x47, has a carrying component) and either no name is given or the carrier's script name equals `name`
// Leaf: IsBeingCarriedBy
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D0730, 233 bytes, `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3 on PThing), the const component-map LowerBound @
//   0x004365B0, CBasicString<char>::operator==(const char*) @ 0x004115A0 (inside CCharString == ""), CTCCarryable::
//   GetCarrier @ 0x00705170 (label "CGuiControlListBox::GetCurrentSelectedEntryData" is propagated), Compare @
//   0x00411570 (inside the inlined CCharString ==). Literal "" @ 0x0122D70E.
// Layout: engine/CGameScriptThing.h (PThing CIntelligentPointer<CThing> +0x0c); retail CThing: component bitset +0x20
//   (kind 0x47 = bit 7 of +0x28), component map +0x44, ScriptName +0x74 (Ego_r.pdb +0x84, retail 0x10 lower), Dead bit
//   +0x91; CTCCarryable (Ego_r.pdb) PCarryingTC +0x10.
// Shape: the alive / component / carried tests are nested ifs sharing the final `xor al,al` exit; the component is a
//   named local (ebx); the name test is one `return name == "" || name == carrier->ScriptName;` (int-style 1/0 exits).
//   The inline CCharString ==("") writes its null-data case as `if (strcmp(text, "") == 0) return true; return false;`
//   so VC threads the intrinsic strcmp's flags straight into the `||` (a plain `return strcmp(...) == 0;` merges a
//   `sete al`); the two-level CCharString == (identity, null tests, length, Compare) then loads Compare's edx first.
#include <string.h>
#include "engine/CGameScriptThing.h"

struct CBaseObject;
#pragma pack(push, 1)
struct CBasicStringChar {                          // CBasicString<char>, 13 bytes
    char*         PData;                           // +0x00
    unsigned long StringLength;                    // +0x04
    unsigned long DataLength : 31;                 // +0x08
    bool          UseFastExtend : 1;               // +0x0c
    bool operator==(const char* text) const;       // 0x004115A0
};
#pragma pack(pop)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
struct CCharStringData {
    CBasicStringChar Data;                         // +0x00 (text, length)
};
class CCharString {
public:
    CCharStringData* PStringData;                  // +0x00
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    __forceinline bool SameText(const CCharString& other) const
    {
        if (PStringData->Data.StringLength != other.PStringData->Data.StringLength)
            return false;
        return Compare_00411570(PStringData->Data.PData, other.PStringData->Data.PData) == 0;
    }
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return SameText(other);
        return false;
    }
};

struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThingRetail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x24];
    CCharString    ScriptName;                     // +0x74
    unsigned char  _pad78[0x91 - 0x78];
    unsigned char  Dead : 1;                       // +0x91 bit 0
    unsigned char  _bits91 : 7;
    __forceinline bool HasTC(long kind) const
    {
        if (m_tcMask[kind >> 5] & (1UL << (kind & 31)))
            return true;
        return false;
    }
};
struct CTCCarrying;
struct CTCCarryable {                              // component kind 0x47
    unsigned char _base[0x10];                     // CTCBase {vptr, ParentThing, flags} + ForceFragility
    CTCCarrying*  PCarryingTC;                     // +0x10
    CThingRetail* GetCarrier() const;              // 0x00705170
};
struct CBaseIntelligentPointer {
    void* __vftable;
    void* PData;
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};

struct CGameScriptThingMethods : CGameScriptThing {
    CThingRetail* GetThing() const { return (CThingRetail*)((const CBaseIntelligentPointer*)PThing)->GetPItem(); }
    bool IsBeingCarriedBy(const CCharString& name) const;
};

bool CGameScriptThingMethods::IsBeingCarriedBy(const CCharString& name) const
{
    if (GetThing()) {
        if (!GetThing()->Dead) {
            CThingRetail* thing = GetThing();
            if (thing->HasTC(0x47)) {
                CTCCarryable* carryable = (CTCCarryable*)thing->m_tcs.Find(0x47);
                if (carryable->PCarryingTC)
                    return name == "" || name == carryable->GetCarrier()->ScriptName;
            }
        }
    }
    return false;
}
// ---- end source ----
static CThingRetail* g_thing;
static CThingRetail* g_carrier;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)g_thing; }
STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind) const
{
    STCEntry_Retail* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind)
        ++it;
    return it;
}
bool CBasicStringChar::operator==(const char* text) const { return strcmp(PData, text) == 0; }
CThingRetail* CTCCarryable::GetCarrier() const { return g_carrier; }
long __fastcall Compare_00411570(const char* a, const char* b) { return strcmp(a, b); }

static CCharStringData MakeData(char* text)
{
    CCharStringData d;
    memset(&d, 0, sizeof(d));
    d.Data.PData = text;
    d.Data.StringLength = (unsigned long)strlen(text);
    return d;
}
int main()
{
    static char tBob[] = "Bob", tBob2[] = "Bob", tBen[] = "Ben", tAnna[] = "Anna", tEmpty[] = "";
    CCharStringData dBob = MakeData(tBob), dBob2 = MakeData(tBob2), dBen = MakeData(tBen), dAnna = MakeData(tAnna), dEmpty = MakeData(tEmpty);
    CTCCarrying* carrying = (CTCCarrying*)&dBob;          // any non-null pointer
    CTCCarryable carryable; memset(&carryable, 0, sizeof(carryable)); carryable.PCarryingTC = carrying;
    STCEntry_Retail entries[3] = { { 0x10, 0 }, { 0x47, (CTCBase_Retail*)&carryable }, { 0x60, 0 } };
    CThingRetail thing; memset(&thing, 0, sizeof(thing));
    thing.m_tcMask[0x47 >> 5] = 1UL << (0x47 & 31);
    thing.m_tcs.m_pFirst = entries; thing.m_tcs.m_pLast = entries + 3; thing.m_tcs.m_pEnd = entries + 3;
    CThingRetail carrier; memset(&carrier, 0, sizeof(carrier)); carrier.ScriptName.PStringData = &dBob;
    g_carrier = &carrier;
    CGameScriptThingMethods script; memset(&script, 0, sizeof(script));
    CCharString none = { 0 }, empty = { &dEmpty }, bob = { &dBob2 }, bobSame = { &dBob }, ben = { &dBen }, anna = { &dAnna };

    g_thing = 0; CHECK(script.IsBeingCarriedBy(none) == false);
    g_thing = &thing;
    CHECK(script.IsBeingCarriedBy(none) == true);
    CHECK(script.IsBeingCarriedBy(empty) == true);
    CHECK(script.IsBeingCarriedBy(bob) == true);
    CHECK(script.IsBeingCarriedBy(bobSame) == true);
    CHECK(script.IsBeingCarriedBy(ben) == false);
    CHECK(script.IsBeingCarriedBy(anna) == false);
    carrier.ScriptName.PStringData = 0;
    CHECK(script.IsBeingCarriedBy(bob) == false);
    CHECK(script.IsBeingCarriedBy(none) == true);
    carrier.ScriptName.PStringData = &dBob;
    carryable.PCarryingTC = 0; CHECK(script.IsBeingCarriedBy(none) == false); carryable.PCarryingTC = carrying;
    thing.m_tcMask[2] = 0; CHECK(script.IsBeingCarriedBy(none) == false); thing.m_tcMask[2] = 1UL << 7;
    thing.Dead = 1; CHECK(script.IsBeingCarriedBy(bob) == false); thing.Dead = 0;
    CHECK(script.IsBeingCarriedBy(bob) == true);
    if (fails == 0) printf("008d0730_TEST PASS\n"); else printf("008d0730_TEST FAIL\n");
    return fails;
}