// Test for 0x004D3179 CTCMeshAutomaticEntityCreator::GetInterfaceType: the virtual returns the class's interface type 0x20.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCMeshAutomaticEntityCreator {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCMeshAutomaticEntityCreator::GetInterfaceType() const
{
    return (ETCInterfaceType)0x20;
}
int main()
{
    CTCMeshAutomaticEntityCreator object;
    const CTCMeshAutomaticEntityCreator* p = &object;
    if ((long)p->GetInterfaceType() != 0x20) { printf("004d3179_TEST FAIL\n"); return 1; }
    printf("004d3179_TEST PASS\n");
    return 0;
}
