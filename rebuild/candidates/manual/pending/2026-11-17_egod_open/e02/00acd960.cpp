// QuadricOptimiserInternals::COptimisedPrimitive::Optimise(float max_error) -- contract vertex pairs cheapest first until the cheapest costs more than max_error: pop stale (non-existing) contractions, re-cost illegal fold-overs with the high penalty, perform the rest
// Grade: DIFFER(383v383) -- allocation only (every instruction equal up to esi/edi/ebp naming): retail map ptr ebp / contraction Data edi / Info esi; ours map edi / Data esi / Info ebp (c2prio: y 100 > map 91 > Data 86 > Info 72; retail needs Info coloured before the erase node y)
// Leaf: ?Optimise@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXM@Z
// Flags: /Gr
// Retail: Fable.exe 0x00ACD960, 383 bytes, `ret 4`. Ego_d locals: max_error; block { pair_itr, contraction_ptr, contraction& }. Same-TU bodies after the row in Ego_d order: UpdateCost (0x00AC8A60, landed), PopFrontContraction (Ego_r 0x00C61530; local `front`; inlined twice in retail, its erase expanded at the first site and called at the second -- reproduced).
// Pass 2: DetectFoldOver(const CVertexPairContraction&) body added after the row (Ego_d order; Ego_d local c_sub; RELOCATION_MATCH at its own
//   address 0x00ACD900 with overlay FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_DETECTFOLDOVER_SUB): no change to the row's residue.
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_MULTIMAP_BEGIN_END
#define FABLE_RETAIL_STLP_MULTIMAP_EMPTY
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_QUADRIC_CVERTEXPAIRCONTRACTION
#define FABLE_RETAIL_QUADRIC_CVERTEXPAIRCONTRACTION_ISILLEGAL
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_DETECTFOLDOVER_SUB
#include <new>
#include "stlport/fable_map.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"

namespace QuadricOptimiserInternals {

void COptimisedPrimitive::Optimise(float max_cost)
{
    while (!VertexPairContractions.empty()) {
        std::multimap<float,CCountedPointer<CVertexPairContraction> >::iterator pair_itr = VertexPairContractions.begin();
        CCountedPointer<CVertexPairContraction> contraction_ptr = pair_itr->second;
        CVertexPairContraction& contraction = *contraction_ptr;
        if (!contraction.Exists) {
            PopFrontContraction();
            continue;
        }
        if (contraction.Cost > max_cost)
            return;
        if (contraction.IsIllegal() && DetectFoldOver(contraction)) {
            contraction.Cost += Parameters.HighPenalty;
            UpdateCost(contraction_ptr);
        } else {
            PerformContraction(contraction);
            PopFrontContraction();
        }
    }
}

void COptimisedPrimitive::UpdateCost(const CCountedPointer<CVertexPairContraction>& contraction)
{
    if (contraction->Exists) {
        VertexPairContractions.erase(contraction->ListIterator);
        contraction->ListIterator = VertexPairContractions.insert(std::pair<const float,CCountedPointer<CVertexPairContraction> >(contraction->Cost, contraction));
    }
}

void COptimisedPrimitive::PopFrontContraction()
{
    std::multimap<float,CCountedPointer<CVertexPairContraction> >::iterator front = VertexPairContractions.begin();
    front->second->Exists = false;
    VertexPairContractions.erase(front);
}


bool COptimisedPrimitive::DetectFoldOver(const CVertexPairContraction& contraction)
{
    for (unsigned long c_sub = 0; c_sub < contraction.SubContractions.size(); ++c_sub) {
        if (DetectFoldOver(contraction.SubContractions[c_sub], contraction.ResultingPosition))
            return true;
    }
    return false;
}

}
