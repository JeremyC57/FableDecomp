// behaviour test for 00e57020 CBS_Teacher::GetBookSpecificArgs (same branch logic over a tiny argument store)
#include <cstdio>
#include <cstring>
struct Args {
    char key[3][8]; char val[3][48]; int n;
    void set(const char* k, const char* v)
    {
        for (int i = 0; i < n; ++i) if (std::strcmp(key[i], k) == 0) { std::strcpy(val[i], v); return; }
        std::strcpy(key[n], k); std::strcpy(val[n], v); ++n;
    }
    const char* get(const char* k) const { for (int i = 0; i < n; ++i) if (std::strcmp(key[i], k) == 0) return val[i]; return ""; }
};
struct Teacher {
    long postSave; bool twinblade; const char* teddy; float attract, scary; bool terminating;
    bool IsActiveThreadTerminating() { return terminating; }
    void GetBookSpecificArgs(long book, Args& args);
};
static void cat2(char* out, const char* a, const char* b) { std::strcpy(out, a); std::strcat(out, b); }
void Teacher::GetBookSpecificArgs(long book, Args& args)
{
    if (book == 15) {
        if (IsActiveThreadTerminating()) return;
        if (postSave <= 1250) { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK15_90D"); }
        else if (postSave <= 1700) { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK15_90A"); }
        else if (postSave <= 2300) { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK15_90B"); }
        else if (postSave > 2600) { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK15_90B"); }
        else { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK15_90C"); }
    } else if (book == 17) {
        if (IsActiveThreadTerminating()) return;
        if (attract > 0.5) { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK17_20B"); }
        else { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK17_20A"); }
    } else if (book == 19) {
        if (IsActiveThreadTerminating()) return;
        if (postSave <= 700) { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK19_80A"); }
        else if (twinblade) { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK19_80C"); }
        else { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK19_80B"); }
    } else if (book == 21) {
        if (IsActiveThreadTerminating()) return;
        char buf[48];
        cat2(buf, "TEXT_CS_B16_BOOK21_20", teddy); args.set("$ARG1", buf);
        cat2(buf, "TEXT_CS_B16_BOOK21_30", teddy); args.set("$ARG2", buf);
        if (std::strcmp(teddy, "A") == 0) { if (IsActiveThreadTerminating()) return; args.set("$ARG3", "TEXT_CS_B16_BOOK21_40A"); }
        else { if (IsActiveThreadTerminating()) return; args.set("$ARG3", "TEXT_CS_B16_BOOK21_40B"); }
    } else if (book == 22) {
        if (IsActiveThreadTerminating()) return;
        if (scary > 0.5) { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK22_20B"); }
        else { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK22_20A"); }
    } else if (book == 23) {
        if (IsActiveThreadTerminating()) return;
        if (attract < -0.5) { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK23_10A"); }
        else if (attract > 0.5) { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK23_10C"); }
        else { if (IsActiveThreadTerminating()) return; args.set("$ARG1", "TEXT_CS_B16_BOOK23_10B"); }
    }
}
static int check(Teacher& t, long book, const char* k, const char* v) { Args a; a.n = 0; t.GetBookSpecificArgs(book, a); return std::strcmp(a.get(k), v) == 0; }
int main()
{
    int ok = 1;
    Teacher t; t.postSave = 2500; t.twinblade = true; t.teddy = "A"; t.attract = 0.7f; t.scary = 0.1f; t.terminating = false;
    ok &= check(t, 15, "$ARG1", "TEXT_CS_B16_BOOK15_90C");
    t.postSave = 3000; ok &= check(t, 15, "$ARG1", "TEXT_CS_B16_BOOK15_90B");
    t.postSave = 1000; ok &= check(t, 15, "$ARG1", "TEXT_CS_B16_BOOK15_90D");
    ok &= check(t, 17, "$ARG1", "TEXT_CS_B16_BOOK17_20B");
    t.postSave = 900; ok &= check(t, 19, "$ARG1", "TEXT_CS_B16_BOOK19_80C");
    ok &= check(t, 21, "$ARG2", "TEXT_CS_B16_BOOK21_30A");
    ok &= check(t, 21, "$ARG3", "TEXT_CS_B16_BOOK21_40A");
    t.teddy = "B"; ok &= check(t, 21, "$ARG3", "TEXT_CS_B16_BOOK21_40B");
    ok &= check(t, 22, "$ARG1", "TEXT_CS_B16_BOOK22_20A");
    ok &= check(t, 23, "$ARG1", "TEXT_CS_B16_BOOK23_10C");
    t.attract = -0.8f; ok &= check(t, 23, "$ARG1", "TEXT_CS_B16_BOOK23_10A");
    t.terminating = true; ok &= check(t, 23, "$ARG1", "");
    if (!ok) { std::printf("00e57020_TEST FAIL\n"); return 1; }
    std::printf("00e57020_TEST PASS\n");
    return 0;
}