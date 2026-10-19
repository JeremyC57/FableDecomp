// CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocks @ 0x01228CE0 (retail, 27 bytes; catalog name propagated, not this body's identity).
// True shape: the compiler-generated at-exit destructor of a static STLport list at 0x013B8A24:
// the inline ~list() clears the nodes (out-of-line clear @ 0x004CC370) and frees the sentinel node with
// free (MSVCR71 thunk 0x00BFEA14; STLport's node allocator is malloc/free). The list object and its one
// word (the sentinel pointer) are the same address, so one global models both reads. __fastcall, no args.
struct CList_004cc370 {                    // STLport list whose nodes hold a CIntelligentPointer at +8 (node: next, prev, value); clear @ 0x004CC370
    void* _M_node;                          // +0x0 sentinel node
    void clear();                           // 0x004CC370
};
extern CList_004cc370 g_List_013b8a24;             // 0x013B8A24
extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14
void __fastcall RenderMeshPrimitiveBlocks()
{
    g_List_013b8a24.clear();
    if (g_List_013b8a24._M_node != 0)
        free(g_List_013b8a24._M_node);
}
