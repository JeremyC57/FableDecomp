#ifndef FABLE_ENGINE_CEDGESTIPVERTEXARRAY_H
#define FABLE_ENGINE_CEDGESTIPVERTEXARRAY_H
// Retail layout (hand-written, chunk 706): Ego_r.pdb class template CEdgeStipVertexArray<T> (class key V in
// ghidra_out/pdb_class_keys.tsv; the Lionhead spelling "Stip" is kept), 8 bytes: Data +0x00, Count +0x04 (struct_layouts_egor.tsv,
// every instantiation over the CPatchTesselationEdgeStrip vertex structs). No member functions are modelled yet.
template <class T> class CEdgeStipVertexArray {
public:
    T*   Data;                                                // +0x00
    long Count;                                               // +0x04
#ifdef FABLE_RETAIL_CEDGESTIPVERTEXARRAY_ACCESSORS
    // Retail opt-in (chunk 785, rows 0x00BE4660 / 0x00BE4FE0 CWaterPatchTesselatorBase<T>::TesselateEdge): Ego_r.pdb
    // ??A?$CEdgeStipVertexArray@U..@@QAEAAU..@J@Z / QBEABU..@J@Z and ?Size@..@@QBEJXZ; no out-of-line copy in retail (the
    // water tesselators index Data with `imul 3Ch` / `lea [i*5]*4` in place).
    T& operator[](long index) { return Data[index]; }
    const T& operator[](long index) const { return Data[index]; }
    long Size() const { return Count; }
#endif
};

#endif // FABLE_ENGINE_CEDGESTIPVERTEXARRAY_H
