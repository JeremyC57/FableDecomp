#ifndef FABLE_ENGINE_CEDGESTIPVERTEXARRAY_H
#define FABLE_ENGINE_CEDGESTIPVERTEXARRAY_H
// Retail layout (hand-written, chunk 706): Ego_r.pdb class template CEdgeStipVertexArray<T> (class key V in
// ghidra_out/pdb_class_keys.tsv; the Lionhead spelling "Stip" is kept), 8 bytes: Data +0x00, Count +0x04 (struct_layouts_egor.tsv,
// every instantiation over the CPatchTesselationEdgeStrip vertex structs). No member functions are modelled yet.
template <class T> class CEdgeStipVertexArray {
public:
    T*   Data;                                                // +0x00
    long Count;                                               // +0x04
};

#endif // FABLE_ENGINE_CEDGESTIPVERTEXARRAY_H
