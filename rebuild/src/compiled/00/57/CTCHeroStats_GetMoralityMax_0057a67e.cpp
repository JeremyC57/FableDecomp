// CTCHeroStats::GetMoralityMax() const -- the hero-stats def's MaxMorality (+0xD0)
// Leaf: ?GetMoralityMax@CTCHeroStats@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A67E, 18 bytes: `call 0x006869D0 ; mov eax,[eax+0DCh] ; mov eax,[eax+0D0h] ; ret`.
// Identity: Ego_r.pdb + FableWin.pdb ?GetMoralityMax@CTCHeroStats@@QBEJXZ (public const; engine/CTCHeroStats.h
// FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX; Ego_r donor 0x0056DA4B).
// Callees: CTCBase::GetDefinitionManager 0x006869D0; CGameDefinitionManager::PeekHeroStatsDef (inline, +0xDC).
// Name fix (agentNF8 2026-11-11): was the free `long __fastcall CTCHeroStats__GetMoralityMax(CTCHeroStats*)` with an invented
// `CTCHeroStats_GetHelper` callee and raw offsets; now the PDB member on the shared headers, binding
// ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ. See RENAMES.tsv.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX
#include "engine/CTCHeroStats.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CGameDefinitionManager.h"

long CTCHeroStats::GetMoralityMax() const
{
    return GetDefinitionManager().PeekHeroStatsDef().MaxMorality;
}
