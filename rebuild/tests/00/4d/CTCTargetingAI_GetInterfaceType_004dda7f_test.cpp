// Test for 0x004DDA7F CTCTargetingAI::GetInterfaceType: the virtual returns the class's interface type 0x8.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTargetingAI {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTargetingAI::GetInterfaceType() const
{
    return (ETCInterfaceType)0x8;
}
int main()
{
    CTCTargetingAI object;
    const CTCTargetingAI* p = &object;
    if ((long)p->GetInterfaceType() != 0x8) { printf("004dda7f_TEST FAIL\n"); return 1; }
    printf("004dda7f_TEST PASS\n");
    return 0;
}
