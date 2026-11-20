// Naming wave 35c (queue land0411b 0x009E5090): CSymbolInfo is nested in CSymbolMap (FableWin.pdb `?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@J@Z`, class-keyed, const).
// std::for_each(const NAnimationEvents::CEvent* first, const NAnimationEvents::CEvent* last, CFillEventNames f) -- push the symbol name of every event index onto the functor's name vector, return the functor
// Leaf: ??$for_each@PBVCEvent@NAnimationEvents@@VCFillEventNames@2@@std@@YI?AVCFillEventNames@NAnimationEvents@@PBVCEvent@2@0V12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FC8E0, 112 bytes, `ret 0xc` (/Gr: hidden functor result in ecx, first in edx).
// Label correction: catalog "_For_each<CThingCreatureBase**,CApplyAdditionalImpulseIfInCombatAndTooClose>" is
//   propagated; this is std::for_each<const NAnimationEvents::CEvent*, NAnimationEvents::CFillEventNames>.
// Callees: CSymbolMap::FindFirstSymbolWithVal 0x009E5090, CCharString copy constructor 0x0099EC30,
//   vector<CCharString>::_M_insert_overflow 0x00433530.
// Shape: CFillEventNames::operator() inlined as `EventNames->push_back(map->FindFirstSymbolWithVal(event.Index)->
//   EntryName)`; the 30-bit Index field gives the `and eax,3FFFFFFFh`.
// Name fix (NF208, 2026-11-20, FP-VECTORINSERT q6332 / q6341): the row-local CCharString / CSymbolMap / vector / __false_type /
//   for_each models are replaced by fable_string.h, engine/CSymbolMap.h (FABLE_RETAIL_CSYMBOLMAP + NF208 opt-in
//   FABLE_RETAIL_CSYMBOLMAP_FINDFIRSTSYMBOLWITHVAL_VALUE), stlport/fable_containers.h and stlport/fable_algorithm.h
//   (FABLE_RETAIL_STLP_FOR_EACH_BODY); the header-less NAnimationEvents::CEvent / CFillEventNames stay local (Ego_r.pdb layouts).
//   push_back's overflow call now binds the STLport protected spelling of 0x00433530 (?_M_insert_overflow@?$vector@VCCharString@@
//   V?$allocator@VCCharString@@@std@@@std@@IAE...). The empty tag shares the live `last` slot only when the callee's body is visible
//   (GOTCHAS b48): FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE keeps the real stl/_vector.c body visible but out of line, and
//   FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE (the self row 0x00433530's own macro) makes the pick-any copy it emits the owner's bytes.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_FINDFIRSTSYMBOLWITHVAL_VALUE
#include "engine/CSymbolMap.h"
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#include "stlport/fable_algorithm.h"

namespace NAnimationEvents {
class CEvent {                                            // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;                             // +0x00
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;                                           // +0x04
};

class CFillEventNames {                                   // Ego_r.pdb: 8 bytes
public:
    std::vector<CCharString>* EventNames;                 // +0x00
    const CSymbolMap* EventSymbolMap;                     // +0x04
    CFillEventNames(std::vector<CCharString>& names, const CSymbolMap& symbols) : EventNames(&names), EventSymbolMap(&symbols) {}
    void operator()(const CEvent& event)
    {
        EventNames->push_back(EventSymbolMap->FindFirstSymbolWithVal(event.Index)->EntryName);
    }
};
}

template NAnimationEvents::CFillEventNames FABLE_FASTCALL std::for_each(const NAnimationEvents::CEvent* first, const NAnimationEvents::CEvent* last,
                                                                    NAnimationEvents::CFillEventNames f);