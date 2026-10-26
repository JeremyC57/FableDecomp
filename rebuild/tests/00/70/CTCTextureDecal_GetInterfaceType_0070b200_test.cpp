// Test for 0x0070B200 CTCTextureDecal::GetInterfaceType: the virtual returns the class's interface type 0xE9.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTextureDecal {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTextureDecal::GetInterfaceType() const
{
    return (ETCInterfaceType)0xE9;
}
int main()
{
    CTCTextureDecal object;
    const CTCTextureDecal* p = &object;
    if ((long)p->GetInterfaceType() != 0xE9) { printf("0070b200_TEST FAIL\n"); return 1; }
    printf("0070b200_TEST PASS\n");
    return 0;
}
