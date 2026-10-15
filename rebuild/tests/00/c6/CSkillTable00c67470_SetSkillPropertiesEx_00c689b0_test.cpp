#include <cstdio>
// Behaviour test for 00c689b0.

// CSkillTable::SetSkillPropertiesEx: reserved indices fail with -3 (error slot -1); otherwise the index property is stored
// and the GUID / value / flag setters run in turn until one fails.
struct GUID_ { unsigned int a, b, c, d; };
struct SSkillPropertiesEx { GUID_ Guid; long Value; long Property; long Flag; };
struct CSkillTable {
    long property[8]; long lastError; int calls; long failAt;
    long SetSkillIndexProperty(long index, long value)
    {
        if (index == 0 || index == 1) { lastError = -1; return -3; }
        if (value != property[index]) property[index] = value;
        return 0;
    }
    long Step(long n) { ++calls; return n == failAt ? -5 : (long)n; }
    long SetSkillGUID(long index, GUID_ guid) { (void)index; (void)guid; return Step(1); }
    long SetSkillValue(long index, long value) { (void)index; (void)value; return Step(2); }
    long SetSkillFlag(long index, long flag) { (void)index; (void)flag; return Step(3); }
    long SetSkillPropertiesEx(long index, const SSkillPropertiesEx& p)
    {
        long result = SetSkillIndexProperty(index, p.Property);
        if (result >= 0) {
            result = SetSkillGUID(index, p.Guid);
            if (result >= 0) {
                result = SetSkillValue(index, p.Value);
                if (result >= 0) result = SetSkillFlag(index, p.Flag);
            }
        }
        return result;
    }
};
int main()
{
    CSkillTable t; for (int i = 0; i < 8; ++i) t.property[i] = 0; t.lastError = 0; t.calls = 0; t.failAt = 0;
    SSkillPropertiesEx p; p.Guid.a = p.Guid.b = p.Guid.c = p.Guid.d = 0; p.Value = 1; p.Property = 9; p.Flag = 1;
    int ok = t.SetSkillPropertiesEx(1, p) == -3 && t.lastError == -1 && t.calls == 0;
    ok = ok && t.SetSkillPropertiesEx(3, p) == 3 && t.property[3] == 9 && t.calls == 3;
    t.failAt = 2; t.calls = 0;
    ok = ok && t.SetSkillPropertiesEx(4, p) == -5 && t.calls == 2;
    if (!ok) { std::printf("00c689b0_TEST FAIL\n"); return 1; }
    std::printf("00c689b0_TEST PASS\n");
    return 0;
}