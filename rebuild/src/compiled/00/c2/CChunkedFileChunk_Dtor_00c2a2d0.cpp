extern char g_vtbl[];
// Leaf: ??1CChunkedFileChunk@
struct Base { void BaseDtor(); };
void __fastcall Dtor(void* self){ *(void**)self = (void*)g_vtbl; ((Base*)((char*)self + 4))->BaseDtor(); }