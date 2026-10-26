// CPersistTraits<CDefIndex>::TransferOut(CPersistContext*, CCharString*, const CDefIndex*) -- write the definition's name for a positive index, "NULL" when the name is empty, through the CCharString traits
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3E180, 195 bytes, `ret 0Ch` (__fastcall, the empty traits `this` is unused).
// Callees: CCharString::CCharString() @ 0x0099E4B0 (x2), CDefinitionManager::GetDefNameFromGlobalIndex(long) @ 0x009ACCC0
//   (CDefString by hidden pointer) on context->PDefinitionManager (+0), CDefStringTable::GetString(long) @ 0x009D49B0 on
//   g_DefStringTable 0x013CA828, CCharString::operator=(const CCharString&) @ 0x0099EFB0, CBasicString<char>::operator==
//   (const char*) @ 0x004115A0 (on the string data), CCharString::operator=(const char*) @ 0x0099EFE0,
//   CPersistTraits<CCharString>::TransferOut @ 0x0040E630, ~CCharString @ 0x0099EAE0. Strings: "" 0x0122D70E,
//   "NULL" 0x012393E4.
// Layout: PDB CDefIndex { long Val; }, CDefString { long TablePos; }; CCharString's data pointer (null for the empty
//   string) carries the CBasicString compare.
// Shape: the CCharString == "" test is the inline operator of the landed 0x00463334 / 0x00A63EE0 models: a null data
//   pointer compares the literal with "" through the strcmp intrinsic (`repe cmpsb` over the pooled "" twice), else
//   the CBasicString operator== (written as nested ifs, which keeps retail's je/jmp arms instead of a sete). The
//   manager in a named local makes VC7.1 load it into ecx before the arguments, as retail; the name lookup goes through
//   an inline CDefString::GetString() (its result address is re-formed with `lea` for the assignment).
//   Speed-optimised (default) TU.
#include <string.h>
#pragma intrinsic(strcmp)
class CCharStringData;
class CStringParser;
struct CBasicStringChar {
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharString { public:
    CBasicStringChar* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator=(const char* text);           // 0x0099EFE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return *PStringData == text;
    }
};

struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
class CDefString { public:
    long TablePos;                                      // +0x00 (PDB)
    CDefString() {}
    CCharString GetString() const { return g_DefStringTable.GetString(TablePos); }
};
class CDefIndex { public:
    long Val;                                           // +0x00 (PDB)
};
class CDefinitionManager { public:
    CDefString GetDefNameFromGlobalIndex(long index);   // 0x009ACCC0
};
class CPersistContext { public:
    CDefinitionManager* PDefinitionManager;             // +0x00
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CCharString> {
    CCharString DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, CCharString* value);   // 0x0040E630
};
template<> struct CPersistTraits<CDefIndex> {
    void TransferOut(CPersistContext* context, CCharString* out, const CDefIndex* value);
};

void CPersistTraits<CDefIndex>::TransferOut(CPersistContext* context, CCharString* out, const CDefIndex* value)
{
    CPersistTraits<CCharString> traits;
    CCharString name;
    if (value->Val > 0) {
        CDefinitionManager* manager = context->PDefinitionManager;
        name = manager->GetDefNameFromGlobalIndex(value->Val).GetString();
    }
    if (name == "")
        name = "NULL";
    traits.TransferOut(context, out, &name);
}