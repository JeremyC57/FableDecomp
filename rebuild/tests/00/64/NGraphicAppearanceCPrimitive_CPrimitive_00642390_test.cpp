// Behaviour test for NGraphicAppearance::CPrimitive::CPrimitive(EPrimitiveType, long) (0x00642390): type and bank
// index stored, the primitive handle constructed empty.
#include <cstdio>
static int g_baseCalls = 0;
class CBaseClass { public: CBaseClass() { ++g_baseCalls; } virtual ~CBaseClass() { } };
class CEnginePrimitiveHandle : public CBaseClass { public: void* Data; CEnginePrimitiveHandle() : Data(0) { } };
struct CPrimitive {
    enum EPrimitiveType { TYPE_0, TYPE_1, TYPE_2 };
    EPrimitiveType Type; CEnginePrimitiveHandle PrimitiveHandle; long BankIndex;
    CPrimitive(EPrimitiveType type, long bankIndex) : Type(type), BankIndex(bankIndex) { }
};
int main()
{
    CPrimitive p(CPrimitive::TYPE_2, 17);
    bool ok = p.Type == CPrimitive::TYPE_2 && p.BankIndex == 17 && p.PrimitiveHandle.Data == 0 && g_baseCalls == 1;
    if (!ok) { std::printf("00642390_TEST FAIL\n"); return 1; }
    std::printf("00642390_TEST PASS\n");
    return 0;
}