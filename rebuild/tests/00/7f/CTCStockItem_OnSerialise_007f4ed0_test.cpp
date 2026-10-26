#include <cstdio>

#include <cstring>
class CPersistContext { public:
    bool saving; int count; char names[3][16]; long values[3];
    template<class T> void Transfer(const char* name, T& v, T)
    {
        std::strcpy(names[count], name);
        if (saving) values[count] = (long)v; else v = (T)values[count];
        ++count;
    }
};
class CTCStockItem { public:
    bool ForSale; bool Stealable; bool Initialised; long Price;
    void OnSerialise(CPersistContext& context, bool)
    {
        context.Transfer("ForSale", ForSale, false);
        context.Transfer("Stealable", Stealable, false);
        context.Transfer("Price", Price, 0L);
        Initialised = true;
    }
};
static bool Run()
{
    CTCStockItem s; s.ForSale = true; s.Stealable = false; s.Initialised = false; s.Price = 40;
    CPersistContext c; c.saving = true; c.count = 0;
    s.OnSerialise(c, true);
    bool ok = c.count == 3 && std::strcmp(c.names[0], "ForSale") == 0 && std::strcmp(c.names[1], "Stealable") == 0 &&
              std::strcmp(c.names[2], "Price") == 0 && c.values[0] == 1 && c.values[1] == 0 && c.values[2] == 40 && s.Initialised;
    CTCStockItem l; l.ForSale = false; l.Stealable = false; l.Initialised = false; l.Price = 0;
    c.saving = false; c.count = 0; c.values[1] = 1; l.OnSerialise(c, false);
    return ok && l.ForSale && l.Stealable && l.Price == 40 && l.Initialised;
}

int main()
{
    if (!Run()) { std::printf("007f4ed0_TEST FAIL\n"); return 1; }
    std::printf("007f4ed0_TEST PASS\n");
    return 0;
}