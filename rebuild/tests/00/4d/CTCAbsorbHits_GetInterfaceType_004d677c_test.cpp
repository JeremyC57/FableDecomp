// Test for 0x004D677C CTCAbsorbHits::GetInterfaceType: the virtual returns the class's interface type 0x110.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCAbsorbHits {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCAbsorbHits::GetInterfaceType() const
{
    return (ETCInterfaceType)0x110;
}
int main()
{
    CTCAbsorbHits object;
    const CTCAbsorbHits* p = &object;
    if ((long)p->GetInterfaceType() != 0x110) { printf("004d677c_TEST FAIL\n"); return 1; }
    printf("004d677c_TEST PASS\n");
    return 0;
}
