// CTCHeroStats::GetMaxDrunkenness() const -- the hero-stats def's PintsToVomit (+0xA4), the top of the drunkenness scale
// Leaf: ?GetMaxDrunkenness@CTCHeroStats@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A89D, 18 bytes: `call 0x006869D0 ; mov eax,[eax+0DCh] ; fld dword ptr [eax+0A4h] ; ret`.
// Identity: Ego_r.pdb + FableWin.pdb ?GetMaxDrunkenness@CTCHeroStats@@QBEMXZ (public const; Ego_r donor 0x0056DE75 right after IsDrunk
// 0x0056DE4A, as retail 0x0057A89D after 0x0057A874).
// Callees: CTCBase::GetDefinitionManager 0x006869D0; CGameDefinitionManager::PeekHeroStatsDef (inline, +0xDC).
// Name fix (agentNF8 2026-11-11): was the free `float __fastcall CTCHeroStats_GetMaxDrunkenness(CTCHeroStats*)` over a local layout and
// an invented `CTCHeroStats_GetOwner` callee; now the PDB member on the shared headers, binding
// ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ. See RENAMES.tsv.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROSTATS_DRUNKENNESS
#include "engine/CTCHeroStats.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CGameDefinitionManager.h"

float CTCHeroStats::GetMaxDrunkenness() const
{
    return GetDefinitionManager().PeekHeroStatsDef().PintsToVomit;
}
