// Test for 0x004E6D71 CTCLightningNode::GetInterfaceType: the virtual returns the class's interface type 0x10B.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCLightningNode {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCLightningNode::GetInterfaceType() const
{
    return (ETCInterfaceType)0x10B;
}
int main()
{
    CTCLightningNode object;
    const CTCLightningNode* p = &object;
    if ((long)p->GetInterfaceType() != 0x10B) { printf("004e6d71_TEST FAIL\n"); return 1; }
    printf("004e6d71_TEST PASS\n");
    return 0;
}
