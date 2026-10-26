// Test for 0x004D4278 CTCDInternalMarker::GetInterfaceType: the virtual returns the class's interface type 0xA2.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDInternalMarker {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDInternalMarker::GetInterfaceType() const
{
    return (ETCInterfaceType)0xA2;
}
int main()
{
    CTCDInternalMarker object;
    const CTCDInternalMarker* p = &object;
    if ((long)p->GetInterfaceType() != 0xA2) { printf("004d4278_TEST FAIL\n"); return 1; }
    printf("004d4278_TEST PASS\n");
    return 0;
}
