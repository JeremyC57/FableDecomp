// CTCTavernTable::GetEatingPoint(CThing& creature, C3DVector& pos, CRightHandedSet& rh_set, long& action_point_index, long& seat_index)
// -- claims free "ACTION_POINT_SIT" points of the table's named-action-point component in turn until one whose seat number (the
// digits after the prefix, 1-based) has food on the table; a number outside the Food vector gives the point back and fails.
// Grade: RELOCATION_MATCH (wf4) -- ported onto the shared headers; the old ebp/ebx web swap and the missing loop pad were the
// local hand-written GetTC: the shared GetTC(id, &ptc) (FABLE_RETAIL_CTHING_GETTC_EXPANDED) with the inline LowerBound fixed both.
// Leaf: ?GetEatingPoint@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B8B10, 417 bytes, thiscall `ret 14h`, no frame pointer, no EH frame. No flags.
// Identity: Ego_r.pdb + FableWin.pdb ?GetEatingPoint@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3@Z
//   (Ego_r 0x007B7AD0, between FreeSeat and GetServingPoint as retail's 0x007B8AA0 / 0x007B8CC0 are); the landed caller
//   CAIStateGroup_ServeFood::IsRunnable 0x00923070 declares it at this address with this signature.
// Callees: CCharString(const char*, long) 0x0099EBF0 (the by-value name argument built in its stack slot),
//   CTCNamedUserActionPoints::GetFreeNamedActionPoint 0x007FF190, CCharString() 0x0099E4B0,
//   CTCNamedUserActionPoints::GetName 0x007FEFC0, CCharString::GetSubString 0x0099EC70, GFCharStringToInt 0x0099E7F0 (__fastcall),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCUserActionPointBase::SetActionPointAsFree 0x007FEF40,
//   ~CCharString 0x0099EAE0. CVectorMap::LowerBound (0x0040F020) is expanded in place here (FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND:
//   finish then start loaded, `cdq ; sub ; sar` halving, `cmp [esi+eax*8],2Eh`), CCharString::GetLength is inline.
// Layout: CTCBase ParentThing +0x04; Food (vector<CIntelligentPointer<CThingObject> >, 8-byte elements) +0x28,
//   ActionPointSitStrlen +0x48, AvailableForUse +0x5C (engine/CTCTavernTable.h FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT).
// Shape: the component pointer is an uninitialised local filled by the unchecked GetTC(id, &ptc); VC7.1 homes it in esi, first
//   loaded from the action_point_index argument slot (`mov esi,[esp+28h]`), as in the landed sibling FreeSeat 0x007B8AA0. The
//   range test is written on `chair - 1` twice (`lea edx,[edi-1] ; cmp ; jbe` then `lea eax,[edi-1] ; test ; jl`), and the next
//   start index is read back after SetActionPointAsFree (`mov ebx,[ebp] ; inc ebx`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE_SETFREE
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_GETNAME
#define FABLE_RETAIL_CTCTAVERNTABLE_SEATS
#define FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCNamedUserActionPoints.h"
#include "engine/CTCTavernTable.h"

long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0

bool CTCTavernTable::GetEatingPoint(CThing& creature, C3DVector& pos, CRightHandedSet& rh_set, long& action_point_index, long& seat_index)
{
    if (AvailableForUse) {
        long start = 0;
        for (;;) {
            CTCNamedUserActionPoints* points;
            ParentThing->GetTC(TCI_NAMED_USER_ACTION_POINTS, &points);
            if (!points->GetFreeNamedActionPoint(creature, pos, rh_set, "ACTION_POINT_SIT", action_point_index, start, true)) {
                action_point_index = -1;
                return false;
            }
            CCharString name;
            points->GetName(action_point_index, name);
            CCharString number = name.GetSubString(ActionPointSitStrlen, name.GetLength());
            long chair = GFCharStringToInt(number);
            if (Food.size() <= (unsigned long)(chair - 1) || chair - 1 < 0) {
                points->SetActionPointAsFree(action_point_index);
                action_point_index = -1;
                break;
            }
            if (Food[chair - 1].GetPItem()) {
                seat_index = chair - 1;
                return true;
            }
            points->SetActionPointAsFree(action_point_index);
            start = action_point_index + 1;
        }
    }
    return false;
}