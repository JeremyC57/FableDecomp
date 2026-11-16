// 007d00f0 CTCTavernGameShoveHaPenny::FrameUpdate
// Grade: DIFFER(1016v1016) -- frame 0x28 instead of 0x2c; local-slot layout differs
// Flags: /Gr
// Inline-budget: 007D0186..1D6 expands GetAngleXY and its rounding helper. Plain, T116/160/174/512 and scoped depth16 fail the required split; force only these real helpers.
// Leaf: ?FrameUpdate@CTCTavernGameShoveHaPenny@@UAEXXZ
// Identity: both PDBs, donor 02739BB0, retail vtable 0126F2F4 slot 9.
#define FABLE_RETAIL_SHOVE_FRAME_PASS23
#define FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_HIDEMESH
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_PRESSED_ACCESS
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_EVENTINQUEUE_ACCESS
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameShoveHaPennyDef.h"
#include "engine/CDefinitionManager.h"
template <class T>
bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCoinBase.h"
#include "engine/CTCTavernGameShoveHaPenny.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessTavernGame.h"

#ifdef _MSC_VER
#pragma intrinsic(sqrt,atan2,fmod)
#endif
// Full original string cleanup context: both PDBs and native0099E9B0.
void CCharString::UnassignString()
{
    if(PStringData) {
        CCharStringData* data=PStringData;
        data->DecRef();
        if(data->GetRefs()<=0) delete PStringData;
        PStringData=0;
    }
}
#ifdef _MSC_VER
// Native ctor0099EBF0 and dtor0099EAE0 remain calls. Plain context1088B expands them;
// this existing owner policy retains both exact native bodies61B/12B and cleanup87B.
#pragma auto_inline(off)
#endif
CCharString::~CCharString() { UnassignString(); --NoStrings; }
CCharString::CCharString(const char* text,long length) : PStringData(0)
{
    ++NoStrings;
    if(PStringData) UnassignString();
    if(text && *text) PStringData=AllocStringData(text,length);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

void CTCTavernGameShoveHaPenny::FrameUpdate()
{
    CTCTavernGame::FrameUpdate();
    if (!IsBeingPlayed) return;
        CDefPointer<const CTavernGameShoveHaPennyDef> pdef;
        GetParentThing().PeekBaseDef().GetSubDef(&pdef);
        switch (GetGameState()) {
        case 15:
            HideMesh(Arrow);
            PositionMesh(Arrow1,BoardPos,BoardRHSet,C3DVector(Held->Pos.X-0.05f,Held->Pos.Y,0.03f),0,0.25f);
            PositionMesh(Arrow2,BoardPos,BoardRHSet,C3DVector(Held->Pos.X+0.05f,Held->Pos.Y,0.03f),0,0.75f);
            break;
        case 16:
            if (Held->Vel.GetMagnitude() > 0.0f) {
                PositionMesh(Arrow,BoardPos,BoardRHSet,
                    C3DVector(Held->Vel.X*pdef->TargetDistanceScale+Held->Pos.X,
                              Held->Vel.Y*pdef->TargetDistanceScale+Held->Pos.Y,0.001f),
                    0,Held->Vel.GetAngleXY()+0.5f);
            } else HideMesh(Arrow);
            HideMesh(Arrow1);
            HideMesh(Arrow2);
            break;
        default:
            HideMesh(Arrow);
            HideMesh(Arrow1);
            HideMesh(Arrow2);
            break;
        }
        if (GetGameState()==17) {
            float time_delta=1.0f/CMainGameComponent::GetConstantFPS();
            long physics_frames=5;
            for(long n=0;n<physics_frames;++n) UpdateCoins(time_delta/physics_frames);
            UpdateCoinSounds(false);
            long old_score=(long)Score;
            CalcScores();
            if(Score!=old_score) PlaySoundW("TAVERN_GAME_OBJECT_SELECTION_CHANGE");
            if(AtRest(pdef->RestSpeed)) {
                if(RoundsPlayed+1==pdef->Rounds) {
                    GameWon=TargetRound<=Score;
                    CDefPointer<const CTavernGameDef> base_def;
                    GetParentThing().PeekBaseDef().GetSubDef(&base_def);
                    PlaySoundW(GameWon?"TAVERN_GAME_WIN_GAME":"TAVERN_GAME_LOSE_GAME");
                    SetStateAfterOK(GetText(GameWon?base_def->WinPhrase:base_def->LosePhrase),GetText(base_def->Continue),(ETavernGameStates)6);
                } else {
                    ++RoundsPlayed;
                    AddNewCoin(C3DVector(BoardSize.X*0.5f,pdef->PositionCoinDepth,0.0f));
                    AverageJoystickPos=C2DVector(0.0f,0.0f);
                    MousePosEffect=C2DVector(0.0f,0.0f);
                    SetState((ETavernGameStates)15);
                }
            }
        }
        if(GetGameState()==45 && YesState==11) UpdateCoinSounds(true);
        UpdateMeshes();
}
