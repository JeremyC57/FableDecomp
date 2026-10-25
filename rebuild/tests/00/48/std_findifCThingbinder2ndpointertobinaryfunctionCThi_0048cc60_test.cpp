// Behaviour test for 0x0048CC60 std::__find_if over CThing* with bind2nd(ptr_fun(IsThingScriptNamed), name).
#include <cstdio>
#include <cstring>

static int g_live = 0;      // live CCharString copies (every copy must be destroyed)

struct CCharString {
    const char* text;
    CCharString(const char* t) : text(t) { ++g_live; }
    CCharString(const CCharString& o) : text(o.text) { ++g_live; }
    ~CCharString() { --g_live; }
};

struct CThing { const char* scriptName; };

static bool IsThingScriptNamed(CThing* thing, CCharString name)
{
    return std::strcmp(thing->scriptName, name.text) == 0;
}

struct NameBinder {          // binder2nd<pointer_to_binary_function<CThing*, CCharString, bool> >
    bool (*op)(CThing*, CCharString);
    CCharString value;
    NameBinder(bool (*f)(CThing*, CCharString), const CCharString& v) : op(f), value(v) { }
    NameBinder(const NameBinder& o) : op(o.op), value(o.value) { }
    bool call(CThing* x, CCharString y) const { return op(x, y); }
    bool operator()(CThing* x) const { return call(x, value); }
};

static CThing** FindIf(CThing** first, CThing** last, NameBinder pred)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
    }
    switch (last - first) {
    case 3: if (pred(*first)) return first; ++first;
    case 2: if (pred(*first)) return first; ++first;
    case 1: if (pred(*first)) return first; ++first;
    case 0:
    default: return last;
    }
}

int main()
{
    CThing things[7] = { { "SiteA" }, { "SiteB" }, { "SiteC" }, { "SiteD" }, { "SiteE" }, { "SiteF" }, { "SiteG" } };
    CThing* ptrs[7];
    for (int i = 0; i < 7; ++i) ptrs[i] = &things[i];
    int ok = 1;
    {
        CCharString name("SiteF");
        NameBinder pred(&IsThingScriptNamed, name);
        if (FindIf(ptrs, ptrs + 7, pred) != ptrs + 5) ok = 0;
        if (FindIf(ptrs, ptrs + 5, pred) != ptrs + 5) ok = 0;
        for (int n = 0; n <= 5; ++n)
            if (FindIf(ptrs, ptrs + n, pred) != ptrs + n) ok = 0;
        CCharString first("SiteA");
        NameBinder pred2(&IsThingScriptNamed, first);
        if (FindIf(ptrs, ptrs + 7, pred2) != ptrs) ok = 0;
        CCharString last("SiteG");
        NameBinder pred3(&IsThingScriptNamed, last);
        if (FindIf(ptrs, ptrs + 7, pred3) != ptrs + 6) ok = 0;
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("0048cc60_TEST FAIL\n"); return 1; }
    std::printf("0048cc60_TEST PASS\n");
    return 0;
}