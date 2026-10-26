// Test for 0x004D5781 CTCGeneratesExperienceOnKilling::GetInterfaceType: the virtual returns the class's interface type 0xC9.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCGeneratesExperienceOnKilling {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCGeneratesExperienceOnKilling::GetInterfaceType() const
{
    return (ETCInterfaceType)0xC9;
}
int main()
{
    CTCGeneratesExperienceOnKilling object;
    const CTCGeneratesExperienceOnKilling* p = &object;
    if ((long)p->GetInterfaceType() != 0xC9) { printf("004d5781_TEST FAIL\n"); return 1; }
    printf("004d5781_TEST PASS\n");
    return 0;
}
