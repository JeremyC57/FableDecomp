#include <cstdio>
#include <cstring>
struct CPersistContext {
    bool loading; int calls; const char* lastName;
    void Transfer(const char* name, long& value, const long& def) { ++calls; lastName = name; if (loading) value = def; }
    void Transfer(const char* name, bool& value, const bool& def) { ++calls; lastName = name; if (loading) value = def; }
};
struct CTCTrophy {
    long BestWitnessesAheadToDate; bool Mountable;
    virtual ~CTCTrophy() {}
    virtual void OnSerialise(CPersistContext& context, bool)
    {
        long defaultBest = 0;
        context.Transfer("BestWitnessesAheadToDate", BestWitnessesAheadToDate, defaultBest);
        context.Transfer("Mountable", Mountable, false);
    }
};
int main()
{
    CTCTrophy t; t.BestWitnessesAheadToDate = 9; t.Mountable = true;
    CPersistContext save; save.loading = false; save.calls = 0;
    t.OnSerialise(save, false);
    if (save.calls != 2 || std::strcmp(save.lastName, "Mountable") != 0 || t.BestWitnessesAheadToDate != 9 || !t.Mountable) { std::printf("006ddbf0_TEST FAIL\n"); return 1; }
    CPersistContext load; load.loading = true; load.calls = 0;
    t.OnSerialise(load, true);
    if (load.calls != 2 || t.BestWitnessesAheadToDate != 0 || t.Mountable) { std::printf("006ddbf0_TEST FAIL\n"); return 1; }
    std::printf("006ddbf0_TEST PASS\n");
    return 0;
}