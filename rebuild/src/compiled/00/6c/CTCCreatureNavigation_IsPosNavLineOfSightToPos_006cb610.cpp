// CTCCreatureNavigation::IsPosNavLineOfSightToPos(const C3DVector&, const C3DVector&) const -- ask the active navigator whether the straight line between two positions is navigable
// Leaf: ?IsPosNavLineOfSightToPos@CTCCreatureNavigation@@QBE_NABVC3DVector@@0@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CB610, 8 bytes: `mov ecx,[ecx+0Ch] ; mov eax,[ecx] ; jmp [eax+28h]` (PNavigator slot 10).
// Identity (two sources): Ego_r.pdb ?IsPosNavLineOfSightToPos@CTCCreatureNavigation@@QBE_NABVC3DVector@@0@Z (two const C3DVector&
//   arguments forwarded in place to CNavigatorBase slot 10 IsNavLineOfSightClear(const C3DVector&, const C3DVector&) const), placed in
//   the CTCCreatureNavigation TU just before IsAbleToNavigateFromPosToPos 0x006CB620 (landed), and the landed caller 0x00864620
//   already binds this exact spelling (the catalog's "IsNavLineOfSight" and the 0x006A7640 caller's
//   ?IsNavLineOfSight@CTCCreatureNavigation@@QAE_NPBVC3DVector@@ABV2@@Z are invented: NAME_FIXES.tsv).
// Header: engine/CTCCreatureNavigation.h (FABLE_RETAIL_CTCCREATURENAVIGATION_LINEOFSIGHT, new), engine/CNavigatorBase.h.
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_LINEOFSIGHT
#include "engine/CNavigatorBase.h"
#include "engine/CTCCreatureNavigation.h"

bool CTCCreatureNavigation::IsPosNavLineOfSightToPos(const C3DVector& from, const C3DVector& to) const
{
    return PNavigator->IsNavLineOfSightClear(from, to);
}