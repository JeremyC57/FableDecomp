// Test for 0x007B8050 CTCOverheadDisplay::GetInterfaceType: the virtual returns the class's interface type 0xB7.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCOverheadDisplay {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCOverheadDisplay::GetInterfaceType() const
{
    return (ETCInterfaceType)0xB7;
}
int main()
{
    CTCOverheadDisplay object;
    const CTCOverheadDisplay* p = &object;
    if ((long)p->GetInterfaceType() != 0xB7) { printf("007b8050_TEST FAIL\n"); return 1; }
    printf("007b8050_TEST PASS\n");
    return 0;
}
