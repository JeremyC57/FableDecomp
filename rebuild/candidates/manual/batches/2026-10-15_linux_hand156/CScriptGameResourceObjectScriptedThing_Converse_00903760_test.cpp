// Behaviour test for 0x00903760: CScriptGameResourceObjectScriptedThing::Converse: speak the text group at the other thing's script thing (method 0, true, true, false)
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct ScriptThing { int id; };
struct Base { virtual ~Base() {} int thing; virtual ScriptThing GetScriptThing() const { ScriptThing s; s.id = thing; return s; }
    int spokeTo; const char* text; int method; bool b1, b2, b3;
    virtual void Speak(const ScriptThing& t, const char* g, int m, bool x1, bool x2, bool x3) { spokeTo = t.id; text = g; method = m; b1 = x1; b2 = x2; b3 = x3; } };
struct Thing : Base { void Converse(const Base& other, const char* g) { Speak(other.GetScriptThing(), g, 0, true, true, false); } };
static void Run()
{
    Thing me; me.thing = 1; me.spokeTo = 0; Base other; other.thing = 42;
    me.Converse(other, "TEXT_HELLO");
    CHECK(me.spokeTo == 42 && std::strcmp(me.text, "TEXT_HELLO") == 0 && me.method == 0 && me.b1 && me.b2 && !me.b3);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00903760_TEST FAIL\n"); return 1; }
    std::printf("00903760_TEST PASS\n");
    return 0;
}
