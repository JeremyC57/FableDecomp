// CIsHitEventHitByThingWithScriptName::operator()(const CHitEvent& hit) const -- true when the predicate's script name is empty, or when the hit's attacker is alive and carries that script name
// Grade: DIFFER(131v130) -- the inlined `ScriptName == ""` null path materialises `sete al` and re-tests, retail branches straight from `repe cmpsb` (je true / jmp on); also `add ecx,50h` vs `mov eax,[ecx+8] ; lea ecx,[eax+50h]` (fixed in probe i3 by a GetAttacker() accessor reading PRecord into a local)
// Leaf: ??RCIsHitEventHitByThingWithScriptName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00811100, length 130 bytes.
// Callees: CBasicString<char>::operator==(const char*) @ 0x004115A0 (this = the string block), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, the __fastcall string Compare(const char*, const char*) @ 0x00411570. Literal "" @ 0x0122D70E.
// Model: CIsHitEventHitByThingWithScriptName is one CCharString ScriptName (Ego_r.pdb, 4 bytes); the CCharString
// equalities as the landed CCharString::operator!= 0x0099E960 (`== const char*`: an unassigned string equals only "",
// the strcmp intrinsic over the pooled literal) and CScriptBase::OnRegionLoaded 0x00CB7B00 (`== CCharString`: same
// block, or both present, equal lengths and Compare == 0). The hit event's +8 record holds the attacker as an
// intelligent pointer at +0x50; the retail CThing keeps ScriptName at +0x74 and the dead bit at +0x91 (PDB +0x84 / +0xa1).
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                          // CBasicString<char>
    char*         PData;                           // +0x00
    unsigned long StringLength;                    // +0x04
    bool operator==(const char* text) const;       // 0x004115A0
};
#pragma pack(pop)
long __fastcall CompareStrings(const char* a, const char* b);   // 0x00411570
class CCharString {
public:
    CBasicStringChar* PStringData;                 // +0x00 (the shared block starts with its CBasicString)
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return *PStringData == text;
    }
    __forceinline bool operator==(const CCharString& other) const
    {
        CBasicStringChar* a = PStringData;
        CBasicStringChar* b = other.PStringData;
        if (a == b)
            return true;
        if (a && b && a->StringLength == b->StringLength)
            return CompareStrings(a->PData, b->PData) == 0;
        return false;
    }
};
struct CThing {
    unsigned char _pad00[0x74];
    CCharString   ScriptName;                      // +0x74
    unsigned char _pad78[0x91 - 0x78];
    unsigned char Dead : 1;                        // +0x91 bit 0
    unsigned char _flags91 : 7;
};
struct CBaseIntelligentPointer {
    CThing* GetPItem() const;                      // 0x00A01B50
};
struct CHitRecord {
    unsigned char           _pad00[0x50];
    CBaseIntelligentPointer PAttacker;             // +0x50
    CBaseIntelligentPointer* GetAttacker() { return &PAttacker; }
};
struct CHitEvent {
    unsigned char _pad00[8];
    CHitRecord*   PRecord;                         // +0x08
};
struct CIsHitEventHitByThingWithScriptName {
    CCharString ScriptName;                        // +0x00
    bool operator()(const CHitEvent& hit) const;
};

bool CIsHitEventHitByThingWithScriptName::operator()(const CHitEvent& hit) const
{
    if (ScriptName == "")
        return true;
    CThing* attacker = hit.PRecord->GetAttacker()->GetPItem();
    if (attacker && !attacker->Dead && attacker->ScriptName == ScriptName)
        return true;
    return false;
}
