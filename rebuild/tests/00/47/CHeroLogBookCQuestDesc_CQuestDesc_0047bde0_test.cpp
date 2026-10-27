// behaviour test for 0x0047bde0 CHeroLogBook::CQuestDesc copy constructor: memberwise copy -- ScriptID, the text
// (string copy ctor), QuestStatus and the sub-objective vector (vector copy ctor, a new block)
#include <cstdio>
static int gStrCopies = 0, gVecCopies = 0;
struct Str { const char* p; Str(const char* s) : p(s) { } Str(const Str& o) : p(o.p) { ++gStrCopies; } };
struct Vec { int n; int* data; Vec() : n(0), data(0) { } Vec(const Vec& o) : n(o.n), data(o.n ? new int[o.n] : 0) { for (int i = 0; i < n; ++i) data[i] = o.data[i]; ++gVecCopies; } ~Vec() { delete[] data; } };
struct QuestDesc { unsigned int ScriptID; Str Text; int Status; Vec Subs;
    QuestDesc() : ScriptID(999), Text("No description yet"), Status(0) { }
    QuestDesc(const QuestDesc& o) : ScriptID(o.ScriptID), Text(o.Text), Status(o.Status), Subs(o.Subs) { } };
int main()
{
    int fails = 0;
    QuestDesc a; a.ScriptID = 77; a.Status = 2; a.Subs.n = 2; a.Subs.data = new int[2]; a.Subs.data[0] = 5; a.Subs.data[1] = 6;
    QuestDesc b(a);
    if (b.ScriptID != 77 || b.Status != 2 || b.Text.p != a.Text.p) fails |= 1;
    if (gStrCopies != 1 || gVecCopies != 1) fails |= 2;
    if (b.Subs.n != 2 || b.Subs.data == a.Subs.data || b.Subs.data[1] != 6) fails |= 4;
    if (fails) { std::printf("0047bde0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0047bde0_TEST PASS\n");
    return 0;
}