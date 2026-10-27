// behaviour test for 0x004065f0 CUserProfileManager::GetManualSaveBaseFileName() const: returns a new wide string "Save"
#include <cstdio>
struct WStr { unsigned short buf[16]; int len; };
static WStr MakeW(const char* s) { WStr w; w.len = 0; while (s[w.len]) { w.buf[w.len] = (unsigned short)s[w.len]; ++w.len; } w.buf[w.len] = 0; return w; }
struct Mgr { WStr GetManualSaveBaseFileName() const { return MakeW("Save"); } };
int main()
{
    int fails = 0;
    Mgr m;
    WStr s = m.GetManualSaveBaseFileName();
    if (s.len != 4 || s.buf[0] != 'S' || s.buf[1] != 'a' || s.buf[2] != 'v' || s.buf[3] != 'e' || s.buf[4] != 0) fails |= 1;
    WStr t = m.GetManualSaveBaseFileName();
    if (t.len != s.len) fails |= 2;
    if (fails) { std::printf("004065f0_TEST FAIL %d\n", fails); return 1; }
    std::printf("004065f0_TEST PASS\n");
    return 0;
}