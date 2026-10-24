// Leaf: __RTC_Initialize
// Flags: /O1
// Grade: RELOCATION_MATCH 68/68 on the real span with /O1 (catalog row 61 stopped before the __SEH_epilog tail)
// _RTC_Initialize() -- the VC7.1 CRT walk over the .rtc$I initializer table (initsect.cpp): call every non-null entry
//   between __rtc_iaa and __rtc_izz, each inside its own __try/__except(EXCEPTION_EXECUTE_HANDLER) so a faulting
//   initializer cannot abort the walk.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004012CE, 68 bytes (the catalog row stopped at 61, before the `call __SEH_epilog ; ret`
//   tail), plain `ret`; table 0x01368E58 .. 0x01368F58. Its twin _RTC_Terminate follows at 0x00401312 (table 0x01369060).
// Why it differed: the CRT objects are size-optimised. Under /O1 VC7.1 builds the SEH frame with the shared helpers
//   __SEH_prolog 0x0040138C / __SEH_epilog 0x004013C7 (push 0Ch ; push scopetable ; call) and keeps the cursor in the
//   frame slot [ebp-1Ch]; under the default /O2 it inlines the fs:[0] frame (120 bytes). The source was right, the flags
//   were not.
typedef void (__cdecl* _PVFV)();

extern "C" _PVFV __rtc_iaa[];                 // 0x01368E58 (.rtc$IAA)
extern "C" _PVFV __rtc_izz[];                 // 0x01368F58 (.rtc$IZZ)

extern "C" void __cdecl _RTC_Initialize()
{
    for (_PVFV* f = __rtc_iaa; f < __rtc_izz; ++f) {
        __try {
            if (*f)
                (**f)();
        }
        __except (1) {
        }
    }
}
