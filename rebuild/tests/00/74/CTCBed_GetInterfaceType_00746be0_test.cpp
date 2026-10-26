// Test for 0x00746BE0 CTCBed::GetInterfaceType: the virtual returns the class's interface type 0x35.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBed {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBed::GetInterfaceType() const
{
    return (ETCInterfaceType)0x35;
}
int main()
{
    CTCBed object;
    const CTCBed* p = &object;
    if ((long)p->GetInterfaceType() != 0x35) { printf("00746be0_TEST FAIL\n"); return 1; }
    printf("00746be0_TEST PASS\n");
    return 0;
}
