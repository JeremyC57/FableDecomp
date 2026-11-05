// Behaviour test for QuadricOptimiserInternals::COptimisedPrimitive::ComputeContraction(CVertexPairContraction&) (0x00AC92C0):
// a model of the row -- an unsplit contraction defers to the self contraction; otherwise the optimal placement is replaced by
// the first (or second) vertex when that vertex's unique vertex still has remaining vertices outside the contraction, Weight2 is
// the first distance over the total (0.5 when both coincide), the cost is the largest sub-contraction cost, a contraction fixed
// at both ends takes the high penalty, and one fixed at its second end flips Weight2 and swaps every sub pair.
#include <stdio.h>
#include <math.h>
#include <vector>
struct Vec { float X, Y, Z; };
struct Sub { unsigned long Vertex1, Vertex2; };
struct Contraction { bool Unsplit; std::vector<Sub> Subs; float Weight2, Cost; Vec Result; };
struct Primitive {
    std::vector<Vec> Positions;
    std::vector<bool> Free;                 // ContainsAllVertices(GetRemainingVertices(unique vertex)) per vertex
    std::vector<float> SubCosts;            // ComputeSubContractionCost per sub pair
    float HighPenalty;
    int SelfContractions;
    static float Distance(const Vec& a, const Vec& b)
    {
        float x = a.X - b.X, y = a.Y - b.Y, z = a.Z - b.Z;
        return (float)sqrt(x * x + z * z + y * y);
    }
    void ComputeContraction(Contraction& c)
    {
        unsigned long vertex1 = c.Subs[0].Vertex1;
        unsigned long vertex2 = c.Subs[0].Vertex2;
        if (c.Unsplit) { ++SelfContractions; return; }
        Vec p1 = Positions[vertex1], p2 = Positions[vertex2];
        Vec position = { (p1.X + p2.X) * 0.5f, (p1.Y + p2.Y) * 0.5f, (p1.Z + p2.Z) * 0.5f };
        bool fixed1 = false;
        if (!Free[vertex1]) { fixed1 = true; position = p1; }
        bool fixed2 = false;
        if (!Free[vertex2]) { fixed2 = true; position = p2; }
        c.Result = position;
        float d1 = Distance(position, p1), d2 = Distance(position, p2), total = d1 + d2;
        c.Weight2 = total > 0.0f ? d1 / total : 0.5f;
        float cost = 0.0f;
        for (unsigned long j = 0; j < c.Subs.size(); ++j)
            if (!(cost > SubCosts[j])) cost = SubCosts[j];
        if (fixed1) { if (fixed2) cost += HighPenalty; }
        else if (fixed2) {
            c.Weight2 = 1.0f - c.Weight2;
            for (unsigned long k = 0; k < c.Subs.size(); ++k) { unsigned long t = c.Subs[k].Vertex1; c.Subs[k].Vertex1 = c.Subs[k].Vertex2; c.Subs[k].Vertex2 = t; }
        }
        c.Cost = cost;
    }
};
int main()
{
    Primitive p; p.HighPenalty = 100.0f; p.SelfContractions = 0;
    Vec a = { 0, 0, 0 }, b = { 2, 0, 0 }; p.Positions.push_back(a); p.Positions.push_back(b);
    p.Free.push_back(true); p.Free.push_back(true); p.SubCosts.push_back(3.0f); p.SubCosts.push_back(5.0f);
    Sub s01 = { 0, 1 }, s10 = { 1, 0 };
    Contraction c; c.Unsplit = false; c.Subs.push_back(s01); c.Subs.push_back(s10);
    p.ComputeContraction(c);
    int ok = c.Result.X == 1.0f && c.Weight2 == 0.5f && c.Cost == 5.0f;
    p.Free[1] = false; p.ComputeContraction(c);                        // second end fixed: flip and swap
    ok = ok && c.Result.X == 2.0f && c.Weight2 == 0.0f && c.Subs[0].Vertex1 == 1 && c.Subs[1].Vertex1 == 0 && c.Cost == 5.0f;
    p.Free[0] = false; p.ComputeContraction(c);                        // both fixed: high penalty, no swap
    ok = ok && c.Cost == 105.0f && c.Subs[0].Vertex1 == 1;
    c.Unsplit = true; p.ComputeContraction(c);
    ok = ok && p.SelfContractions == 1;
    printf(ok ? "00ac92c0_TEST PASS\n" : "00ac92c0_TEST FAIL\n");
    return ok ? 0 : 1;
}