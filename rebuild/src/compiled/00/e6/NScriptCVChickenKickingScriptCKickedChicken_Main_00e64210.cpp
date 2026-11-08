// NScript::CV_ChickenKickingScript::CKickedChicken::Main() -- the kicked chicken: measure the arena's four distance lines, wait until the chicken lands (shot strike position) or the hero crosses the foul line, squawk on each kick, then pick the landing sector between the four angle markers and the distance band from the line distances, flag ChickenLanded, release the chicken handle and hand the chicken back to scripting
// Shape (chunk 820): the strike direction is built member-wise from two named differences, `float dy = ...Y; float dx = ...X; direction.X = dx;
//   direction.Z = 0.0f; direction.Y = dy;`: member stores make VC7.1 load `a` first in every product of the final dot, and its term order is the reverse of the store order (X, Z, Y -> Y, Z, X as retail, 0x00E64B4B); the named differences keep retail's Y-then-X evaluation (chunk 790 park: 52 attempts on the dot spelling, all canonicalised)
// Leaf: ?Main@CKickedChicken@CV_ChickenKickingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E64210, 2803 bytes, thiscall, EH frame (the vector<CScriptThing> and CScriptThing locals).
// Identity (proven): slot 1 (Main) of the entity-script vtable 0x012E7498 (only reference to 0x00E64210 is .rdata 0x012E749C),
//   stored by CKickedChicken's ctor at 0x00E635C6 (Alloc 0x00E63590); Ego_r.pdb / FableWin.pdb name set
//   ?Main@CKickedChicken@CV_ChickenKickingScript@NScript@@EAEXXZ; registration "KickedChicken" at 0x00E62BB1.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, GetDistanceBetweenThings 0x00CBE512 (x8, fastcall),
//   GetDistanceBetweenThingAndPosition 0x00CBE551 (fastcall; FableWin/Ego_r ?GetDistanceBetweenThingAndPosition@@YIMABVCScriptThing@@
//   ABVC3DVector@@@Z, bytes: thing GetPos then fsqrt of the squared difference), C3DVector::AccurateNormalise 0x00A14440 (x3) /
//   Rotate 0x00A13C60, vector<CScriptThing>::push_back 0x008ADF90 (x4) / ~vector 0x008AC970 (declared-only specialization),
//   ~CScriptThing 0x004AA840, rand 0x00BFEB16, GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0, _CIacos 0x00BFEBA2 (intrinsic acos; cos is the inline fcos),
//   CScriptGameResourceObjectScriptedThingBase::IsValid 0x00CD23B9 / Release 0x00CD2770; Game slots 6/60/455/690 (GetPos via the thing,
//   MsgIsKicked, EntityGetShotStrikePos, PlaySoundOnThing). Constants: 0.0f, 1.0f, -1.0f, DELTA_NOTIONAL_ZERO 0x0129BA3C,
//   INV_TWO_PI 0x01237F08 (float), 0.5f 0x0122F59C, TWO_PI 0x0124F2B8 (double) -- fltchk pairs every one.
// Layout: CKickedChicken ParentClass +0x14, PMasterData +0x18 (local class model, as the landed CKickedChicken siblings); parent
//   engine/NScript_CV_ChickenKickingScript.h: DistanceBand, FinalSector (+0x4c in this function's store), ChickenLanded, seh_Chicken.
// Shape: the landing wait is `bool landed; while (!landed)` with an if / else-if setting the flag (a while(true)+break rotates the loop);
//   the foul / hero copies use the implicit operator= so all three floats are stored; the angle dot is a running sum Z, Y, X; acos * INV_
//   TWO_PI and * 0.5f stay separate statements (a literal product folds into one constant); ++sector is its own statement.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#include "engine/EHeroAbility.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/ECutsceneBehaviour.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc.h"
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
#include "engine/NScript_CV_ChickenKickingScript.h"
extern "C" double __cdecl cos(double);
extern "C" double __cdecl acos(double);
#pragma intrinsic(cos, acos)
extern "C" int __cdecl rand();                                          // 0x00BFEB16
CCharString __fastcall GFIntToCharString(long);                         // 0x0099F830
CCharString __fastcall operator+(const char*, const CCharString&);      // 0x0099F690
float __fastcall GetDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);              // 0x00CBE512
float __fastcall GetDistanceBetweenThingAndPosition(const CScriptThing& thing, const C3DVector& pos); // 0x00CBE551
static const float TWO_PI = 6.2831855f;
static const float INV_TWO_PI = 0.15915494f;
inline CScriptThing::~CScriptThing() { }
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { return !PImp.Data ? NullCoord : PImp.Data->GetPos(); }
template <> std::vector<CScriptThing, std::allocator<CScriptThing> >::~vector();   // 0x008AC970

namespace NScript {
class CV_ChickenKickingScript::CKickedChicken : public CEntityScriptBase {
public:
    CV_ChickenKickingScript* ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    CKickedChicken(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& me);
    virtual ~CKickedChicken();
    static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& me);
    virtual CScriptBase* GetParentScript() const;
private:
    virtual void Main();
};

