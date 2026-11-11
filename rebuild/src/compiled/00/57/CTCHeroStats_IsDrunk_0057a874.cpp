// CTCHeroStats::IsDrunk() const -- the hero's Drunkenness (+0x19C) has reached the hero-stats def's PintsToDrunk (+0xA0)
// Leaf: ?IsDrunk@CTCHeroStats@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A874, size TU.
// Identity: Ego_r.pdb + FableWin.pdb ?IsDrunk@CTCHeroStats@@QBE_NXZ (public const; Ego_r donor 0x0056DE4A, between OnHealthModified
// 0x0056DB3E and GetMaxDrunkenness 0x0056DE75 as retail 0x0057A874 between 0x0057A753 and 0x0057A89D).
// Callees: CTCBase::GetDefinitionManager 0x006869D0; CGameDefinitionManager::PeekHeroStatsDef (inline, +0xDC).
// Layout: retail STLport 4.6 header offsets (engine/CTCHeroStats.h Drunkenness +0x19c).
// Name fix (agentNF8 2026-11-11): was the free `bool __fastcall CTCHeroStats_IsDrunk(const CTCHeroStats*)` with an invented
// `extern "C" GetGlobal` callee; now the PDB member on the shared headers, binding
// ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ. See RENAMES.tsv.
#pragma optimize("s",on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROSTATS_DRUNKENNESS
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CTCHeroStats.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CGameDefinitionManager.h"

bool CTCHeroStats::IsDrunk() const
{
    return Drunkenness >= GetDefinitionManager().PeekHeroStatsDef().PintsToDrunk;
}
