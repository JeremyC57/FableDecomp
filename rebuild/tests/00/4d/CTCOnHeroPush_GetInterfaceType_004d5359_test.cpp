// Test for 0x004D5359 CTCOnHeroPush::GetInterfaceType: the virtual returns the class's interface type 0xC4.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCOnHeroPush {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCOnHeroPush::GetInterfaceType() const
{
    return (ETCInterfaceType)0xC4;
}
int main()
{
    CTCOnHeroPush object;
    const CTCOnHeroPush* p = &object;
    if ((long)p->GetInterfaceType() != 0xC4) { printf("004d5359_TEST FAIL\n"); return 1; }
    printf("004d5359_TEST PASS\n");
    return 0;
}
