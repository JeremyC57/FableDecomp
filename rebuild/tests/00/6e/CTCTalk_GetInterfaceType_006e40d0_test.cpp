// Test for 0x006E40D0 CTCTalk::GetInterfaceType: the virtual returns the class's interface type 0xF.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTalk {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTalk::GetInterfaceType() const
{
    return (ETCInterfaceType)0xF;
}
int main()
{
    CTCTalk object;
    const CTCTalk* p = &object;
    if ((long)p->GetInterfaceType() != 0xF) { printf("006e40d0_TEST FAIL\n"); return 1; }
    printf("006e40d0_TEST PASS\n");
    return 0;
}
