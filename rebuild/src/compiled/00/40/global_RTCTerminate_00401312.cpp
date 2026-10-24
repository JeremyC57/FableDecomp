// _global::_RTC_Terminate() -- CRT run-time-check teardown: call every non-null callback in the .rtc$T table, each under its own __try/__except(EXCEPTION_EXECUTE_HANDLER)
// Leaf: _RTC_Terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00401312, 68 bytes. Callees: __SEH_prolog 0x0040138C,
// __SEH_epilog 0x004013C7 (CRT), the scope table at 0x0122D040, table bounds 0x01369060/0x01369160.
// Pragma: optimize("s", on) -- the CRT is built /O1, which is what routes the frame through
// __SEH_prolog (push 0xc ; push scopetable ; call __SEH_prolog) instead of an inline SEH frame.
//
// Retail bytes:  push 0xc ; push scopetable ; call __SEH_prolog ; mov [f],__rtc_taa ;
//   L: cmp [f],__rtc_tzz ; jae X ; and [trylevel],0 ; mov eax,[f] ; mov eax,[eax] ; test eax,eax ;
//   je N ; call eax ; jmp N ; filter: xor eax,eax ; inc eax ; ret ; handler: mov esp,[ebp-0x18] ;
//   N: or [trylevel],-1 ; add [f],4 ; jmp L ; X: call __SEH_epilog ; ret
// The manifest label `Init_CallFunctionPointers` is a descriptive guess; the identity is the
// VC7.1 CRT `_RTC_Terminate` (initsect.cpp): WinMainCRTStartup registers this address with
// `push 0x401312 ; call atexit` (0x00401147), right after the twin loop at 0x004012CE (the same
// body over 0x01368E58..0x01368F58 = _RTC_Initialize). The loop variable lives in the frame
// (SEH-visible); the filter returns 1 (EXCEPTION_EXECUTE_HANDLER) and the handler is empty.
#pragma optimize("s", on)

typedef void (__cdecl *_PVFV)(void);

extern "C" _PVFV __rtc_taa[];                     // 0x01369060
extern "C" _PVFV __rtc_tzz[];                     // 0x01369160

extern "C" void __cdecl _RTC_Terminate(void)
{
    _PVFV* f;
    for (f = __rtc_taa; f < __rtc_tzz; f++)
    {
        __try {
            if (*f)
                (**f)();
        } __except (1) {
        }
    }
}