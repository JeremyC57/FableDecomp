// Test for 0x007E4890 CTCWillResponse::GetInterfaceType: the virtual returns the class's interface type 0x9C.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWillResponse {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWillResponse::GetInterfaceType() const
{
    return (ETCInterfaceType)0x9C;
}
int main()
{
    CTCWillResponse object;
    const CTCWillResponse* p = &object;
    if ((long)p->GetInterfaceType() != 0x9C) { printf("007e4890_TEST FAIL\n"); return 1; }
    printf("007e4890_TEST PASS\n");
    return 0;
}
