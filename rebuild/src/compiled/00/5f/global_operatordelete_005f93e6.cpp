#pragma optimize("s",on)
extern void __stdcall del_helper_005f8e12(void* p, int a, int b);
void __stdcall operator_delete(void* p){ del_helper_005f8e12(p, 0, 0); }