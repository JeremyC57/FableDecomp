// Test for 0x004E7545 CTCBoastingPodium::GetInterfaceType: the virtual returns the class's interface type 0x70.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBoastingPodium {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBoastingPodium::GetInterfaceType() const
{
    return (ETCInterfaceType)0x70;
}
int main()
{
    CTCBoastingPodium object;
    const CTCBoastingPodium* p = &object;
    if ((long)p->GetInterfaceType() != 0x70) { printf("004e7545_TEST FAIL\n"); return 1; }
    printf("004e7545_TEST PASS\n");
    return 0;
}
