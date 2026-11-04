// Behaviour test for 0x00441EE0 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
static int g_released = 0;
struct Handle { int Id; bool Unique; };
struct Set {
    std::vector<Handle> Handles;
    ~Set() { for (size_t i = 0; i < Handles.size(); ++i) if (Handles[i].Unique) ++g_released; }
};
int main() {
    { Set s; Handle a = { 1, true }, b = { 2, false }, c = { 3, true }; s.Handles.push_back(a); s.Handles.push_back(b); s.Handles.push_back(c); }
    bool ok = g_released == 2;
    { Set e; }
    ok = ok && g_released == 2;
    std::printf(ok ? "00441ee0_TEST PASS\n" : "00441ee0_TEST FAIL\n");
    return ok ? 0 : 1;
}
