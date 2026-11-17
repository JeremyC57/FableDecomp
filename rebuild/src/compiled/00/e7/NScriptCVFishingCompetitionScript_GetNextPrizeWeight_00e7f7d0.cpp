// NScript::CV_FishingCompetitionScript::GetNextPrizeWeight() -- the fish weight the hero must beat for the next prize: pGlobalScriptDef->FishPrizeWeights[PrizeNumber + 1], skipping one more entry while no prize has been set (PrizeNumber == -1)
// Leaf: ?GetNextPrizeWeight@CV_FishingCompetitionScript@NScript@@QAEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7F7D0, 27 bytes (+ int3 padding), __fastcall, float in st(0).
// Identity: its only caller 0x00E7F770 compares the result with a caught fish and then writes BiggestCompetitorFish (+0x70)
//   and Winner (+0x68) of CV_FishingCompetitionScript, so `this` is that script (Ego_r.pdb PrizeNumber +0x78 -> retail
//   +0x64, PDB - 0x14). The PDB names no such member (inlined in the debug build): descriptive name; the catalog's
//   "GetFloatTableEntryByIndex" is descriptive too.
// Data: pGlobalScriptDef @ 0x0143E90C; retail CScriptDef FishPrizeWeights (vector<float>) at +0x5d0, right after the landed
//   FishDefNames +0x5c4 (PDB +0x628: the retail def is 0x58 lower here, every earlier STLport vector is 4 bytes smaller).
//   Local model: no engine header for CScriptDef and its PDB offsets do not hold in retail.
// Shape: `next = PrizeNumber + 1; if (PrizeNumber == -1) ++next;` gives retail's `lea ecx,[eax+1]` before the compare and
//   the conditional `inc ecx` (the `prize == -1 ? 0 : prize` then `[prize + 1]` form folds the +1 into `fld [edx+eax*4+4]`
//   with an `xor eax,eax` arm); STLport `operator[]` is `*(begin() + n)`.
// agentNF60: std::vector is the shared STLport 4.6 vector (stlport/fable_containers.h; was a local namespace-std stand-in).
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)
namespace NScript {
class CV_FishingCompetitionScript {
public:
    unsigned char _pad00[0x64];
    long PrizeNumber;                              // +0x64
    float GetNextPrizeWeight();
};
float CV_FishingCompetitionScript::GetNextPrizeWeight()
{
    long next = PrizeNumber + 1;
    if (PrizeNumber == -1)
        ++next;
    return pGlobalScriptDef->FishPrizeWeights[next];
}
}