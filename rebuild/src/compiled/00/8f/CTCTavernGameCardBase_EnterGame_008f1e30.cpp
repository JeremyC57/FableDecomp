// Flags: (none)
// Leaf: ?EnterGame@CTCTavernGameCardBase@@UAEXXZ
#define FABLE_RETAIL_CTCTAVERNGAME_CAMERAPOINT
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE          // count-store schedule of the expanded GetPackPosition (0x008F116B)
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_HIDEMESH
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDPACK
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_TYPENAME
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCARD_POSITIONS
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET
#define FABLE_RETAIL_STLP_VECTOR_RANGE_INSERT
#include "stlport/fable_containers.h"                
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardDef.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
#include "engine/CDefClassBase_GetSubDef.h"

// 0x008F0B20 (landed row; its body verbatim, same TU in retail)
// Fidelity: same-TU strong definition of 0x008F0B20 (retail TU-mate)
void CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* pdef)
{
    CDefPointer<const CTavernGameCardBaseDef> new_pdef;
    GetParentThing().PeekBaseDef().GetSubDef(&new_pdef);
    *pdef = new_pdef;
}

C3DVector CTCTavernGameCardBase::GetPackPosition()
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    return C3DVector(def->CardWidth * def->PackOffset_X, def->CardHeight * def->PackOffset_Y, 0.0f);
}

CCard::CCard(CThingObject* thing, long name, long value, long index, const C3DVector& pos)
{
    Thing = thing;
    Name = name;
    Value = value;
    FaceDown = false;
    FinalRotation = 0.0f;
    Turning = false;
    Moving = false;
    LocalPos = pos;
    Width = 0.0f;
    Height = 0.0f;
    Index = index;
}

void CTCTavernGameCardBase::EnterGame()
{
    CTCTavernGame::EnterGame();
    CDefPointer<const CTavernGameCardBaseDef> pdef;
    GetCardDef(&pdef);
    CardOrder = 0;
    for (long i = 0; i < pdef->CardPack.size(); ++i) {
        C3DVector base_offset(0.0f, 0.0f, (53-i) * pdef->CardSeparation + pdef->TableSeparation);
        base_offset += GetPackPosition();
        CCard* new_card = new CCard(CreateGamePiece(pdef->CardPack[i].CardName), pdef->CardPack[i].CardName,
                                    pdef->CardPack[i].CardVal, i, base_offset);
        new_card->MoveSpeed = pdef->MoveSpeed;
        new_card->TurnSpeed = pdef->TurnSpeed;
        PositionMesh(new_card->Thing, GamePositionPos, GamePositionRHSet, base_offset, 0.5f, 0.5f);
        new_card->FinalRotation = 0.5f;
        new_card->FaceDown = true;
        new_card->Width = pdef->CardWidth;
        new_card->Height = pdef->CardHeight;
        Pack.push_back(new_card);
    }
    ShuffleCards();
    CDefPointer<const CTavernGameDef> pbasedef;
    GetDef(&pbasedef);
    CameraUseCameraPoint(pbasedef->CameraName);
    Counter = 0.0f;
}