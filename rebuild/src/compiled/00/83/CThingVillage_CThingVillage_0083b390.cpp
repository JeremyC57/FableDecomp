struct C {};
extern int g_vtbl;
extern void __fastcall base_dtor(C*);
void __fastcall Dtor(C* village){ *(void**)village = &g_vtbl; base_dtor(village); }