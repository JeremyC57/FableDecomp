// QuadricOptimiserInternals::COptimisedPrimitive::Optimise(float max_error) -- contract vertex pairs cheapest first until the cheapest costs more than max_error: pop stale (non-existing) contractions, re-cost illegal fold-overs with the high penalty, perform the rest
// Fidelity: test legality, then handle fold-over in the nested branch at 0x00ACDA3C.
// Leaf: ?Optimise@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXM@Z
// Flags: /Gr
// Retail: Fable.exe, 0x00ACD960, 383 bytes, thiscall, ret 4.
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

void COptimisedPrimitive::Optimise(float max_error)
{
    while (!VertexPairContractions.empty()) {
        std::multimap<float,CCountedPointer<CVertexPairContraction> >::iterator pair_itr = VertexPairContractions.begin();
        CCountedPointer<CVertexPairContraction> contraction_ptr = pair_itr->second;
        CVertexPairContraction& contraction = *contraction_ptr;
        if (!contraction.Exists) {
            PopFrontContraction();
            continue;
        }
        if (contraction.Cost > max_error)
            return;
        if (contraction.IsIllegal()) {
            if (DetectFoldOver(contraction)) {
                contraction.Cost += Parameters.HighPenalty;
                UpdateCost(contraction_ptr);
                continue;
            }
        }
        PerformContraction(contraction);
        PopFrontContraction();
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


}
