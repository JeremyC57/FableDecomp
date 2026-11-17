// std::for_each<pair<CCharString, float>*, `anonymous namespace'::CGetMatchingStrikeEndEventName>(first, last, f) -- apply the strike-end matcher to every animation event and return it
// Leaf: ??$for_each@PAU?$pair@VCCharString@@M@std@@VCGetMatchingStrikeEndEventName@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path, as the landed 0x00870C90.)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085B840, 61 bytes, `ret 10h`: STLport/SGI `for (; first != last; ++first) f(*first); return f;`,
//   /Gr `__fastcall` (the 12-byte functor returned through the hidden pointer in ecx, `first` in edx, last and the functor by value on
//   the stack).
// Identity (NF56, round 7, queue q10447): the 8-byte element is std::pair<CCharString, float> (an animation event) and the 12-byte
//   functor whose operator() it calls (0x0085B720) is the strike-events TU's anonymous-namespace CGetMatchingStrikeEndEventName
//   (FableWin.pdb 0x0282F660; Ego_r.pdb 0x0083B660 is the Dinkumware twin ??$_For_each@PAU?$pair@VCCharString@@M@std@@
//   VCGetMatchingStrikeEndEventName@?A0xad05d24c@@@std@@...). Was a local std::for_each over the made-up SCombatantRing / CSetupHelperRings
//   (binding the callee as ??RCSetupHelperRings@@QAEXABUSCombatantRing@@@Z). Now the explicit instantiation of the shared
//   stlport/fable_algorithm.h for_each (FABLE_RETAIL_STLP_FOR_EACH_BODY) over the functor's declaration (FableWin.pdb: class, constructor from const CCombatActionBase::CStrikeEvent&; layout as the 0x0085B720 row).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
namespace {
struct CStrikeEvent;   // CCombatActionBase::CStrikeEvent in the PDBs (engine/CCombatActionBase.h only forward-declares it; the 0x0085B720 row's layout model)
class CGetMatchingStrikeEndEventName {   // FableWin.pdb: a class with a user constructor (so for_each returns it through the hidden pointer in ecx)
public:
    CGetMatchingStrikeEndEventName(const CStrikeEvent& strikeEvent);   // FableWin.pdb ??0CGetMatchingStrikeEndEventName@?A0x..@@QAE@ABVCStrikeEvent@CCombatActionBase@@@Z (not emitted here)
    void operator()(std::pair<CCharString,float>& event);                                 // 0x0085B720
    const CStrikeEvent* StrikeEvent;                                   // +0x00
    long BestEventEndFrame;                                                              // +0x04
    bool FoundEvent;                                                                     // +0x08
};
}

template CGetMatchingStrikeEndEventName FABLE_FASTCALL std::for_each(std::pair<CCharString,float>* first, std::pair<CCharString,float>* last, CGetMatchingStrikeEndEventName f);
