// CTCStealth::UpdateDangerInfo() -- per update: drop dead danger things; with danger things left, tell the attackers beyond the allowed maximum (furthest from their initial position first) not to attack this thing and restart the danger cool-off timer; with none, count the cool-off down (clearing the danger status when it runs out); count the running-away timer down
// Leaf: ?UpdateDangerInfo@CTCStealth@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00747C70, 303 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?UpdateDangerInfo@CTCStealth@@AAEXXZ (private); Ego_d.pdb tc_stealth.obj record (num_to_send_home,
//   real_threat_list; it; pthing, ptc_targeting_ai -- Ego_d puts `it` in its own block, the do-while here keeps it in the if block:
//   byte-identical either way, an explicit bare block was graded RELOCATION_MATCH too).
// Callees: std::__find_if<pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>*, CDeadDangerThing> 0x00748E40 and
//   std::remove_copy_if 0x00748D80 (out of line, __fastcall: the inline STLport remove_if), vector<...>::erase(first, last) 0x00749280,
//   CTCStealth::GetNumberOfAttackersOverMaximum 0x00747890 (same TU, body below), std::sort<const CThing**, CCompare_FurthestFromInitPos2D>
//   0x00749740 (__fastcall), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (GetTC<CTCTargetingAI>, interface 8),
//   CTCTargetingAI::AddThingToNotAttack 0x007F61B0, __ftol2 0x00BFEA70, free 0x00BFEA14, CTCStealth::ClearDangerStatus 0x00747090.
//   Data: CMainGameComponent::ConstantFPS 0x01375550, 1.0f 0x0122DED8 (the inline GetDangerFeedbackHangtimeFrames, body below).
// Shape: retail keeps real_threat_list's start in esi from the GetNumberOfAttackersOverMaximum call to the final free (0x00747CDC ..
//   0x00747D6E) and zeroes the list through esi: VC7.1 does that only when it sees the callee's body (same TU, defined before the row
//   as in Ego_d's object order) AND that body's vector::reserve (FABLE_RETAIL_STLP_VECTOR_RESERVE: visible, out of line). The helper
//   body itself grades RELOCATION_MATCH against 0x00747890.
// Headers: overlay FABLE_RETAIL_CSTEALTHDEF_SUBDEF (CStealthDef : CSubDefClassBase, Ego_d), FABLE_RETAIL_NSTEALTH_EDANGERSTATUS (Ego_d
//   enumerators); existing FABLE_RETAIL_CTCTARGETINGAI_ATTACK, FABLE_RETAIL_STLP_SORT_FASTCALL, FABLE_RETAIL_STLP_VECTOR(_RESERVE),
//   FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE, FABLE_RETAIL_CTHING_GETTC_EXPANDED.
// Names: CDeadDangerThing / CCompare_FurthestFromInitPos2D are Ego_d's anonymous-namespace classes (also FableWin _Remove_if<CDeadDangerThing>);
//   g_maxNumberOfAttackers 0x01375728 is the bound DESCRIPTIVE spelling (no PDB data name found).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CTCTARGETINGAI_ATTACK
#define FABLE_RETAIL_CSTEALTHDEF_SUBDEF
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_NSTEALTH_EDANGERSTATUS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCStealth.h"
#include "engine/CTargetingDef.h"
#include "engine/CTCTargetingAI.h"
#include "engine/CStealthDef.h"
#include "engine/CMainGameComponent.h"

namespace {
class CDeadDangerThing {                    // anonymous-namespace predicate (Ego_d tc_stealth.obj; the landed __find_if 0x00748E40 / remove_copy_if 0x00748D80)
public:
    bool operator()(const std::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>& danger_thing) const;
};
class CCompare_FurthestFromInitPos2D {      // anonymous-namespace sort comparator (Ego_d)
public:
    bool operator()(const CThing* a, const CThing* b) const;
};
}
namespace std {
template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&);   // 0x00748E40
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred);   // 0x00748D80
template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred) {
    return __find_if(__first, __last, __pred, random_access_iterator_tag());
}
template <class _ForwardIter, class _Predicate>
inline _ForwardIter remove_if(_ForwardIter __first, _ForwardIter __last, _Predicate __pred) {
    __first = find_if(__first, __last, __pred);
    if (__first == __last)
        return __first;
    else {
        _ForwardIter __next = __first;
        return remove_copy_if(++__next, __last, __first, __pred);
    }
}
}

