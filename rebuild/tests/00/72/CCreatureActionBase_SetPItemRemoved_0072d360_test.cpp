// Behaviour test for 0072d360 CCreatureActionBase::SetPItemRemoved: PItemRemoved takes the by-value pointer's target, and the argument is destroyed once.
#include <cstdio>
static int g_destroyed = 0;
class CThing { public: int id; };
class CIntelligentPointer
{
public:
    CThing* PData;
    CThing* GetPItem() const { return PData; }
    void SetPItem(CThing* item) { PData = item; }
    ~CIntelligentPointer() { ++g_destroyed; }
};
class CCreatureActionBase
{
public:
    unsigned char _pad[0x2c];
    CIntelligentPointer PItemRemoved;
    void SetPItemRemoved(CIntelligentPointer item) { PItemRemoved.SetPItem(item.GetPItem()); }
};
int main()
{
    CThing thing; thing.id = 7;
    CCreatureActionBase action; action.PItemRemoved.PData = 0;
    {
        CIntelligentPointer item; item.PData = &thing;
        action.SetPItemRemoved(item);
        if (action.PItemRemoved.GetPItem() != &thing || g_destroyed != 1) { std::printf("0072d360_TEST FAIL\n"); return 1; }
    }
    std::printf("0072d360_TEST PASS\n");
    return 0;
}
