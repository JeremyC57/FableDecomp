// Test for 0x004D5F1C CTCAmbientBird::GetInterfaceType: the virtual returns the class's interface type 0xE7.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCAmbientBird {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCAmbientBird::GetInterfaceType() const
{
    return (ETCInterfaceType)0xE7;
}
int main()
{
    CTCAmbientBird object;
    const CTCAmbientBird* p = &object;
    if ((long)p->GetInterfaceType() != 0xE7) { printf("004d5f1c_TEST FAIL\n"); return 1; }
    printf("004d5f1c_TEST PASS\n");
    return 0;
}
