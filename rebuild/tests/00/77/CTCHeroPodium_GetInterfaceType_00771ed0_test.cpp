// Test for 0x00771ED0 CTCHeroPodium::GetInterfaceType: the virtual returns the class's interface type 0x52.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroPodium {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroPodium::GetInterfaceType() const
{
    return (ETCInterfaceType)0x52;
}
int main()
{
    CTCHeroPodium object;
    const CTCHeroPodium* p = &object;
    if ((long)p->GetInterfaceType() != 0x52) { printf("00771ed0_TEST FAIL\n"); return 1; }
    printf("00771ed0_TEST PASS\n");
    return 0;
}
