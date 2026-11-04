// Behaviour test for CTCShopKeeper::GetCustomer 0x00798D80: the shopkeeper's customer is its intelligent pointer's item. The body forwards to the member at +0xC
// (`add ecx,Ch ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
struct CThing { int id; };
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } void SetPItem(CThing* t) { Item = t; } };
#pragma pack(push, 4)
struct CTCShopKeeper {                                  // retail is 32-bit: 4-byte packing keeps the +0xC offset on a 64-bit host
    unsigned char Before[0xC];
    CIntelligentPointer Customer;
    CThing* GetCustomer() const { return Customer.GetPItem(); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CTCShopKeeper, Customer) != 0xCu) { std::printf("00798d80_TEST FAIL\n"); return 1; }
    CThing t = { 5 };
    CTCShopKeeper o; o.Customer.Item = &t;
    if (o.GetCustomer() != &t) { std::printf("00798d80_TEST FAIL\n"); return 1; }
    o.Customer.Item = 0;
    if (o.GetCustomer() != 0) { std::printf("00798d80_TEST FAIL\n"); return 1; }
    std::printf("00798d80_TEST PASS\n");
    return 0;
}