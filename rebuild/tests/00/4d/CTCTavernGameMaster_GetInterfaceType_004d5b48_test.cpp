// Test for 0x004D5B48 CTCTavernGameMaster::GetInterfaceType: the virtual returns the class's interface type 0xD2.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTavernGameMaster {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTavernGameMaster::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD2;
}
int main()
{
    CTCTavernGameMaster object;
    const CTCTavernGameMaster* p = &object;
    if ((long)p->GetInterfaceType() != 0xD2) { printf("004d5b48_TEST FAIL\n"); return 1; }
    printf("004d5b48_TEST PASS\n");
    return 0;
}
