struct C {};
// Leaf: ??1CChunkedFileChunk@
extern int g_vtbl_0125be0c;
extern void __fastcall base_dtor_0099a300(C*);
void __fastcall Dtor(C* self){ *(void**)self = &g_vtbl_0125be0c; base_dtor_0099a300(self); }