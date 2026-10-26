// Behaviour test for 0x008ED2B0 CTCContainer::OnSerialise: names out when saving, indices back when loading.
#include <cstdio>

struct Item { long first; int second; };

class CPersistContext { public:
    long Mode;
    long Saved[8];
    int  SavedCount;
    int  Transfers;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void TransferVector(long* names, int& count)
    {
        ++Transfers;
        if (IsSaving()) { for (int i = 0; i < count; ++i) Saved[i] = names[i]; SavedCount = count; }
        else { for (int i = 0; i < SavedCount; ++i) names[i] = Saved[i]; count = SavedCount; }
    }
};

class CTCContainer { public:
    Item items[8];
    int  count;
    void OnSerialise(CPersistContext& context, bool)
    {
        long names[8];
        int nameCount = 0;
        if (context.IsSaving()) {
            for (int i = 0; i < count; ++i) names[nameCount++] = 1000 + items[i].first;   // index -> name
            context.TransferVector(names, nameCount);
        } else {
            context.TransferVector(names, nameCount);
            for (int i = 0; i < nameCount; ++i) { items[count].first = names[i] - 1000; items[count].second = 0; ++count; }
        }
    }
};

int main()
{
    CTCContainer a; a.count = 2; a.items[0].first = 5; a.items[1].first = 9;
    CPersistContext ctx; ctx.Mode = 3; ctx.SavedCount = 0; ctx.Transfers = 0;
    int fail = 0;
    a.OnSerialise(ctx, true);
    if (ctx.SavedCount != 2 || ctx.Saved[0] != 1005 || ctx.Saved[1] != 1009 || ctx.Transfers != 1) fail = 1;
    CTCContainer b; b.count = 0;
    ctx.Mode = 2;                                   // loading
    b.OnSerialise(ctx, false);
    if (b.count != 2 || b.items[0].first != 5 || b.items[1].first != 9 || ctx.Transfers != 2) fail = 1;
    ctx.Mode = 1;                                   // the other saving mode
    a.OnSerialise(ctx, false);
    if (ctx.Transfers != 3 || ctx.SavedCount != 2) fail = 1;
    if (fail) { std::printf("008ed2b0_TEST FAIL\n"); return 1; }
    std::printf("008ed2b0_TEST PASS\n");
    return 0;
}