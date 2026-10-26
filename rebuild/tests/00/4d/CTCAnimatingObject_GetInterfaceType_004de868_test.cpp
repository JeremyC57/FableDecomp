// Test for 0x004DE868 CTCAnimatingObject::GetInterfaceType: the virtual returns the class's interface type 0x83.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCAnimatingObject {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCAnimatingObject::GetInterfaceType() const
{
    return (ETCInterfaceType)0x83;
}
int main()
{
    CTCAnimatingObject object;
    const CTCAnimatingObject* p = &object;
    if ((long)p->GetInterfaceType() != 0x83) { printf("004de868_TEST FAIL\n"); return 1; }
    printf("004de868_TEST PASS\n");
    return 0;
}
