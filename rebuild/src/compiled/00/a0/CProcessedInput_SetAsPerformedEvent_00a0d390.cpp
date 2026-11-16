// CProcessedInput::SetAsPerformedEvent() -- mark the processed input as a performed event: no game events, type 2.
// Leaf: ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ
// NF37 (2026-11-16): moved off the free `Init(S*)` stand-in (catalog CWomanToAttract::Init, an ICF label) onto the shared
// engine/CProcessedInput.h (PERFORMED opt-in, which names 0x00A0D390; Ego_r.pdb ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ, Ego_r order
// AddGameEvent 0x00B74C50 / SetAsPerformedEvent 0x00B74CA0 / SetPlayer 0x00B74CB0 = retail 0x00A0D340 / 0x00A0D390 / 0x00A0D3A0; 37 landed
// callers bind the claim). Layout (Ego_r.pdb): Type +0x04, NoGameEvents +0xA8; retail stores NoGameEvents first.
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#include "engine/CGameEvent.h"
#include "engine/CProcessedInput.h"

void CProcessedInput::SetAsPerformedEvent()
{
    NoGameEvents = 0;
    Type = (EProcessedEventType)2;
}
