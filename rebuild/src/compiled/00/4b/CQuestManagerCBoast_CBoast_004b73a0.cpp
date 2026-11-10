// CBoast::CBoast(const CBoast& other) -- memberwise copy: id/cost/reward, the script name, the two wide strings, three flags, the group index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B73A0, length 91 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, CWideString::CWideString(const CWideString&) @ 0x0099B720 (x2).
// Layout from Ego_r.pdb (CBoast, 32 bytes): ID +0, Cost +4, Reward +8, ScriptName +0xc, Text +0x10,
// Title +0x14, Evil +0x18, Failed +0x19, Completed +0x1a, GroupIndex +0x1c. The copy constructor is
// written out of line (an implicit one would be inlined away and the TU would carry no symbol).
// Leaf: ??0CBoast@CQuestManager@@QAE@ABV01@@Z
// Name fix (agentNF5 2026-11-10): Ego_r.pdb ??0CBoast@CQuestManager@@QAE@ABV01@@Z (CBoast is nested in CQuestManager, `V` key; engine/
//   CQuestManager.h declares this copy constructor at 0x004B73A0), was the top-level stand-in ??0CBoast@@QAE@ABU0@@Z; now the shared
//   retail header.
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "engine/CQuestManager.h"
CQuestManager::CBoast::CBoast(const CBoast& other)
    : ID(other.ID), Cost(other.Cost), Reward(other.Reward), ScriptName(other.ScriptName)
    , Text(other.Text), Title(other.Title), Evil(other.Evil), Failed(other.Failed)
    , Completed(other.Completed), GroupIndex(other.GroupIndex)
{
}
