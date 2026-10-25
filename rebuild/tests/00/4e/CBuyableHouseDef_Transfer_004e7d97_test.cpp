// Behaviour test for CBuyableHouseDef::Transfer (0x004E7D97): every member is handed to the typed transfer once,
// in the TRANSFER list order (IsScripted ahead of MaxRentBags).
#include <cstdio>

template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

class CPersistContext {
public:
    const void* seen[16]; char kind[16]; int count;
    CPersistContext() : count(0) {}
    void Transfer(bool& value)             { seen[count] = &value; kind[count++] = 'b'; }
    void Transfer(long& value)             { seen[count] = &value; kind[count++] = 'l'; }
    void Transfer(CStlVector<long>& value) { seen[count] = &value; kind[count++] = 'v'; }
};

class CBuyableHouseDef {
public:
    virtual void Transfer(CPersistContext& context);
    virtual ~CBuyableHouseDef() {}
    unsigned char _base[0x21];
    bool IsBuyable;
    bool InitiallyEmpty;
    CStlVector<long> Price;
    CStlVector<long> Rent;
    long DaysBetweenRentPayments;
    long MaxRentBags;
    bool IsScripted;
    bool IsBuyableWithoutSignpost;
};

void CBuyableHouseDef::Transfer(CPersistContext& context)
{
    context.Transfer(IsBuyable);
    context.Transfer(InitiallyEmpty);
    context.Transfer(Price);
    context.Transfer(Rent);
    context.Transfer(DaysBetweenRentPayments);
    context.Transfer(IsScripted);
    context.Transfer(MaxRentBags);
    context.Transfer(IsBuyableWithoutSignpost);
}

int main()
{
    CBuyableHouseDef def;
    CPersistContext ctx;
    def.Transfer(ctx);
    const void* want[8] = { &def.IsBuyable, &def.InitiallyEmpty, &def.Price, &def.Rent, &def.DaysBetweenRentPayments,
                            &def.IsScripted, &def.MaxRentBags, &def.IsBuyableWithoutSignpost };
    const char kinds[9] = "bbvvlblb";
    if (ctx.count != 8) { std::printf("004e7d97_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i)
        if (ctx.seen[i] != want[i] || ctx.kind[i] != kinds[i]) { std::printf("004e7d97_TEST FAIL\n"); return 1; }
    std::printf("004e7d97_TEST PASS\n");
    return 0;
}