#ifndef FABLE_ENGINE_CTCACTIONUSEACTIVATEQUEST_H
#define FABLE_ENGINE_CTCACTIONUSEACTIVATEQUEST_H
// New header (chunk 2039, row 0x006ACA30 CThingPlayerCreature::OnCollisionWithObject): the quest-activating action-use component,
// derived from CTCActionUseBase (interface TCI_ON_ACTION_USE, inherited StatGetInterfaceType). Ego_r.pdb / FableWin.pdb
// ?StatGetClassTypeName@CTCActionUseActivateQuest@@SI?AVCCharString@@XZ: out of line in the TC registry TU (retail 0x004D57FA:
// `push -1 ; push "CTCActionUseActivateQuest" ; ... call CCharString::CCharString(const char*, long)` into the hidden result) and
// expanded in CThing::HasTC<CTCActionUseActivateQuest> (retail's unreferenced COMDAT copy 0x006AE680 and the row 0x006ACA30 build
// the literal string in place), so the body is in the class. Needs FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CCHARSTRING.
#include "fable_string.h"
#include "engine/CTCActionUseBase.h"
class CTCActionUseActivateQuest : public CTCActionUseBase {
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCActionUseActivateQuest"); }
};
#endif
