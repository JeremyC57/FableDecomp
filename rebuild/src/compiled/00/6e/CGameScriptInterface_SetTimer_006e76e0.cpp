// CGameScriptInterface::SetTimer(long timer, long value) -- update a registered script timer: look the timer up in the timer map
//   and, when it is registered, store the new value in its entry.
// Leaf: ?SetTimer@CGameScriptInterface@@UAEXJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E76E0, 44 bytes, thiscall, `ret 8`.
// Identity: Ego_r.pdb 0x00710980 / FableWin.pdb 0x02466E80 ?SetTimer@CGameScriptInterface@@UAEXJJ@Z (virtual, slot 89 of the
//   CGameScriptInterface vtable 0x01260F0C, engine/CGameScriptInterface.h); the map is Ego_r.pdb CGameScriptInterface::ScriptTimers
//   (map<long,long>, retail +0x3C), as in the sibling DeregisterTimer 0x006E7690.
// Callee: std::_Rb_tree<long, pair<const long,long>, _Select1st, less<long>, allocator>::find<long>(const long&) 0x005937E1 (STLport
//   4.6.1 _tree.h member template, owner decision 2026-11-03 D1; out of line, iterator through the hidden pointer), declared only
//   (FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL), so this TU emits no copy of the owner row's body.
// Name fix (NF185, 2026-11-20, q4385): was the July stand-in free function CGameScriptInterface_SetTimer over a local Tree::Find
//   model; now the PDB member on the shared header with the STLport map (FABLE_RETAIL_CGAMESCRIPTINTERFACE_SCRIPTTIMERS_MAP).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTINTERFACE_SCRIPTTIMERS_MAP
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL
#include "stlport/fable_map.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetTimer(long timer, long value)
{
    std::map<long, long>::iterator it = ScriptTimers.find(timer);
    if (it != ScriptTimers.end())
        it->second = value;
}
