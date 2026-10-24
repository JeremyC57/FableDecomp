// Behaviour test for QuadricOptimiserInternals::COptimisedPrimitive::UpdateContractions(const CSubContraction&) @ 0x00AC96E0
// For every pair contraction referenced by the sub-pair's first vertex: recompute it (ComputeContraction), then re-key
// it in the cost queue (UpdateCost: only when it still exists -- erase its queue entry, re-insert under the new cost and
// remember the new position). Contractions of other vertices are not touched; reference counts are restored.
#include <cstdio>
#include <map>
#include <vector>

struct CContractionModel;
typedef std::multimap<float, CContractionModel*> TContractionQueue;

struct CContractionModel {
    bool Exists;
    bool Unsplit;
    float Cost;
    float NextCost;                       // what ComputeContraction will produce
    int RefCount;
    int Computed;
    TContractionQueue::iterator ListIterator;
};

struct CReferenceModel {
    CContractionModel* Contraction;
    unsigned int SubIndex;
};

struct CVertexModel {
    std::map<unsigned int, CReferenceModel> PairContractions;
};

struct CSubContractionModel { unsigned int Vertex1; unsigned int Vertex2; };

struct CPrimitiveModel {
    std::vector<CVertexModel> Vertices;
    TContractionQueue VertexPairContractions;

    void ComputeContraction(CContractionModel& contraction)
    {
        ++contraction.Computed;
        contraction.Cost = contraction.NextCost;
    }
    void UpdateCost(CContractionModel* contraction)
    {
        if (contraction->Exists) {
            VertexPairContractions.erase(contraction->ListIterator);
            contraction->ListIterator = VertexPairContractions.insert(TContractionQueue::value_type(contraction->Cost, contraction));
        }
    }
    void UpdateContractions(const CSubContractionModel& sub)
    {
        CVertexModel& vertex = Vertices[sub.Vertex1];
        for (std::map<unsigned int, CReferenceModel>::iterator i = vertex.PairContractions.begin();
             i != vertex.PairContractions.end(); ++i) {
            CReferenceModel reference = i->second;
            ++reference.Contraction->RefCount;            // the reference copy
            CContractionModel* contraction = reference.Contraction;
            ++contraction->RefCount;                      // the counted-pointer local
            ComputeContraction(*contraction);
            UpdateCost(contraction);
            --contraction->RefCount;
            --reference.Contraction->RefCount;
        }
    }
};

static void Track(CPrimitiveModel& p, CContractionModel& c)
{
    c.ListIterator = p.VertexPairContractions.insert(TContractionQueue::value_type(c.Cost, &c));
}

static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    CPrimitiveModel prim;
    prim.Vertices.resize(3);
    CContractionModel c[4];
    static const float costs[4] = { 5.0f, 1.0f, 3.0f, 2.0f };
    static const float next[4] = { 0.5f, 9.0f, 4.0f, 7.0f };
    for (int i = 0; i < 4; ++i) {
        c[i].Exists = i != 2;            // c[2] was already contracted away: not in the queue
        c[i].Unsplit = false;
        c[i].Cost = costs[i];
        c[i].NextCost = next[i];
        c[i].RefCount = 1;
        c[i].Computed = 0;
        if (c[i].Exists) Track(prim, c[i]);
    }
    CReferenceModel r0 = { &c[0], 0 }, r1 = { &c[1], 1 }, r2 = { &c[2], 0 }, r3 = { &c[3], 0 };
    prim.Vertices[1].PairContractions[4] = r0;
    prim.Vertices[1].PairContractions[2] = r1;
    prim.Vertices[1].PairContractions[9] = r2;
    prim.Vertices[2].PairContractions[1] = r3;   // another vertex: untouched
    CSubContractionModel sub = { 1, 2 };
    prim.UpdateContractions(sub);
    int ok = c[0].Computed == 1 && c[1].Computed == 1 && c[2].Computed == 1 && c[3].Computed == 0;
    for (int i = 0; i < 4; ++i) if (c[i].RefCount != 1) ok = 0;
    if (prim.VertexPairContractions.size() != 3) ok = 0;
    // queue order by cost: c[0] 0.5, c[3] 2.0 (not recomputed), c[1] 9.0
    TContractionQueue::iterator q = prim.VertexPairContractions.begin();
    if (ok && (q->second != &c[0] || !Near(q->first, 0.5f))) ok = 0;
    ++q;
    if (ok && (q->second != &c[3] || !Near(q->first, 2.0f))) ok = 0;
    ++q;
    if (ok && (q->second != &c[1] || !Near(q->first, 9.0f))) ok = 0;
    if (ok && (c[0].ListIterator->second != &c[0] || c[1].ListIterator->second != &c[1])) ok = 0;
    if (!Near(c[2].Cost, 4.0f)) ok = 0;          // recomputed even though it is not queued
    if (!ok) { std::printf("00ac96e0_TEST FAIL\n"); return 1; }
    std::printf("00ac96e0_TEST PASS\n");
    return 0;
}