// Behaviour test for 004ab7f0 CPersistContext::Transfer<ESaveType>(name, value): forwards to the three-argument
// Transfer with a value-initialised default (0).
#include <cstdio>
#include <cstring>
enum ESaveType { SAVE_TYPE_DEFAULT = 0, SAVE_TYPE_AUTO = 3 };
class CPersistContext { public:
    const char* seenName; ESaveType* seenValue; long seenDefault;
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue) { seenName = name; seenValue = &value; seenDefault = (long)defaultValue; }
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, T()); }
};
int main()
{
    CPersistContext c; c.seenName = 0; c.seenValue = 0; c.seenDefault = -1;
    ESaveType v = SAVE_TYPE_AUTO;
    c.Transfer("MostRecentSaveType", v);
    if (c.seenName == 0 || std::strcmp(c.seenName, "MostRecentSaveType") != 0 || c.seenValue != &v || c.seenDefault != 0 || v != SAVE_TYPE_AUTO) { std::printf("004ab7f0_TEST FAIL\n"); return 1; }
    std::printf("004ab7f0_TEST PASS\n");
    return 0;
}