// Test for 0x004E7FC2 CTCCreatureHitNotification::GetInterfaceType: the virtual returns the class's interface type 0xCD.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureHitNotification {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureHitNotification::GetInterfaceType() const
{
    return (ETCInterfaceType)0xCD;
}
int main()
{
    CTCCreatureHitNotification object;
    const CTCCreatureHitNotification* p = &object;
    if ((long)p->GetInterfaceType() != 0xCD) { printf("004e7fc2_TEST FAIL\n"); return 1; }
    printf("004e7fc2_TEST PASS\n");
    return 0;
}
