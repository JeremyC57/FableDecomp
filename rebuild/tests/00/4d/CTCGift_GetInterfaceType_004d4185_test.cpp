// Test for 0x004D4185 CTCGift::GetInterfaceType: the virtual returns the class's interface type 0x59.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCGift {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCGift::GetInterfaceType() const
{
    return (ETCInterfaceType)0x59;
}
int main()
{
    CTCGift object;
    const CTCGift* p = &object;
    if ((long)p->GetInterfaceType() != 0x59) { printf("004d4185_TEST FAIL\n"); return 1; }
    printf("004d4185_TEST PASS\n");
    return 0;
}
