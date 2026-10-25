#include <cstdio>

class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value);
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
protected:
    template <class T> static T GetDefaultVal() { return T(); }
};

static const CPersistContext* g_self;
static const char* g_name;
static bool* g_value;
static int g_default = -1;

template <class T> void CPersistContext::Transfer(const char* name, T& value, const T& defaultValue)
{
    g_self = this;
    g_name = name;
    g_value = &value;
    g_default = defaultValue ? 1 : 0;
}

template <class T> void CPersistContext::Transfer(const char* name, T& value)
{
    Transfer(name, value, GetDefaultVal<T>());
}

int main()
{
    CPersistContext ctx;
    bool v = true;
    ctx.Transfer("Flag", v);
    if (g_self == &ctx && g_name != 0 && g_name[0] == 'F' && g_value == &v && g_default == 0 && v) {
        std::printf("004107c0_TEST PASS\n");
        return 0;
    }
    std::printf("004107c0_TEST FAIL default=%d\n", g_default);
    return 1;
}
