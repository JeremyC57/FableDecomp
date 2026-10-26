// Test for 0x004E74C5 CTCCarrying::GetInterfaceType: the virtual returns the class's interface type 0x46.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCarrying {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCarrying::GetInterfaceType() const
{
    return (ETCInterfaceType)0x46;
}
int main()
{
    CTCCarrying object;
    const CTCCarrying* p = &object;
    if ((long)p->GetInterfaceType() != 0x46) { printf("004e74c5_TEST FAIL\n"); return 1; }
    printf("004e74c5_TEST PASS\n");
    return 0;
}
