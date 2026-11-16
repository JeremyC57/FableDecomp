// CTCTavernGameCoinGolf::GetCamera(C3DVector& camPos, C3DVector& lookAt) -- overview camera orbiting the table by the Z angle; once the coin is live, a chase camera behind and above the coin, pitched and turned by the player's angles
// Leaf: ?GetCamera@CTCTavernGameCoinGolf@@QAEXAAVC3DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D3560, 416 bytes (+ int3 padding), `ret 8`.
// Callees: C3DVector rotate-about-Z 0x00A14040 (landed spelling, 0x007A9960) and the two-angle rotate 0x00A14260;
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (inline IS_THING_ALIVE + the position fetch); CThing::GetPos 0x004C73D0.
// Constants: 0.05f/1.75f immediates, 0.1f 0x0122EE58, 0.9f 0x0126F3C8, 0.25f 0x0123078C, 0.2f 0x012305A0, 0.025f 0x0126F42C.
// Layout: engine/CTCTavernGameCoinGolf.h / CTCTavernGameCoinBase.h retail branch: ZAngle +0x148 / XAngle +0x14c; the base's Held
//   (Coin*, +0x108; Coin::Thing at the odd offset +0x25) and BoardPos (+0x10c) -- Ego_r.pdb CTCTavernGameCoinBase member names.
// Shape: C3DVector with a user copy constructor (memberwise register copies); the offset is added in place then copied
//   out; the coin camera is a temporary `C3DVector(0, y, 0)` assigned to camPos before the two rotations.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_ROTATE
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_PRESSED_ACCESS
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_EVENTINQUEUE_ACCESS
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#define FABLE_RETAIL_CTAVERNGAMECOINGOLFDEF_SUBDEF
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCoinGolfDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCoinBase.h"
#include "engine/CTCTavernGameCoinGolf.h"
#include "engine/ThingAlive.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "fable_game.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessTavernGame.h"
#include <math.h>

void CTCTavernGameCoinGolf::GetCamera(C3DVector& camPos, C3DVector& lookAt)
{
    C3DVector centre(BoardPos);
    C3DVector offset(0.0f, 0.05f, 1.75f);
    offset.RotateXY(ZAngle);
    offset += BoardPos;
    camPos = offset;
    lookAt = centre;
    if (Held && IS_THING_ALIVE(Held->Thing)) {
        lookAt = Held->Thing->GetPos();
        lookAt.Z += 0.1f;
        camPos = C3DVector(0.0f, XAngle * 0.9f + 0.25f, 0.0f);
        camPos.Rotate(0.0f, XAngle * 0.2f + 0.025f);
        camPos.Rotate(ZAngle, 0.0f);
        camPos += lookAt;
    }
}
