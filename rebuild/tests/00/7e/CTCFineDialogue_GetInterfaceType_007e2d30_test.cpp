// Test for 0x007E2D30 CTCFineDialogue::GetInterfaceType: the virtual returns the class's interface type 0xF3.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFineDialogue {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFineDialogue::GetInterfaceType() const
{
    return (ETCInterfaceType)0xF3;
}
int main()
{
    CTCFineDialogue object;
    const CTCFineDialogue* p = &object;
    if ((long)p->GetInterfaceType() != 0xF3) { printf("007e2d30_TEST FAIL\n"); return 1; }
    printf("007e2d30_TEST PASS\n");
    return 0;
}
