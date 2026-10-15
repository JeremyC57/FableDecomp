// COptimisedFace::RemapIndex(long, long) -- replaces every corner of the triangle that refers to vertex `from` with vertex `to`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC6F50, length 35 bytes. No callees.
// (QuadricOptimiserInternals::COptimisedFace in the PDB; modelled without the namespace like the catalog row.)
// A face starts with its three vertex indices (+0, +4, +8); each corner is compared with `from`
// and overwritten with `to` independently (three compare/store pairs, `ret 8` = two arguments).
// The only caller is the mesh optimiser at 0x00ACCCAB (the vertex-pair contraction).
struct COptimisedFace {
    long m_vertex[3];                       // +0x00 vertex indices of the three corners
    void RemapIndex(long from, long to);
};

void COptimisedFace::RemapIndex(long from, long to)
{
    if (m_vertex[0] == from) m_vertex[0] = to;
    if (m_vertex[1] == from) m_vertex[1] = to;
    if (m_vertex[2] == from) m_vertex[2] = to;
}
