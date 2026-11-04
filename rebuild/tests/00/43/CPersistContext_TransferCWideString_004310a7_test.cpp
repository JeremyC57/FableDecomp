// Behaviour test for CPersistContext::Transfer<CWideString>(value) (0x004310A7): a model of the row -- mode 2 reads the value from the
// load stream into the argument, mode 3 writes the argument to the save stream, any other mode touches neither; CheckCRC("")
// runs first in every mode and the traits default is constructed and destroyed once.
#include <stdio.h>
#include <string>
static int g_crc, g_ctor, g_dtor;
struct Str { std::string s; };
struct CPersistTraits { Str DefaultVal; CPersistTraits() : DefaultVal() { ++g_ctor; } ~CPersistTraits() { ++g_dtor; } };
struct LoadStream { std::string next; int reads; Str Read() { ++reads; Str r; r.s = next; return r; } };
struct SaveStream { std::string written; int writes; void Write(const Str& v) { ++writes; written = v.s; } };
struct CPersistContext {
    long Mode; LoadStream* PLoadStream; SaveStream* PSaveStream;
    void CheckCRC(const char* name) { if (name[0] == 0) ++g_crc; }
    void Transfer(Str& value)
    {
        CheckCRC("");
        CPersistTraits traits;
        switch (Mode) {
        case 2: value = PLoadStream->Read(); return;
        case 3: PSaveStream->Write(value); return;
        }
    }
};
int main()
{
    LoadStream in; in.next = "loaded"; in.reads = 0;
    SaveStream out; out.writes = 0;
    CPersistContext c; c.PLoadStream = &in; c.PSaveStream = &out;
    Str v; v.s = "start";
    c.Mode = 2; c.Transfer(v); int ok = v.s == "loaded" && in.reads == 1 && out.writes == 0;
    v.s = "saved"; c.Mode = 3; c.Transfer(v); ok = ok && out.written == "saved" && out.writes == 1 && in.reads == 1;
    c.Mode = 0; c.Transfer(v); ok = ok && in.reads == 1 && out.writes == 1 && v.s == "saved";
    ok = ok && g_crc == 3 && g_ctor == 3 && g_dtor == 3;
    printf(ok ? "004310a7_TEST PASS\n" : "004310a7_TEST FAIL\n");
    return ok ? 0 : 1;
}