// Test for 0x004D34C3 CTCWeatherVane::GetInterfaceType: the virtual returns the class's interface type 0x2C.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWeatherVane {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWeatherVane::GetInterfaceType() const
{
    return (ETCInterfaceType)0x2C;
}
int main()
{
    CTCWeatherVane object;
    const CTCWeatherVane* p = &object;
    if ((long)p->GetInterfaceType() != 0x2C) { printf("004d34c3_TEST FAIL\n"); return 1; }
    printf("004d34c3_TEST PASS\n");
    return 0;
}
