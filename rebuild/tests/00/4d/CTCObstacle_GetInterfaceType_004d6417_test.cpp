// Test for 0x004D6417 CTCObstacle::GetInterfaceType: the virtual returns the class's interface type 0xFE.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCObstacle {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCObstacle::GetInterfaceType() const
{
    return (ETCInterfaceType)0xFE;
}
int main()
{
    CTCObstacle object;
    const CTCObstacle* p = &object;
    if ((long)p->GetInterfaceType() != 0xFE) { printf("004d6417_TEST FAIL\n"); return 1; }
    printf("004d6417_TEST PASS\n");
    return 0;
}
