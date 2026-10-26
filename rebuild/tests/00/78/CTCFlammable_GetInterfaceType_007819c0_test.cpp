// Test for 0x007819C0 CTCFlammable::GetInterfaceType: the virtual returns the class's interface type 0x6D.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFlammable {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFlammable::GetInterfaceType() const
{
    return (ETCInterfaceType)0x6D;
}
int main()
{
    CTCFlammable object;
    const CTCFlammable* p = &object;
    if ((long)p->GetInterfaceType() != 0x6D) { printf("007819c0_TEST FAIL\n"); return 1; }
    printf("007819c0_TEST PASS\n");
    return 0;
}
