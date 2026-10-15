#include <cstdio>
#include <cstring>
struct Str { char b[64]; Str() { b[0] = 0; } Str(const char* s) { std::strcpy(b, s); } };
static Str Cat(const char* l, const Str& r) { Str s(l); std::strcat(s.b, r.b); return s; }
static Str Cat(const Str& l, const char* r) { Str s(l); std::strcat(s.b, r); return s; }
struct Stream { long pos; long log[8]; int n; long GetPosition() { return pos; } void WriteLong(long v) { log[n++] = v; pos += 4; } void WriteString(const Str& s) { log[n++] = (long)std::strlen(s.b); pos += (long)std::strlen(s.b) + 1; } };
struct Section { Str Name; long StartPosition; };
struct Ctx {
    long Mode; Str* Text; Stream* Save;
    void StartSection(Section& section, const Str& name)
    {
        section.Name = name;
        if (Mode == 3) { Save->WriteString(name); Save->WriteLong(0); section.StartPosition = Save->GetPosition(); }
        else if (Mode == 1) { Str t = Cat(Cat("START_", section.Name), "\r\n"); std::strcat(Text->b, t.b); }
    }
};
int main()
{
    Stream st; st.pos = 10; st.n = 0; Str text; Ctx c; c.Text = &text; c.Save = &st; Section s;
    c.Mode = 3; c.StartSection(s, Str("AB"));
    int ok = st.n == 2 && st.log[0] == 2 && st.log[1] == 0 && s.StartPosition == 17 && std::strcmp(s.Name.b, "AB") == 0;
    c.Mode = 1; c.StartSection(s, Str("CD"));
    ok = ok && std::strcmp(text.b, "START_CD\r\n") == 0;
    if (!ok) { std::printf("009bab60_TEST FAIL\n"); return 1; }
    std::printf("009bab60_TEST PASS\n");
    return 0;
}