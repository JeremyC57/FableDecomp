// Test for 0x004DB401 CTCDiceGame::GetInterfaceType: the virtual returns the class's interface type 0xF0.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDiceGame {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDiceGame::GetInterfaceType() const
{
    return (ETCInterfaceType)0xF0;
}
int main()
{
    CTCDiceGame object;
    const CTCDiceGame* p = &object;
    if ((long)p->GetInterfaceType() != 0xF0) { printf("004db401_TEST FAIL\n"); return 1; }
    printf("004db401_TEST PASS\n");
    return 0;
}
