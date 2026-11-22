// Locate the game board and project its dummy range onto the local axes.
// Leaf: ?OnInitialActivate@CTCTavernGameShoveHaPenny@@UAEXXZ
// Flags:
// Bracket: the shared grouped cross expression and the donor Initialise subtraction place the x87 operands at 0x007D06C9 and 0x007D0818.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#include "fable_string.h"
#include "fable_assert.h"
#include "engine/CTCTavernGameShoveHaPenny.h"
#include "engine/CTavernGameShoveHaPennyDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThing.h"
#include "engine/CTCThingOwner.h"
#include "engine/ThingAlive.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

void CTCTavernGameShoveHaPenny::OnInitialActivate()
{
    CTCTavernGame::OnInitialActivate();
    CDefPointer<const CTavernGameShoveHaPennyDef> pdef;
    GetParentThing().PeekBaseDef().GetSubDef(&pdef);
    CThing* board = 0;
    const CTCThingOwner* owner = 0;
    if (ParentThing->PeekTC(&owner)) {
        const std::list<CIntelligentPointer<CThing> >& children = owner->GetObjectList();
        for (std::list<CIntelligentPointer<CThing> >::const_iterator i = children.begin(); i != children.end(); ++i) {
            CThing* thing = i->Get();
            if (IS_THING_ALIVE(thing) && thing->GetDefGlobalIndex() == pdef->BoardDefIndex) {
                board = thing;
                break;
            }
        }
    }
    FABLE_RETAIL_ASSERT(board);
    if (board) {
        const NGraphicAppearance::CTCGraphicAppearance* graphic_appearance = board->GetTCGraphicAppearance();
        C3DVector min, max;
        CRightHandedSet rhset;
        graphic_appearance->GetDummyObject("DUMMY_OBJECT_MIN", 0, 0, min, rhset, 1.0f);
        graphic_appearance->GetDummyObject("DUMMY_OBJECT_MAX", 0, 0, max, rhset, 1.0f);
        C3DVector size = max - min;
        BoardPos = min;
        BoardRHSet = rhset;
        BoardSize = C3DVector(rhset.GetRight().Dot(size), rhset.GetForward().Dot(size), rhset.GetUp().Dot(size));
        if (BoardSize.X < 0.0f) {
            BoardPos += rhset.GetRight() * BoardSize.X;
            BoardSize.X = -BoardSize.X;
        }
        if (BoardSize.Y < 0.0f) {
            BoardPos += rhset.GetForward() * BoardSize.Y;
            BoardSize.Y = -BoardSize.Y;
        }
        if (BoardSize.Z < 0.0f) {
            BoardPos += rhset.GetUp() * BoardSize.Z;
            BoardSize.Z = -BoardSize.Z;
        }
        BoardPos.Z += BoardSize.Z;
    }
}