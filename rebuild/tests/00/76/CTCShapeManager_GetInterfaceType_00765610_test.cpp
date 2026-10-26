// Test for 0x00765610 CTCShapeManager::GetInterfaceType: the virtual returns the class's interface type 0x44.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCShapeManager {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCShapeManager::GetInterfaceType() const
{
    return (ETCInterfaceType)0x44;
}
int main()
{
    CTCShapeManager object;
    const CTCShapeManager* p = &object;
    if ((long)p->GetInterfaceType() != 0x44) { printf("00765610_TEST FAIL\n"); return 1; }
    printf("00765610_TEST PASS\n");
    return 0;
}
