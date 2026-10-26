// Test for 0x004DF1A6 CTCDPhysicalObstruction::GetInterfaceType: the virtual returns the class's interface type 0xED.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDPhysicalObstruction {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDPhysicalObstruction::GetInterfaceType() const
{
    return (ETCInterfaceType)0xED;
}
int main()
{
    CTCDPhysicalObstruction object;
    const CTCDPhysicalObstruction* p = &object;
    if ((long)p->GetInterfaceType() != 0xED) { printf("004df1a6_TEST FAIL\n"); return 1; }
    printf("004df1a6_TEST PASS\n");
    return 0;
}
