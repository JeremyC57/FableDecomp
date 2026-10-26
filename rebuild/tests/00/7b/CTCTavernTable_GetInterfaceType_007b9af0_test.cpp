// Test for 0x007B9AF0 CTCTavernTable::GetInterfaceType: the virtual returns the class's interface type 0xB8.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTavernTable {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTavernTable::GetInterfaceType() const
{
    return (ETCInterfaceType)0xB8;
}
int main()
{
    CTCTavernTable object;
    const CTCTavernTable* p = &object;
    if ((long)p->GetInterfaceType() != 0xB8) { printf("007b9af0_TEST FAIL\n"); return 1; }
    printf("007b9af0_TEST PASS\n");
    return 0;
}
