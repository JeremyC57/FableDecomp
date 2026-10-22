struct T { void DtorHelper(int flag); };
void __fastcall Dest_val(T* inventory){ inventory->DtorHelper(1); }