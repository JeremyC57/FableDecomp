// NUISystem::CClickable::GetDataFromDef() -- load click actions and timing from the UI definition.
// Leaf: ?GetDataFromDef@CClickable@NUISystem@@QAEXXZ
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/NUISystem_CClickable.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CDefPointer.h"
namespace NUISystem {
void CClickable::GetDataFromDef()
{
    CDefPointer<const CUIDef> pdefinition=LoadAndReturnDef();
    InputDelay=pdefinition->InputDelay;
    if(pdefinition->ActionOnLeftClicked != UI_ACTION_TYPE_NONE) {
        CCountedPointer<CAction> paction=new CAction;
        paction->ActionType=pdefinition->ActionOnLeftClicked;
        if(paction->ActionType == UI_ACTION_TYPE_PLAY_MOVIE) {
            CWideString nonConstFilename=pdefinition->MovieFilename;
            paction->AddParam(new CActionParamString(nonConstFilename));
        }
        AddActionOnLeftClicked(paction);
    }
    if(pdefinition->ActionOnLeftUnclicked != UI_ACTION_TYPE_NONE) {
        CCountedPointer<CAction> paction=new CAction;
        paction->ActionType=pdefinition->ActionOnLeftUnclicked;
        if(paction->ActionType == UI_ACTION_TYPE_PLAY_MOVIE) {
            CWideString nonConstFilename=pdefinition->MovieFilename;
            paction->AddParam(new CActionParamString(nonConstFilename));
        }
        AddActionOnLeftUnclicked(paction);
    }
    if(pdefinition->ActionOnLeftHeld != UI_ACTION_TYPE_NONE) {
        CCountedPointer<CAction> paction=new CAction;
        paction->ActionType=pdefinition->ActionOnLeftHeld;
        if(paction->ActionType == UI_ACTION_TYPE_PLAY_MOVIE) {
            CWideString nonConstFilename=pdefinition->MovieFilename;
            paction->AddParam(new CActionParamString(nonConstFilename));
        }
        AddActionOnLeftHeld(paction);
    }
    if(pdefinition->ActionOnRightClicked != UI_ACTION_TYPE_NONE) {
        CCountedPointer<CAction> paction=new CAction;
        paction->ActionType=pdefinition->ActionOnRightClicked;
        if(paction->ActionType == UI_ACTION_TYPE_PLAY_MOVIE) {
            CWideString nonConstFilename=pdefinition->MovieFilename;
            paction->AddParam(new CActionParamString(nonConstFilename));
        }
        AddActionOnRightClicked(paction);
    }
}
}