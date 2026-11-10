// CPersistContext::TransferMap<long,unsigned long>(const char* name, std::map<unsigned long,long>& map, const long& defaultValue) -- transfer a long-valued map keyed by unsigned long in the context's mode: text save (one "name[key] value;\r\n" line per entry), binary load / save through the map serialisers, clear, or text load ("name[key] value;" entries parsed from a copy of the parser, each replacing any entry for its key, the consumed text cleaned)
// Leaf: ??$TransferMap@JK@CPersistContext@@QAEXPBDAAV?$map@KJU?$less@K@std@@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@ABJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D8978, 754 bytes, thiscall `ret 0Ch`; size TU with an EBP frame.
// Identity: Ego_r.pdb / FableWin.pdb ??$TransferMap@JK@CPersistContext@@QAEXPBDAAV?$map@KJ...@std@@ABJ@Z (map<unsigned long,long>, the PDB
//   TransferMap<V,K> order; the catalog's <long,long> is wrong). The caller CTCInventoryStats::OnSerialise passes "CreaturesKilled2".
// Callees: CPersistContext::CheckCRC 0x00404500, SetAsFoundItem 0x009BAB40, PersistError 0x009BAA90, CleanOutLine 0x009BB0D0;
//   CStringParser copy ctor 0x0046381B / SkipPastWholeString 0x009B9C00 / ReadAsSymbol 0x009BA790 / ~CStringParser 0x004047A0;
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, operator+=(const char*) 0x0099F100, CBasicString<char>::
//   operator==(const char*) 0x004115A0; CPersistTraitsSLONG::TransferIn 0x009E2590 (__fastcall); CPersistTraits<unsigned long>::
//   TransferOut 0x005D8185, CPersistTraits<long>::TransferOut 0x005CC6C0; _Rb_global<bool>::_M_increment 0x004292C0 (cdecl);
//   map<unsigned long,long>: _Rb_tree::erase(const key_type&) 0x005D8730, insert_unique 0x0045CA09, clear 0x00451046;
//   GFSerialiseMapBinaryIn<long,unsigned long,less<unsigned long> > 0x0045C95A (mode 2, load stream +0x24) /
//   GFSerialiseMapBinaryOut 0x0045C91D (mode 3, save stream +0x28), __fastcall.
// Shape: the shared CPersistContext / CPersistTraits / CStringParser / STLport map headers. Three devices reproduce retail:
//   - CCharString's text operator== forced inline (FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ): this size TU expands all three
//     symbol tests (0x005D8A2B.., `repe cmpsb` / `call 0x004115A0`, the result in [ebp-1]);
//   - the generic CPersistTraits<T>::TransferOut body visible (FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT; still called out of line):
//     VC7.1 then knows the save loop's empty key and value traits are never read and homes both in the dead map slot [ebp+0Ch]
//     (0x005D8C23 / 0x005D8C40); declaration-only gives the key traits its own frame word (754 v 754, one operand);
//   - map::erase(const key_type&) with its STLport body visible (FABLE_RETAIL_STLP_TREE_ERASE_KEY), called out of line.
//   The text load needs no map::find: the old entry would only seed the value the long traits then overwrite.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_SETASFOUNDITEM
#define FABLE_RETAIL_GFSERIALISEMAPBINARY_DECL
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_MAP
#include <string.h>
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"

template<> void CPersistContext::TransferMap<long,unsigned long>(const char* name, std::map<unsigned long,long>& map, const long& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_SAVE_TEXT:
        for (std::map<unsigned long,long>::iterator it = map.begin(); it != map.end(); ++it) {
            CPersistTraits<unsigned long> keyTraits;
            CPersistTraits<long> valueTraits;
            *PStringToSaveTo += name;
            *PStringToSaveTo += "[";
            keyTraits.TransferOut(*this, *PStringToSaveTo, it->first);
            *PStringToSaveTo += "] ";
            valueTraits.TransferOut(*this, *PStringToSaveTo, it->second);
            *PStringToSaveTo += ";\r\n";
        }
        break;
    case MODE_LOAD_BINARY:
        GFSerialiseMapBinaryIn(*PLoadStream, map);
        break;
    case MODE_SAVE_BINARY:
        GFSerialiseMapBinaryOut(*PSaveStream, map);
        break;
    case MODE_CLEAR:
        map.clear();
        break;
    default: {
        CStringParser parser(*PParser);
        long nameLength = strlen(name);
        while (!Finished && parser.SkipPastWholeString(name)) {
            long start = parser.GetStringPos() - nameLength;
            if (parser.ReadAsSymbol() == "[") {
                unsigned long key;
                CPersistTraits<unsigned long>().TransferIn(*this, parser, key);
                if (parser.ReadAsSymbol() == "]") {
                    long value;
                    CPersistTraits<long>().TransferIn(*this, parser, value);
                    SetAsFoundItem();
                    map.erase(key);
                    map.insert(std::map<unsigned long,long>::value_type(key, value));
                    if (parser.ReadAsSymbol() == ";")
                        CleanOutLine(start, parser.GetStringPos());
                    else
                        PersistError(parser, "Unable to find terminating semicolon.");
                } else
                    PersistError(parser, "Unable to find terminating bracket.");
            } else
                PersistError(parser, "Unable to find opening bracket.");
        }
        break;
    }
    }
}