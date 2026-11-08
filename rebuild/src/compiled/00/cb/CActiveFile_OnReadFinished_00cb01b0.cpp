// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/wxutil.cpp) ??1CAMEvent@@QAE@XZ -- 14 bytes verified against Fable.exe 0x00CB01B0
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);   // IAT 0x0143FE30 = KERNEL32.dll!CloseHandle
struct S { void* p; };
void __fastcall OnReadFinished(S* self){ if(self->p) CloseHandle(self->p); }