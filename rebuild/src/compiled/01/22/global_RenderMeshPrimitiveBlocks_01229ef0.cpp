// CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocks @ 0x01229EF0 (retail, 27 bytes; catalog name propagated, not this body's identity).
// True shape: the compiler-generated at-exit destructor of a static STLport list at 0x013BAF14:
// the inline ~list() clears the nodes (out-of-line clear @ 0x004B8050) and frees the sentinel node with
// free (MSVCR71 thunk 0x00BFEA14; STLport's node allocator is malloc/free). The list object and its one
// word (the sentinel pointer) are the same address, so one global models both reads. __fastcall, no args.
struct CRetailList_004b8050 {                    // STLport list (one word: the sentinel node); clear @ 0x004B8050 (label clear_CRetailList_004b8050)
    void* _M_node;                          // +0x0 sentinel node
    void clear();                           // 0x004B8050
};
extern CRetailList_004b8050 g_List_013baf14;             // 0x013BAF14
extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14
void __fastcall RenderMeshPrimitiveBlocks()
{
    g_List_013baf14.clear();
    if (g_List_013baf14._M_node != 0)
        free(g_List_013baf14._M_node);
}
