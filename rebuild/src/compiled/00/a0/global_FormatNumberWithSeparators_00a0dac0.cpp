// FormatNumberWithSeparators(long value, unsigned short* text) -- write `value` in decimal into `text` and insert the locale's thousands separator between every group of three digits, working back from the terminator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0DAC0, length 124 bytes.
// Callees: MSVCR71 _itow @ 0x00BFEDEE, wcslen @ 0x00BFED94, GetThousandsSeparator @ 0x00A0DA70 (same TU).
// Caller: the string builder 0x009C7A60 (0x009C7AF5: value in ecx, a frame buffer in edx). Name descriptive.
// Shape: `__fastcall` (value in ecx, buffer in edx -> ebx, plain `ret`); both CRT calls' arguments popped by one
// `add esp,10h`; the group count `(length - 1) / 3` is the signed reciprocal multiply; the separator lives in the
// `push ecx` frame word; the copy runs from the terminator down with a source and a destination pointer, the
// character copied BEFORE the `i != 0 && i % 3 == 0` test (VC7.1 knows the counter is non-negative and divides
// unsigned: `xor edx,edx ; mov ebp,3 ; div ebp`), the separator stored through a pre-decremented destination; the for-header steps `++i, --dest, --src` in
// retail's `inc ecx ; sub esi,2 ; sub ebx,2` order (body-level decrements put the `inc` last).
// Leaf: ?FormatNumberWithSeparators@@YIXJPAG@Z
typedef unsigned int size_t;
extern "C" unsigned short* __cdecl _itow(int value, unsigned short* text, int radix);   // 0x00BFEDEE
extern "C" size_t __cdecl wcslen(const unsigned short* text);                           // 0x00BFED94
unsigned short GetThousandsSeparator();                                                  // 0x00A0DA70

void __fastcall FormatNumberWithSeparators(long value, unsigned short* text)
{
    _itow(value, text, 10);
    long length = wcslen(text);
    long separators = (length - 1) / 3;
    if (separators > 0) {
        unsigned short separator = GetThousandsSeparator();
        unsigned short* dest = text + length + separators;
        unsigned short* src = text + length;
        for (long i = 0; i < length; ++i, --dest, --src) {
            *dest = *src;
            if (i != 0 && i % 3 == 0)
                *--dest = separator;
        }
    }
}