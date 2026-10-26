// Test for 0x00787A00 CTCBob::GetInterfaceType: the virtual returns the class's interface type 0xF2.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBob {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBob::GetInterfaceType() const
{
    return (ETCInterfaceType)0xF2;
}
int main()
{
    CTCBob object;
    const CTCBob* p = &object;
    if ((long)p->GetInterfaceType() != 0xF2) { printf("00787a00_TEST FAIL\n"); return 1; }
    printf("00787a00_TEST PASS\n");
    return 0;
}
