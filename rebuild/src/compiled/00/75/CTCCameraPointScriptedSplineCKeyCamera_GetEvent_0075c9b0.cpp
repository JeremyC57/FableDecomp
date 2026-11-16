// CTCCameraPointScriptedSpline::CKeyCamera::GetEvent() const -- return the key's event name.
// Leaf: ?GetEvent@CKeyCamera@CTCCameraPointScriptedSpline@@QBE?AVCCharString@@XZ
// Name-fix NF38: FableWin.pdb ?GetEvent@CKeyCamera@CTCCameraPointScriptedSpline@@QBE?AVCCharString@@XZ, declared for 0x0075C9B0 by
// engine/CTCCameraPointScriptedSpline.h (EventName +0x30); the landed caller binds it. Was the local view CKeyCamera::GetEventName()
// returning a CRetailCharString stand-in.
// Retail: Fable.exe 0x0075C9B0, 22 bytes. The native caller iterates 0x38-byte keys and PDB
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CTCCameraPointScriptedSpline.h"

CCharString CTCCameraPointScriptedSpline::CKeyCamera::GetEvent() const { return EventName; }
