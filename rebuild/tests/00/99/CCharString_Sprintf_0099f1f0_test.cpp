#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
// Behaviour model of 0x0099F1F0 CCharString::Sprintf (a __cdecl varargs member).
struct Str { char* text; Str() : text(0) {} void Unassign() { std::free(text); text = 0; } void Sprintf(const char* format, ...); };
void Str::Sprintf(const char* format, ...)
{
    Str* result = this;
    long size = (long)std::strlen(format) + 0x400;
    char* buffer = size ? (char*)std::malloc(size) : 0;
    std::memset(buffer, 0, size);
    va_list args;
    va_start(args, format);
    int count = std::vsprintf(buffer, format, args);
    va_end(args);
    buffer[size - 1] = 0;
    if (count <= size) {
        if (result->text) result->Unassign();
        if (buffer == 0) return;
        if (*buffer) { result->text = (char*)std::malloc(std::strlen(buffer) + 1); std::strcpy(result->text, buffer); }
    }
    if (buffer) std::free(buffer);
}
int main()
{
    Str s;
    s.Sprintf("%d-%s", 42, "ok");
    bool ok1 = s.text && std::strcmp(s.text, "42-ok") == 0;
    s.Sprintf("%s", "");
    bool ok2 = s.text == 0;
    if (!ok1 || !ok2) { std::printf("0099f1f0_TEST FAIL\n"); return 1; }
    std::printf("0099f1f0_TEST PASS\n");
    return 0;
}