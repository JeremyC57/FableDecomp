// Test for 0x004DAF17 CTCVillageMember::GetInterfaceType: the virtual returns the class's interface type 0x23.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCVillageMember {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCVillageMember::GetInterfaceType() const
{
    return (ETCInterfaceType)0x23;
}
int main()
{
    CTCVillageMember object;
    const CTCVillageMember* p = &object;
    if ((long)p->GetInterfaceType() != 0x23) { printf("004daf17_TEST FAIL\n"); return 1; }
    printf("004daf17_TEST PASS\n");
    return 0;
}
