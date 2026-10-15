#include <cstdio>
#include <cstring>
struct Str { char b[64]; Str() { b[0] = 0; } Str(const char* s) { std::strcpy(b, s); } };
static Str Cat(const char* l, const Str& r) { Str s(l); std::strcat(s.b, r.b); return s; }
static Str Cat(const Str& l, const char* r) { Str s(l); std::strcat(s.b, r); return s; }
struct Stream { long pos; long log[8]; int n; long seeks[4]; int ns;
    long GetPosition() { return pos; } void Seek(long p) { seeks[ns++] = p; pos = p; } void WriteLong(long v) { log[n++] = v; pos += 4; } };
struct Section { Str Name; long StartPosition; };
struct Ctx {
    long Mode; Str* Text; Stream* Save;
    void EndSection(Section& section)
    {
        if (Mode == 3) {
            long end = Save->GetPosition();
            long length = end - section.StartPosition;
            Save->Seek(section.StartPosition - 4);
            Save->WriteLong(length);
            Save->Seek(end);
        } else if (Mode == 1) { Str t = Cat(Cat("END_", section.Name), "\r\n\r\n"); std::strcat(Text->b, t.b); }
    }
};
int main()
{
    Stream st; st.pos = 50; st.n = 0; st.ns = 0; Str text; Ctx c; c.Text = &text; c.Save = &st;
    Section s; s.Name = Str("Q"); s.StartPosition = 20;
    c.Mode = 3; c.EndSection(s);
    int ok = st.n == 1 && st.log[0] == 30 && st.ns == 2 && st.seeks[0] == 16 && st.seeks[1] == 50 && st.pos == 50;
    c.Mode = 1; c.EndSection(s);
    ok = ok && std::strcmp(text.b, "END_Q\r\n\r\n") == 0;
    if (!ok) { std::printf("009babf0_TEST FAIL\n"); return 1; }
    std::printf("009babf0_TEST PASS\n");
    return 0;
}
