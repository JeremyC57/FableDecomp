extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);   // IAT 0x0143FE30 = KERNEL32.dll!CloseHandle
struct S { void* p; };
void __fastcall OnReadFinished(S* self){ if(self->p) CloseHandle(self->p); }