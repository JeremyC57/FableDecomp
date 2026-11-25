// NScript::CRuntimeScriptedEntityPersist::LoadFromGameStateScript(const CCharString&) -- reset PersistScripts, then parse every START_NEW_ENTITY_SCRIPT <id> <script> END_NEW_ENTITY_SCRIPT block of the game-state text into it, re-sorting the map once if anything was added
// Leaf: LoadFromGameStateScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB8440, length 580 bytes, ret 4 (called by the landed
// CScriptBase::LoadGameState 0x00CB8690).
// Callees: vector erase(first, last) 0x00CBBDA0 and CVectorMap::Clear 0x00CBC660 (the inline Reset, out of line at
// 0x00CB80F0), CStringParser ctor 0x00404720 and its implicit inline dtor (~list 0x004046B0, two ~CCharString,
// ~CBaseClass 0x0099A2E0), SkipPastNextItemIfIs 0x009B9E00, ReadAsStringUntilString 0x009BA330, SkipUntilWhitespace
// 0x009B9CE0, ReadInt64 0x009BA5B0, SkipWhitespace 0x009B9C60, NextItemIs 0x009B9BA0, CCharString() 0x0099E4B0 /
// (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / operator= 0x0099EFB0 / dtor 0x0099EAE0, the STLport
// _M_insert_overflow 0x00CBC130 of push_back, the map's sort 0x00CBD370 (__fastcall(first, last, compare)).
// Literals: "" 0x0122D70E, "START_NEW_ENTITY_SCRIPT" 0x012C1674, "END_NEW_ENTITY_SCRIPT" 0x012C165C.
// Layout: Ego_r.pdb CRuntimeScriptedEntityPersist (20 bytes) = PersistScripts CVectorMap<unsigned __int64, CPersistInfo>:
// vector (retail 12 bytes) +0, Compare +0x0c, Dirty +0x0d; element pair<unsigned __int64, CPersistInfo{UID, PersistScript}>
// is 0x18 bytes.
// NF174 (q5066): ported off the local CVector<CEntry> / CStringParser / CCharString / std::list models onto stlport/fable_containers.h,
//   fable_string.h, engine/CStringParser.h and engine/CVectorMap.h (CKeyPairCompareLess): PersistScripts' vector is the STLport
//   vector<pair<unsigned __int64, CPersistInfo> >, so push_back's overflow binds the owner 0x00CBC130 spelling
//   ?_M_insert_overflow@?$vector@U?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@...@std@@IAEX...@Z (was the local
//   CVector<CEntry> stand-in), erase(first, last) the owner 0x00CBBDA0 spelling (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE) and the
//   sort the owner 0x00CBD370 spelling ??$sort@...CKeyPairCompareLess...@std@@YIX...@Z (FABLE_RETAIL_STLP_SORT_FASTCALL; was the
//   descriptive SortEntries). The owner class stays the row's pre-existing local NScript::CRuntimeScriptedEntityPersist model (the shared
//   header declares no PersistScripts); its Clear 0x00CBC660 keeps the local ?Clear@CMap@... spelling (no PDB name for it).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_READASUHUGE
#define FABLE_RETAIL_CSTRINGPARSER_SKIPUNTILWHITESPACE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CStringParser.h"
#include "engine/CVectorMap.h"                    // CKeyPairCompareLess (the PersistScripts map's compare)
// Literal (NF420, 2026-11-25): made-up data symbol(s) replaced by retail's pooled literal (/GF-merged ??_C@) at each use site:
//   g_startNewEntityScript (table ?g_startNewEntityScript@@3QBDB -> 0x012C1674) -> "START_NEW_ENTITY_SCRIPT"
//   g_endNewEntityScript (table ?g_endNewEntityScript@@3QBDB -> 0x012C165C) -> "END_NEW_ENTITY_SCRIPT"

namespace NScript {
struct CRuntimeScriptedEntityPersist {
    class CPersistInfo { public:
        unsigned __int64 UID;                       // +0x00
        CCharString      PersistScript;             // +0x08
    };
    typedef std::pair<unsigned __int64, CPersistInfo> CEntry;   // 0x18 bytes
    struct CMap {
        std::vector<CEntry> Data;                   // +0x00
        CKeyPairCompareLess<unsigned __int64, CPersistInfo> Compare;   // +0x0c
        bool     Dirty;                             // +0x0d
        void Clear();                               // 0x00CBC660
    };
    CMap PersistScripts;

    void LoadFromGameStateScript(const CCharString& text);
};
}

namespace {
// Descriptive TU-local helper (was the row's local CStringParser::SkipPastText inline member; no PDB name): the item string is a
//   named local destroyed after the SkipPastNextItemIfIs call (the HEAD row's shape; same bytes).
inline bool SkipPastText(CStringParser& parser, const char* text)
{
    CCharString item(text, -1);
    return parser.SkipPastNextItemIfIs(item);
}
}

void NScript::CRuntimeScriptedEntityPersist::LoadFromGameStateScript(const CCharString& text)
{
    PersistScripts.Data.erase(PersistScripts.Data.begin(), PersistScripts.Data.end());
    PersistScripts.Clear();
    CStringParser parser(text, 0, CCharString("", -1), 0);
    {
        CCharString start("START_NEW_ENTITY_SCRIPT", -1);
        parser.ReadAsStringUntilString(start);
    }
    while (SkipPastText(parser, "START_NEW_ENTITY_SCRIPT")) {
        parser.SkipUntilWhitespace();
        CPersistInfo info;
        info.UID = parser.ReadAsUHUGE();
        parser.SkipWhitespace();
        {
            CCharString end("END_NEW_ENTITY_SCRIPT", -1);
            info.PersistScript = parser.ReadAsStringUntilString(end);
        }
        PersistScripts.Data.push_back(CEntry(info.UID, info));
        PersistScripts.Dirty = true;
        {
            CCharString end("END_NEW_ENTITY_SCRIPT", -1);
            parser.SkipPastString(end);
        }
    }
    if (PersistScripts.Dirty) {
        std::sort(PersistScripts.Data.begin(), PersistScripts.Data.end(), PersistScripts.Compare);   // 0x00CBD370 (__fastcall)
        PersistScripts.Dirty = false;
    }
}
