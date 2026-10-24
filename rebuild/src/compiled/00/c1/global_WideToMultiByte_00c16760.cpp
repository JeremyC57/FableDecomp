// _global::WideToMultiByte(const unsigned short* source, char* dest, int destSize) -- converts a wide string into the caller's ANSI buffer (code page 0, WC_NO_BEST_FIT_CHARS, " " as the default character, at most destSize-1 bytes) and terminates it; a zero size writes nothing
// Leaf: WideToMultiByte
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16760, 73 bytes (+ int3 padding), __fastcall (source in ecx, dest in
//   edx, the size on the stack, `ret 4`).
// Callers: 0x00C08F84, 0x00C17374, 0x00C17520, 0x00C176A7 (the bank-file loader, size 0x100) and 0x00C73B29.
// Callees: MSVCR71 wcslen (import 0x01440084), KERNEL32 WideCharToMultiByte (import 0x0143FD98).
// Shape: the "used default char" flag is a local homed in the dead size slot (`lea ecx,[esp+0Ch] ; push ecx`); the
//   arguments push right to left with wcslen evaluated in the middle (after the buffer pointer); a negative result
//   clears the first byte (its own epilogue), otherwise the terminator lands at dest[n].
// Label: the catalog's `String_WideCharToMultiByte` is descriptive.
extern "C" __declspec(dllimport) unsigned int __cdecl wcslen(const unsigned short* text);   // 0x01440084
extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int codePage, unsigned long flags,
    const unsigned short* wide, int wideCount, char* multi, int multiSize, const char* defaultChar,
    int* usedDefaultChar);                                     // 0x0143FD98
#pragma function(wcslen)

void __fastcall WideToMultiByte(const unsigned short* source, char* dest, int destSize)
{
    if (destSize) {
        int usedDefault;
        int written = WideCharToMultiByte(0, 0x400, source, wcslen(source), dest, destSize - 1, " ", &usedDefault);
        if (written < 0) {
            dest[0] = 0;
            return;
        }
        dest[written] = 0;
    }
}