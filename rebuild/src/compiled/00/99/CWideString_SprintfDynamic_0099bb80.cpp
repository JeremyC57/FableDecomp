// CWideString::SprintfDynamic(const unsigned short* format, ...) -- format into a wide buffer sized from the format length (+0x1000) and assign the result to this string
// Inline-budget: /d2inlT200 expands the real string assignment and cleanup, which plain /O2 leaves called.
// Leaf: ?SprintfDynamic@CWideString@@QAAXPBGZZ
// Retail: 0x0099bb80, 197 bytes, variadic cdecl member; explicit this on the stack.
// Flags: /d2inlT200
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_SPRINTFDYNAMIC
#define FABLE_RETAIL_STLP_VECTOR
#include <stdarg.h>
#include <wchar.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
inline const CWideString& CWideString::operator=(const wchar_t* text)
{
    UnassignString();
    if (text != 0 && *text != 0)
        PStringData = AllocStringData(text, -1);
    return *this;
}
void CWideString::SprintfDynamic(const wchar_t* format, ...)
{
    std::vector<unsigned short> buffer;
    const unsigned int& length = wcslen(format);
    int size = length + 0x1000;
    buffer.resize(size);
    va_list args;
    va_start(args, format);
    int count = _vsnwprintf(&buffer[0], size - 1, format, args);
    buffer[size - 1] = 0;
    if (count <= size)
        *this = &buffer[0];
    va_end(args);
}