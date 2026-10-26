// Test for 0x004E0C8E CTCHaste::GetInterfaceType: the virtual returns the class's interface type 0x8C.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHaste {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHaste::GetInterfaceType() const
{
    return (ETCInterfaceType)0x8C;
}
int main()
{
    CTCHaste object;
    const CTCHaste* p = &object;
    if ((long)p->GetInterfaceType() != 0x8C) { printf("004e0c8e_TEST FAIL\n"); return 1; }
    printf("004e0c8e_TEST PASS\n");
    return 0;
}
