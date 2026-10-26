// Test for 0x00744270 CTCCreatureModeManager::GetInterfaceType: the virtual returns the class's interface type 0x31.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureModeManager {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureModeManager::GetInterfaceType() const
{
    return (ETCInterfaceType)0x31;
}
int main()
{
    CTCCreatureModeManager object;
    const CTCCreatureModeManager* p = &object;
    if ((long)p->GetInterfaceType() != 0x31) { printf("00744270_TEST FAIL\n"); return 1; }
    printf("00744270_TEST PASS\n");
    return 0;
}
