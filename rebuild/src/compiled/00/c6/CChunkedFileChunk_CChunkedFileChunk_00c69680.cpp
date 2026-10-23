struct C {};
extern int g_vtbl_012b9578;
extern void __fastcall base_dtor_00c74450(C*);
void __fastcall Dtor(C* self){ *(void**)self = &g_vtbl_012b9578; base_dtor_00c74450(self); }