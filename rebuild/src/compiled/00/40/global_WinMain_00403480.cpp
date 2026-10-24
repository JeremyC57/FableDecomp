// _global::WinMain(HINSTANCE instance, HINSTANCE previousInstance, char* commandLine, int showCommand) -- single-instance guard (named mutex), carve the micro-thread stack out of the WinMain frame, run GFMain
// Leaf: _WinMain@16
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00403480, 141 bytes (+3 int3 pad). Callees: __chkstk
// 0x00BFEA30, kernel32 OpenMutexW [0x0143FE24] / CreateMutexW [0x0143FE28], MicroThread SetStack
// 0x009D86B0 (__fastcall top, size), GFMain 0x00402510 (__fastcall instance, commandLine,
// showCommand), __security_check_cookie 0x00BFE9F9; cookie 0x0139C8A8, mutex name 0x0122DC28,
// stack-bound globals 0x013B7C84 (top) / 0x013B7C88 (bottom).
// Flag: /GS -- the frame carries the stack cookie (`mov eax,[cookie] ; mov [esp+0x32010],eax`
// before the first call, `call __security_check_cookie` before the epilogue).
//
// Retail bytes:  mov eax,0x32008 ; call __chkstk ; mov eax,[cookie] ; push name ; push 0 ;
//   push 0x1f0001 ; mov [cookie slot],eax ; call [OpenMutexW] ; test eax,eax ; jne X ; push name ;
//   push eax ; push eax ; call [CreateMutexW] ; lea eax,[esp+0x32004] ; lea ecx,[esp+3] ;
//   mov [bottom],ecx ; mov edx,0x32000 ; mov ecx,eax ; mov [top],eax ; call SetStack ;
//   push showCommand ; mov edx,commandLine ; mov ecx,instance ; call GFMain ; X: cookie check ;
//   xor eax,eax ; add esp,0x32008 ; ret 0x10
// The 0x32004-byte frame buffer starts with 3 bytes the micro-thread stack does not use (the
// bottom is the buffer + 3, the top its end): a {3-byte pad, 0x32001-byte stack} record, the
// SetStack size 0x32000. The CreateMutexW handle is deliberately never closed (it is the
// process-lifetime instance lock). An earlier lane landed this body as Global_WinMain_00403480.cpp
// (fable_boot.h); this TU is self-contained.
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
typedef unsigned short WCHAR;                     // VC7.1 default: wchar_t is not a native type (no /Zc:wchar_t)

extern "C" __declspec(dllimport) void* __stdcall OpenMutexW(unsigned long desiredAccess, int inheritHandle, const WCHAR* name);   // [0x0143FE24]
extern "C" __declspec(dllimport) void* __stdcall CreateMutexW(void* attributes, int initialOwner, const WCHAR* name);           // [0x0143FE28]

namespace MicroThread {
    void __fastcall SetStack(unsigned char* stackTop, unsigned long stackSize);   // 0x009D86B0
}

extern "C" long __fastcall GFMain(HINSTANCE instance, char* commandLine, int showCommand);   // 0x00402510

extern unsigned char* g_MicroThreadStackTop;      // 0x013B7C84
extern unsigned char* g_MicroThreadStackBottom;   // 0x013B7C88

struct SWinMainStackFrame {
    unsigned char m_alignmentPad[3];
    unsigned char m_microThreadStack[0x32001];
};

extern "C" int __stdcall WinMain(HINSTANCE instance, HINSTANCE previousInstance, char* commandLine, int showCommand)
{
    SWinMainStackFrame frame;
    if (OpenMutexW(0x001F0001, 0, L"Global\\Fable: The Lost Chapters") == 0)
    {
        CreateMutexW(0, 0, L"Global\\Fable: The Lost Chapters");
        g_MicroThreadStackTop = frame.m_microThreadStack + sizeof(frame.m_microThreadStack);
        g_MicroThreadStackBottom = frame.m_microThreadStack;
        MicroThread::SetStack(g_MicroThreadStackTop, 0x32000);
        GFMain(instance, commandLine, showCommand);
    }
    return 0;
}