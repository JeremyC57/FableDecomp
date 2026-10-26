// Test for 0x004E3386 CTCNymph::GetInterfaceType: the virtual returns the class's interface type 0xF5.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCNymph {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCNymph::GetInterfaceType() const
{
    return (ETCInterfaceType)0xF5;
}
int main()
{
    CTCNymph object;
    const CTCNymph* p = &object;
    if ((long)p->GetInterfaceType() != 0xF5) { printf("004e3386_TEST FAIL\n"); return 1; }
    printf("004e3386_TEST PASS\n");
    return 0;
}
