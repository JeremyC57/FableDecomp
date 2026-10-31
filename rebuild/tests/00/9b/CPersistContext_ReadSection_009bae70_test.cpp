// Test: CPersistContext::ReadSection -- binary load: the stored name must equal `name`, then the section stream is the next
// `size` bytes (skipped in the parent); text load: the text between "START_<name>" (past its newline) and "END_<name>"
// becomes the section text, or an empty text and false when the start marker is missing.
#include <cstdio>
#include <string>
enum { MODE_LOAD_TEXT = 0, MODE_LOAD_BINARY = 2 };
struct Section { std::string Text; std::string Stream; std::string Filename; };
struct Context {
    int Mode; std::string Data; size_t Pos;
    bool ReadSection(Section& section, const std::string& name)
    {
        bool result = true;
        if (Mode == MODE_LOAD_BINARY) {
            size_t z = Data.find('\0', Pos); std::string stored = Data.substr(Pos, z - Pos); Pos = z + 1;
            if (stored != name) return false;
            long size = (unsigned char)Data[Pos]; Pos += 4;
            section.Stream = size > 0 ? Data.substr(Pos, size) : std::string(); Pos += size > 0 ? size : 0;
        } else if (Mode == MODE_LOAD_TEXT) {
            size_t s = Data.find("START_" + name, Pos);
            if (s != std::string::npos) {
                size_t nl = Data.find('\n', s); Pos = nl + 1;
                size_t e = Data.find("END_" + name, Pos);
                section.Text = Data.substr(Pos, e - Pos); Pos = e;
            } else { section.Text = std::string(); result = false; }
            section.Filename = "";
        }
        return result;
    }
};
int main()
{
    bool ok = true;
    Context t; t.Mode = MODE_LOAD_TEXT; t.Data = "x START_Foo\nabc def\nEND_Foo"; t.Pos = 0;
    Section s; ok = ok && t.ReadSection(s, "Foo") && s.Text == "abc def\n" && s.Filename.empty();
    Context m; m.Mode = MODE_LOAD_TEXT; m.Data = "nothing"; m.Pos = 0; Section s2; s2.Text = "old";
    ok = ok && !m.ReadSection(s2, "Foo") && s2.Text.empty();
    Context b; b.Mode = MODE_LOAD_BINARY; b.Data = std::string("Bar\0\3\0\0\0xyzQ", 12); b.Pos = 0; Section s3;
    ok = ok && b.ReadSection(s3, "Bar") && s3.Stream == "xyz" && b.Pos == 11;
    Context w; w.Mode = MODE_LOAD_BINARY; w.Data = std::string("Baz\0", 4); w.Pos = 0; Section s4;
    ok = ok && !w.ReadSection(s4, "Bar");
    std::puts(ok ? "009bae70_TEST PASS" : "009bae70_TEST FAIL");
    return ok ? 0 : 1;
}