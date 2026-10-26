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

class CBaseObject;
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
class CCharStringData { public:
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
class CTCCarrying;
class CTCCarryable { public:                              // component kind 0x47
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