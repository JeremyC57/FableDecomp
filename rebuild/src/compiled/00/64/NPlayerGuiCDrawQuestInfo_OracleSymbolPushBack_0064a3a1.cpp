// NPlayerGui::CDrawQuestInfo::OracleSymbolPushBack(const CCharString&) -- resolve the symbol to a sprite index and append it to the oracle-symbol list when it is a real sprite
// Leaf: ?OracleSymbolPushBack@CDrawQuestInfo@NPlayerGui@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064A3A1, length 38 bytes.
// Callees: CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString&) const @ 0x0064A37B, list<long>::push_back(const long&) @ 0x0042B687
// (on OracleSymbols +0x40; the body inserts before the head node = end(), so this is push_back; the sibling @ 0x0064A3C7 inserts before
// head->next = push_front).
// Names (NF95, 2026-11-19, queue q11225): moved off the row-local CDrawQuestInfo / CRetailLongList models onto
// engine/NPlayerGui_CDrawQuestInfo.h (opt-in FABLE_RETAIL_CDRAWQUESTINFO_ORACLESYMBOLPUSHBACK: Ego_r.pdb / FableWin.pdb spell
// GetScriptSpriteBySymbol public const, QBE) and stlport/fable_containers.h (push_back out of line, as retail calls 0x0042B687:
// FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE), so the calls bind ?GetScriptSpriteBySymbol@CDrawQuestInfo@NPlayerGui@@QBEJABVCCharString@@@Z
// and ?push_back@?$list@JV?$allocator@J@std@@@std@@QAEXABJ@Z.
// Shape: a frame-pointer size TU; the sprite index is a local whose address is passed on, and VC homes it in the dead `symbol` argument slot
// (`mov [ebp+8],eax ; lea eax,[ebp+8]`); `jle` skips non-positive indices.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CDRAWQUESTINFO_ORACLESYMBOLPUSHBACK
#include "engine/NPlayerGui_CDrawQuestInfo.h"

namespace NPlayerGui {

void CDrawQuestInfo::OracleSymbolPushBack(const CCharString& symbol)
{
    long sprite = GetScriptSpriteBySymbol(symbol);
    if (sprite > 0)
        OracleSymbols.push_back(sprite);
}

}
