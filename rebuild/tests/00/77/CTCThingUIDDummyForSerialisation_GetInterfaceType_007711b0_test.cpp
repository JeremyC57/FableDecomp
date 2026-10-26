// Test for 0x007711B0 CTCThingUIDDummyForSerialisation::GetInterfaceType: the virtual returns the class's interface type 0xEB.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCThingUIDDummyForSerialisation {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCThingUIDDummyForSerialisation::GetInterfaceType() const
{
    return (ETCInterfaceType)0xEB;
}
int main()
{
    CTCThingUIDDummyForSerialisation object;
    const CTCThingUIDDummyForSerialisation* p = &object;
    if ((long)p->GetInterfaceType() != 0xEB) { printf("007711b0_TEST FAIL\n"); return 1; }
    printf("007711b0_TEST PASS\n");
    return 0;
}
