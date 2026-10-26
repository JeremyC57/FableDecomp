// Test for 0x007E6FD0 CTCOracleMinigame::GetInterfaceType: the virtual returns the class's interface type 0x107.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCOracleMinigame {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCOracleMinigame::GetInterfaceType() const
{
    return (ETCInterfaceType)0x107;
}
int main()
{
    CTCOracleMinigame object;
    const CTCOracleMinigame* p = &object;
    if ((long)p->GetInterfaceType() != 0x107) { printf("007e6fd0_TEST FAIL\n"); return 1; }
    printf("007e6fd0_TEST PASS\n");
    return 0;
}