extern long g_maxNumberOfAttackers;                      // 0x01375728 (bound spelling; descriptive)

// Same-TU body (Ego_d tc_stealth.obj: defined just before the row; the landed row 0x00747890's logic with Ego_d's names). Visible, it
// tells VC7.1 the callee keeps no address of real_threat_list, so the row keeps the list's start in esi across the loop.
long CTCStealth::GetNumberOfAttackersOverMaximum(std::vector<const CThing*>* preal_danger_things) const
{
    std::vector<const CThing*> rdt_prop;
    if (!preal_danger_things)
        preal_danger_things = &rdt_prop;
    preal_danger_things->reserve(DangerThings.size());
    for (std::vector<std::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> >::const_iterator it = DangerThings.begin();
         it != DangerThings.end(); ++it) {
        NStealth::EDangerStatus dan_stat = it->second.DangerStatus;
        if (dan_stat == NStealth::DANGER_STATUS_COMBAT || dan_stat == NStealth::DANGER_STATUS_CHASING)
            preal_danger_things->push_back(it->first.Get());
    }
    long num_real_threat = preal_danger_things->size();
    long max_number_of_attackers = -1;
    if (g_maxNumberOfAttackers != -1)
        max_number_of_attackers = g_maxNumberOfAttackers;
    if (MaxNumberOfDangerThings != -1 && (max_number_of_attackers == -1 || MaxNumberOfDangerThings < max_number_of_attackers))
        max_number_of_attackers = MaxNumberOfDangerThings;
    if (RegionBasedMaxNumberOfDangerThings != -1 && (max_number_of_attackers == -1 || RegionBasedMaxNumberOfDangerThings < max_number_of_attackers))
        max_number_of_attackers = RegionBasedMaxNumberOfDangerThings;
    if (max_number_of_attackers != -1 && num_real_threat > max_number_of_attackers)
        return num_real_threat - max_number_of_attackers;
    return -1;
}

float CTCStealth::GetDangerFeedbackHangtimeFrames() const
{
    if (PDef.Get())
        return CMainGameComponent::GetConstantFPS() * PDef->DangerFeedbackHangtimeSeconds;
    return 1.0f;
}

void CTCStealth::UpdateDangerInfo()
{
    if (!DangerThings.empty()) {
        DangerThings.erase(std::remove_if(DangerThings.begin(), DangerThings.end(), CDeadDangerThing()), DangerThings.end());
        std::vector<const CThing*> real_threat_list;
        long num_to_send_home = GetNumberOfAttackersOverMaximum(&real_threat_list);
        if (num_to_send_home > 0 && !real_threat_list.empty()) {
            std::sort(real_threat_list.begin(), real_threat_list.end(), CCompare_FurthestFromInitPos2D());
            std::vector<const CThing*>::iterator it = real_threat_list.begin();
            do {
                CThing* pthing = const_cast<CThing*>(*it++);
                CTCTargetingAI* ptc_targeting_ai;
                if (pthing->GetTC(&ptc_targeting_ai))
                    ptc_targeting_ai->AddThingToNotAttack(GetParentThing());
            } while (--num_to_send_home > 0);
        }
        InDangerCoolOffTimer = (long)GetDangerFeedbackHangtimeFrames();
    } else if (InDangerCoolOffTimer >= 0) {
        if (--InDangerCoolOffTimer < 0)
            ClearDangerStatus();
    }
    if (RunningAwayFromCombatTimer >= 0)
        --RunningAwayFromCombatTimer;
}
