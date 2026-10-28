// Behaviour test for 0x005DB530 CAnimComponentFollowingOnAnimFilter::Initialise: SetDefaultValue first; a value longer than two characters has its first and last characters stripped (GetSubString(1, length - 1)) into the NextFilter def string and returns true; a shorter or empty value returns false
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static int g_defaults = 0, g_lastStart = -1, g_lastEnd = -1;
class CCharString { public: const char* p; int len;
    int GetLength() const { return p ? len : 0; }
    CCharString GetSubString(int s, int e) const { g_lastStart = s; g_lastEnd = e; CCharString r; r.p = p + s; r.len = e - s; return r; } };
class CDefString { public: int TablePos; CDefString() : TablePos(0) { } CDefString(const CCharString& s) { TablePos = s.len * 100 + s.p[0]; } };
struct C {
    CDefString NextFilter;
    void SetDefaultValue() { ++g_defaults; }
    bool Initialise(const CCharString& value)
    {
        SetDefaultValue();
        int length = value.GetLength();
        if (length > 2) { NextFilter = CDefString(value.GetSubString(1, length - 1)); return true; }
        return false;
    }
};
int main()
{
    C c; c.NextFilter.TablePos = -5;
    CCharString v; v.p = "\"Walk\""; v.len = 6;
    CHECK(c.Initialise(v) && g_defaults == 1 && g_lastStart == 1 && g_lastEnd == 5 && c.NextFilter.TablePos == 4 * 100 + 'W');
    CCharString s; s.p = "ab"; s.len = 2; c.NextFilter.TablePos = 11;
    CHECK(!c.Initialise(s) && g_defaults == 2 && c.NextFilter.TablePos == 11);
    CCharString n; n.p = 0; n.len = 9; CHECK(!c.Initialise(n) && g_defaults == 3);
    if (fails == 0) printf("005db530_TEST PASS\n"); else printf("005db530_TEST FAIL\n");
    return fails;
}