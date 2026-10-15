// Behaviour test for CTCBase::EndSerialising (0x006869E0): in text mode "End" + class name + ";\r\n" is appended.
#include <cstdio>
#include <cstring>
struct Str {
    char buf[128];
    Str() { buf[0] = 0; }
    Str(const char* s) { std::strcpy(buf, s); }
    Str& operator+=(const Str& o) { std::strcat(buf, o.buf); return *this; }
};
static Str operator+(const char* l, const Str& r) { Str s(l); s += r; return s; }
static Str operator+(const Str& l, const char* r) { Str s = l; s += Str(r); return s; }
struct TC {
    Str GetClassTypeName() const { return Str("CTCDoor"); }
    void EndSerialising(bool textMode, Str& out)
    {
        if (textMode) {
            Str text = "End" + GetClassTypeName() + ";\r\n";
            out += text;
        }
    }
};
int main()
{
    TC t; Str out("x;");
    t.EndSerialising(false, out);
    if (std::strcmp(out.buf, "x;") != 0) { std::printf("006869e0_TEST FAIL\n"); return 1; }
    t.EndSerialising(true, out);
    if (std::strcmp(out.buf, "x;EndCTCDoor;\r\n") != 0) { std::printf("006869e0_TEST FAIL\n"); return 1; }
    std::printf("006869e0_TEST PASS\n");
    return 0;
}