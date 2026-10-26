// Test for 0x007BB600 CTCTrespasser::GetInterfaceType: the virtual returns the class's interface type 0xBE.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTrespasser {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTrespasser::GetInterfaceType() const
{
    return (ETCInterfaceType)0xBE;
}
int main()
{
    CTCTrespasser object;
    const CTCTrespasser* p = &object;
    if ((long)p->GetInterfaceType() != 0xBE) { printf("007bb600_TEST FAIL\n"); return 1; }
    printf("007bb600_TEST PASS\n");
    return 0;
}