void CV_ChickenKickingScript::CKickedChicken::Main()
{
    CScriptThing arena = Game->GetThingWithScriptName("ChickenKickingArena");
    float distance1 = GetDistanceBetweenThings(arena, Game->GetThingWithScriptName("FirstLineMarker"));
    float distance2 = GetDistanceBetweenThings(arena, Game->GetThingWithScriptName("SecondLineMarker"));
    float distance3 = GetDistanceBetweenThings(arena, Game->GetThingWithScriptName("ThirdLineMarker"));
    float distance4 = GetDistanceBetweenThings(arena, Game->GetThingWithScriptName("FourthLineMarker"));
    float rearRight = GetDistanceBetweenThings(arena, Game->GetThingWithScriptName("RearRight"));
    float midRight = GetDistanceBetweenThings(arena, Game->GetThingWithScriptName("MidRight"));
    float midRight2 = GetDistanceBetweenThings(arena, Game->GetThingWithScriptName("MidRight2"));
    float frontRight = GetDistanceBetweenThings(arena, Game->GetThingWithScriptName("FrontRight"));
    C3DVector strikePos;
    {
        C3DVector foulA;
        foulA = Game->GetThingWithScriptName("FoulLineMarkerA").GetPos();
        C3DVector foulB;
        foulB = Game->GetThingWithScriptName("FoulLineMarkerB").GetPos();
        ParentClass->DistanceBand = 0;
        bool landed = false;
        while (!landed) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Me.MsgIsKicked()) {
                if (IsActiveThreadTerminating()) return;
                CCharString sound = "SND_MM_CHICKEN_BUKARK_0" + GFIntToCharString(rand() % 4 + 1);
                Game->PlaySoundOnThing(Me, sound);
            }
            C3DVector heroPos;
            heroPos = Game->GetHero().GetPos();
            if (Game->EntityGetShotStrikePos(Me, strikePos)) {
                if (IsActiveThreadTerminating()) return;
                landed = true;
            } else if (((foulA - heroPos) ^ (foulB - heroPos)).Z > 0.0f) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->DistanceBand = -1;
                landed = true;
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    if (ParentClass->DistanceBand == 0) {
        if (IsActiveThreadTerminating()) return;
        std::vector<CScriptThing> markers;
        const C3DVector& arenaPos = arena.GetPos();
        float dy = strikePos.Y - arenaPos.Y;
        float dx = strikePos.X - arenaPos.X;
        C3DVector direction;
        direction.X = dx;
        direction.Z = 0.0f;
        direction.Y = dy;
        direction.AccurateNormalise();
        markers.push_back(Game->GetThingWithScriptName("FirstAngleMarker"));
        markers.push_back(Game->GetThingWithScriptName("SecondAngleMarker"));
        markers.push_back(Game->GetThingWithScriptName("ThirdAngleMarker"));
        markers.push_back(Game->GetThingWithScriptName("FourthAngleMarker"));
        ParentClass->FinalSector = 0;
        long sector = 1;
        if (markers.begin() != markers.end()) {
            for (std::vector<CScriptThing>::iterator it = markers.begin() + 1; it != markers.end(); ++it) {
                C3DVector a = (it - 1)->GetPos() - arena.GetPos();
                C3DVector b = it->GetPos() - arena.GetPos();
                b.Z = 0.0f;
                a.Z = 0.0f;
                if (a.X * a.X + a.Y * a.Y > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO &&
                    b.X * b.X + b.Y * b.Y > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
                    a.AccurateNormalise();
                    b.AccurateNormalise();
                    float angle = a.Z * b.Z; angle += a.Y * b.Y; angle += a.X * b.X;
                    if (angle < -1.0f) angle = -1.0f;
                    else if (angle > 1.0f) angle = 1.0f;
                    angle = 0.5f * ((float)acos(angle) * INV_TWO_PI);
                    a.Rotate(C3DVector(0.0f, 0.0f, 1.0f), angle);
                    if (direction.Dot(a) > cos(angle * (double)TWO_PI))
                        ParentClass->FinalSector = sector;
                }
                ++sector;
            }
        }
        const C3DVector& unused = arena.GetPos();
        float distance = GetDistanceBetweenThingAndPosition(arena, strikePos);
        if (distance > distance4) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->DistanceBand = 4;
        } else if (distance > distance3) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->DistanceBand = 3;
        } else if (distance > distance2) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->DistanceBand = 2;
        } else if (distance > distance1) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->DistanceBand = 1;
        } else {
            if (IsActiveThreadTerminating()) return;
            ParentClass->DistanceBand = 0;
        }
    }
    ParentClass->ChickenLanded = true;
    if (ParentClass->seh_Chicken.IsValid())
        ParentClass->seh_Chicken.Release();
    while (!Game->StartScriptingEntity(Me, ParentClass->seh_Chicken, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
}
}