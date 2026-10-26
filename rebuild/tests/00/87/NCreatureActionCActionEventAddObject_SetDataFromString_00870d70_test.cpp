// Behaviour test for CActionEventAddObject::SetDataFromString (0x00870D70).
#include <cstdio>
#include <cstring>

class CCharString { public:
    char text[32];
    CCharString() { text[0] = 0; }
    ~CCharString() {}
};

// field `index` of a comma-separated list
static bool GetDataFromEventString(const CCharString& data, long index, CCharString* out)
{
    const char* p = data.text;
    for (long i = 0; i < index; ++i) { p = std::strchr(p, ','); if (!p) return false; ++p; }
    const char* e = std::strchr(p, ',');
    size_t n = e ? (size_t)(e - p) : std::strlen(p);
    if (n == 0) return false;
    std::memcpy(out->text, p, n); out->text[n] = 0;
    return true;
}

struct CActionEventAddObject {
    CCharString Object; CCharString Hand; bool DestroyOnExit;
    bool SetDataFromString(const CCharString& data)
    {
        if (!GetDataFromEventString(data, 0, &Object))
            return false;
        if (!GetDataFromEventString(data, 1, &Hand))
            return false;
        CCharString destroyOnExit;
        if (GetDataFromEventString(data, 2, &destroyOnExit))
            DestroyOnExit = true;
        return true;
    }
};

static int fail() { std::printf("00870d70_TEST FAIL\n"); return 1; }

int main()
{
    CCharString s; std::strcpy(s.text, "SWORD,RIGHT");
    CActionEventAddObject a; a.DestroyOnExit = false;
    if (!a.SetDataFromString(s) || std::strcmp(a.Object.text, "SWORD") || std::strcmp(a.Hand.text, "RIGHT") || a.DestroyOnExit)
        return fail();
    std::strcpy(s.text, "AXE,LEFT,YES");
    if (!a.SetDataFromString(s) || !a.DestroyOnExit || std::strcmp(a.Object.text, "AXE")) return fail();
    std::strcpy(s.text, "ONLY");
    if (a.SetDataFromString(s)) return fail();
    std::printf("00870d70_TEST PASS\n");
    return 0;
}