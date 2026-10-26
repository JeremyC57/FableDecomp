// Test for 0x004D62B8 CTCFlashOnHit::GetInterfaceType: the virtual returns the class's interface type 0xFB.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFlashOnHit {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFlashOnHit::GetInterfaceType() const
{
    return (ETCInterfaceType)0xFB;
}
int main()
{
    CTCFlashOnHit object;
    const CTCFlashOnHit* p = &object;
    if ((long)p->GetInterfaceType() != 0xFB) { printf("004d62b8_TEST FAIL\n"); return 1; }
    printf("004d62b8_TEST PASS\n");
    return 0;
}
