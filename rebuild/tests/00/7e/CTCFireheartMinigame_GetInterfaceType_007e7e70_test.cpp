// Test for 0x007E7E70 CTCFireheartMinigame::GetInterfaceType: the virtual returns the class's interface type 0x108.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFireheartMinigame {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFireheartMinigame::GetInterfaceType() const
{
    return (ETCInterfaceType)0x108;
}
int main()
{
    CTCFireheartMinigame object;
    const CTCFireheartMinigame* p = &object;
    if ((long)p->GetInterfaceType() != 0x108) { printf("007e7e70_TEST FAIL\n"); return 1; }
    printf("007e7e70_TEST PASS\n");
    return 0;
}
