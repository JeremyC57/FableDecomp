// Test for 0x004DE823 CTCParticleAttacher::GetInterfaceType: the virtual returns the class's interface type 0x82.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCParticleAttacher {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCParticleAttacher::GetInterfaceType() const
{
    return (ETCInterfaceType)0x82;
}
int main()
{
    CTCParticleAttacher object;
    const CTCParticleAttacher* p = &object;
    if ((long)p->GetInterfaceType() != 0x82) { printf("004de823_TEST FAIL\n"); return 1; }
    printf("004de823_TEST PASS\n");
    return 0;
}
