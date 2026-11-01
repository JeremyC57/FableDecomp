// Test for QuadricOptimiserInternals::COptimisedPrimitive::GetRemainingVertices (0x00AC8840): walk the given index set
// in order and return a new index list holding only the indices whose vertex still Exists.
#include <cstdio>
struct Vertex { bool Exists; };
struct IndexList { unsigned long Indices[8]; unsigned long Count;
    void Insert(unsigned long v)   // std::set<unsigned long>::insert: sorted, unique
    {
        unsigned long i = 0;
        while (i < Count && Indices[i] < v) ++i;
        if (i < Count && Indices[i] == v) return;
        for (unsigned long j = Count; j > i; --j) Indices[j] = Indices[j - 1];
        Indices[i] = v; ++Count;
    } };
struct Primitive { Vertex Vertices[8];
    IndexList GetRemainingVertices(const IndexList& indices) const
    {
        IndexList remaining; remaining.Count = 0;
        for (unsigned long i = 0; i < indices.Count; ++i)
            if (Vertices[indices.Indices[i]].Exists) { unsigned long index = indices.Indices[i]; remaining.Insert(index); }
        return remaining;
    }
};
int main()
{
    Primitive p; for (int i = 0; i < 8; ++i) p.Vertices[i].Exists = (i % 2) == 0;
    IndexList in; in.Count = 0; in.Insert(5); in.Insert(2); in.Insert(4); in.Insert(7); in.Insert(0); in.Insert(2);
    IndexList out = p.GetRemainingVertices(in);
    bool ok = out.Count == 3 && out.Indices[0] == 0 && out.Indices[1] == 2 && out.Indices[2] == 4;
    std::printf(ok ? "00ac8840_TEST PASS\n" : "00ac8840_TEST FAIL\n");
    return ok ? 0 : 1;
}