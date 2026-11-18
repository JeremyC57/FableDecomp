#include <cassert>
#include <cstdio>
#include <map>

namespace QuadricOptimiserInternals {

template<class T> class CCountedPointer
{
    T* data;
public:
    explicit CCountedPointer(T* pointer=0) : data(pointer) { }
    T* Get() const { return data; }
    T& operator*() const { return *data; }
    T* operator->() const { return data; }
};

class CVertexPairContraction
{
public:
    typedef std::multimap<float, CCountedPointer<CVertexPairContraction> > List;
    List::iterator ListIterator;
    float Cost;
    bool Exists, Illegal, FoldOver;
    unsigned checked, detected, performed;

    CVertexPairContraction(float cost, bool exists, bool illegal, bool fold_over)
        : Cost(cost), Exists(exists), Illegal(illegal), FoldOver(fold_over),
          checked(0), detected(0), performed(0) { }

    bool IsIllegal() { ++checked; return Illegal; }
};

class COptimisedPrimitive
{
public:
    typedef CVertexPairContraction::List Contractions;
    Contractions VertexPairContractions;
    struct { float HighPenalty; } Parameters;

    COptimisedPrimitive(float penalty) { Parameters.HighPenalty=penalty; }

    bool DetectFoldOver(CVertexPairContraction& contraction)
    {
        ++contraction.detected;
        return contraction.FoldOver;
    }

    void PerformContraction(CVertexPairContraction& contraction) { ++contraction.performed; }

    void Add(CVertexPairContraction& contraction)
    {
        contraction.ListIterator=VertexPairContractions.insert(
            Contractions::value_type(contraction.Cost,CCountedPointer<CVertexPairContraction>(&contraction)));
    }

    void Optimise(float max_error);
    void UpdateCost(const CCountedPointer<CVertexPairContraction>& contraction);
    void PopFrontContraction();
};
}

// The selected implementation is reproduced verbatim for this standalone fixture.
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

int main()
{
    using namespace QuadricOptimiserInternals;
    COptimisedPrimitive primitive(2.0f);
    primitive.Optimise(5.0f);
    assert(primitive.VertexPairContractions.empty());

    CVertexPairContraction stale(-0.5f, false, true, true);
    CVertexPairContraction legal(1.0f, true, false, true);
    CVertexPairContraction illegal_without_fold(2.0f, true, true, false);
    CVertexPairContraction repeated_fold(3.0f, true, true, true);
    CVertexPairContraction boundary(5.0f, true, false, false);
    CVertexPairContraction expensive(8.0f, true, true, true);

    primitive.Add(stale);
    primitive.Add(legal);
    primitive.Add(illegal_without_fold);
    primitive.Add(repeated_fold);
    primitive.Add(boundary);
    primitive.Add(expensive);
    primitive.Optimise(5.0f);

    assert(stale.performed == 0 && stale.checked == 0 && stale.detected == 0);
    assert(legal.performed == 1 && legal.checked == 1 && legal.detected == 0);
    assert(illegal_without_fold.performed == 1 && illegal_without_fold.checked == 1 &&
           illegal_without_fold.detected == 1);
    assert(boundary.performed == 1 && !boundary.Exists);
    assert(repeated_fold.performed == 0 && repeated_fold.Cost == 7.0f &&
           repeated_fold.checked == 2 && repeated_fold.detected == 2 && repeated_fold.Exists);
    assert(expensive.checked == 0 && expensive.performed == 0 && expensive.Exists);
    assert(primitive.VertexPairContractions.size() == 2 &&
           primitive.VertexPairContractions.begin()->second.Get() == &repeated_fold);
    std::puts("00acd960_TEST PASS");
    return 0;
}
