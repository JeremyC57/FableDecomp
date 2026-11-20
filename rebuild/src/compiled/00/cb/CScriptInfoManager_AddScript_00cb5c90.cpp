// CScriptInfoManager::AddScript(const CScriptInfo& info, CCharString section) -- register a script: when it names a code section, register that section (short name, the script's name) with the code-section manager and remember it; then add the script to the sorted Scripts map
// Leaf: ?AddScript@CScriptInfoManager@@IAEXABVCScriptInfo@@VCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB5C90, 240 bytes. __fastcall, `ret 8`. FableWin.pdb
// ?AddScript@CScriptInfoManager@@IAEXABVCScriptInfo@@VCCharString@@@Z (protected). Manifest label "AddScript" is right.
// Callees: CCharString copy ctor 0x0099EC30, dtor 0x0099EAE0; CTBaseSingleton<CCodeSectionManager>::Get 0x00414CE0
// (static __fastcall, out of line in this TU); CCodeSectionManager::RegisterSection(CCharString, CCharString, bool,
// bool) 0x009F05A0 (FableWin `QAEKVCCharString@@0_N1@Z`); STLport vector<CCharString>::_M_insert_overflow 0x00433530;
// vector<pair<CCharString,CScriptInfo> >::push_back 0x00CB7210 (landed, the Scripts map's own vector at +4).
// Layout: CScriptInfoManager (Ego_r.pdb, retail 12-byte STLport vectors): CurrentScriptID +0, Scripts +4 (vector
// +4..+0xf, Compare +0x10, Dirty +0x11), CodeSectionNames +0x14.
// Shape: `section.GetLength() > 0` is the inline null-tested length read; the two by-value CCharString arguments are
// built right to left in their slots before the out-of-line Get(); push_back(section) is STLport's inline push_back
// (null-tested placement copy, else _M_insert_overflow with the __false_type temporary); the map's inline Add builds a
// local pair<CCharString,CScriptInfo>(key, value), pushes it, destroys it and sets Dirty. Add must be __forceinline
// (a plain inline member stays an out-of-line call: 148v228). With _M_insert_overflow declaration-only VC7.1 homes the
// __false_type temporary in the dead `info` slot (`lea edx,[esp+30h]`); retail homes it on the live by-value `section`
// slot (`lea edx,[esp+34h]`), which a visible body (never reading the tag) reproduces.
// Name fix (NF208, 2026-11-20, FP-VECTORINSERT q6332 / q6341): the row-local CCharString / CScriptInfo / pair / vector /
//   CVectorMap / CTBaseSingleton / CScriptInfoManager models are replaced by fable_string.h, stlport/fable_containers.h,
//   engine/CScriptInfoManager.h (engine/CVectorMap.h, engine/CScriptInfo.h) and engine/CTBaseSingleton.h; the map's inline add is
//   Ego_r.pdb CVectorMap::Insert(key, value) (FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE + _INSERT_VALUE_TYPE: the pair built as a
//   direct value_type temporary, forced inline), Get() the declaration-only FABLE_RETAIL_CTBASESINGLETON_GET_DECL; the header-less
//   CCodeSectionManager stays local (HEAD shape). push_back's overflow call now binds the STLport protected spelling of 0x00433530
//   (?_M_insert_overflow@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@IAE...): FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
//   keeps the real stl/_vector.c body visible but out of line (the tag slot above), and FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
//   (the self row 0x00433530's own macro) makes the pick-any copy it emits the owner's bytes.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "engine/CScriptInfoManager.h"
#define FABLE_RETAIL_CTBASESINGLETON_GET_DECL
#include "engine/CTBaseSingleton.h"

class CCodeSectionManager {
public:
    unsigned long RegisterSection(CCharString shortName, CCharString longName, bool loadNow, bool canUnload);   // 0x009F05A0
};

template <>
void std::vector<std::pair<CCharString, CScriptInfo> >::push_back(const std::pair<CCharString, CScriptInfo>& __x);   // 0x00CB7210

void CScriptInfoManager::AddScript(const CScriptInfo& info, CCharString section)
{
    if (section.GetLength() > 0) {
        CTBaseSingleton<CCodeSectionManager>::Get().RegisterSection(section, info.Name, false, false);
        CodeSectionNames.push_back(section);
    }
    Scripts.Insert(info.Name, info);
}
