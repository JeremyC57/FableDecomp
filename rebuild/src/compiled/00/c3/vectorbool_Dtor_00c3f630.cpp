extern char g_vtbl_012b5868[];
extern void __fastcall vb_helper_00c3f490(void*);
void __fastcall Dtor(void* self){ *reinterpret_cast<void**>(reinterpret_cast<char*>(self)+0x1f0) = (void*)g_vtbl_012b5868; vb_helper_00c3f490(self); }