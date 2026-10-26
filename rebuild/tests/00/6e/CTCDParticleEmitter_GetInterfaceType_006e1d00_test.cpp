// Test for 0x006E1D00 CTCDParticleEmitter::GetInterfaceType: the virtual returns the class's interface type 0x4B.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDParticleEmitter {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDParticleEmitter::GetInterfaceType() const
{
    return (ETCInterfaceType)0x4B;
}
int main()
{
    CTCDParticleEmitter object;
    const CTCDParticleEmitter* p = &object;
    if ((long)p->GetInterfaceType() != 0x4B) { printf("006e1d00_TEST FAIL\n"); return 1; }
    printf("006e1d00_TEST PASS\n");
    return 0;
}
