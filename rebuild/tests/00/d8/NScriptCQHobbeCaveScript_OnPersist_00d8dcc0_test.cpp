// Behaviour test for 00d8dcc0 CQ_HobbeCaveScript::OnPersist: two Transfer<bool> calls, keys and member order as retail, default false each time.
#include <cstdio>
#include <cstring>
class CPersistContext { public:
    int calls; const char* keys[4]; bool* vals[4]; bool defs[4]; bool loading;
    CPersistContext() : calls(0), loading(false) { }
    template <class T> void Transfer(const char* name, T& value, const T& def)
    {
        keys[calls] = name; vals[calls] = &value; defs[calls] = def; ++calls;
        if (loading) value = !def;          // a load of a stored "true"
    }
};
struct Script {
    unsigned char raw[0x80];
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("QuestStartScreened", *(bool*)(raw + 0x69), false);
        context.Transfer("SavedBeforeCaves", *(bool*)(raw + 0x49), false);
    }
};
int main()
{
    Script s; std::memset(s.raw, 0, sizeof s.raw);
    CPersistContext save; s.OnPersist(save);
    if (save.calls != 2 || std::strcmp(save.keys[0], "QuestStartScreened") || std::strcmp(save.keys[1], "SavedBeforeCaves")
        || save.vals[0] != (bool*)(s.raw + 0x69) || save.vals[1] != (bool*)(s.raw + 0x49) || save.defs[0] || save.defs[1])
    { std::printf("00d8dcc0_TEST FAIL\n"); return 1; }
    CPersistContext load; load.loading = true; s.OnPersist(load);
    if (!s.raw[0x69] || !s.raw[0x49]) { std::printf("00d8dcc0_TEST FAIL\n"); return 1; }
    std::printf("00d8dcc0_TEST PASS\n");
    return 0;
}