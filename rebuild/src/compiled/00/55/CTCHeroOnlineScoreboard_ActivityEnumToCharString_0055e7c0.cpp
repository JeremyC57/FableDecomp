// CTCHeroOnlineScoreboard::ActivityEnumToCharString(const EOnlineScoreboardActivity& activity) -- the Name of the activity's CActivity record
// (ActivityInfo[activity].Name), copied into the hidden result
// Leaf: ?ActivityEnumToCharString@CTCHeroOnlineScoreboard@@AAE?AVCCharString@@ABW4EOnlineScoreboardActivity@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055E7C0, 33 bytes, thiscall, `ret 8` (hidden result + the reference).
// Callee: CCharString::CCharString(const CCharString&) 0x0099EC30.
// NF39 (2026-11-16): moved off its `CNameTable_0055e7c0::GetEntryName` stand-in (catalog label C3DBoundingSphere::AccurateBuildFromPoints,
//   propagated) onto the shared engine/CTCHeroOnlineScoreboard.h (MEMBERS opt-in, which names 0x0055E7C0): FableWin.pdb 0x022E8910 + Ego_r.pdb
//   0x005D7870 ?ActivityEnumToCharString@CTCHeroOnlineScoreboard@@AAE?AVCCharString@@ABW4EOnlineScoreboardActivity@1@@Z (private); ActivityInfo
//   +0x28 is a vector of the 16-byte CActivity with Name at +0; the landed caller passes &activity (a key copy) and already binds this spelling.
// Defines: the landed 0x00563920 DisplayScoreboardGUI preamble (the MEMBERS opt-in and its chain).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
#include "engine/CTCHeroOnlineScoreboard.h"

CCharString CTCHeroOnlineScoreboard::ActivityEnumToCharString(const EOnlineScoreboardActivity& activity)
{
    return ActivityInfo[activity].Name;
}
