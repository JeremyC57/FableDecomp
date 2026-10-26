// Test for 0x0077C790 NGraphicAppearance::CTCGraphicAppearanceSprite::GetInterfaceType: the virtual returns the class's interface type 0x5C.
#include <stdio.h>
enum ETCInterfaceType { };
namespace NGraphicAppearance {
class CTCGraphicAppearanceSprite {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
}
ETCInterfaceType NGraphicAppearance::CTCGraphicAppearanceSprite::GetInterfaceType() const
{
    return (ETCInterfaceType)0x5C;
}
int main()
{
    NGraphicAppearance::CTCGraphicAppearanceSprite object;
    const NGraphicAppearance::CTCGraphicAppearanceSprite* p = &object;
    if ((long)p->GetInterfaceType() != 0x5C) { printf("0077c790_TEST FAIL\n"); return 1; }
    printf("0077c790_TEST PASS\n");
    return 0;
}
