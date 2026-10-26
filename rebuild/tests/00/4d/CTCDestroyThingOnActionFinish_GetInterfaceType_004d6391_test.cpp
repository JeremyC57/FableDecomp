// Test for 0x004D6391 CTCDestroyThingOnActionFinish::GetInterfaceType: the virtual returns the class's interface type 0xFD.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDestroyThingOnActionFinish {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDestroyThingOnActionFinish::GetInterfaceType() const
{
    return (ETCInterfaceType)0xFD;
}
int main()
{
    CTCDestroyThingOnActionFinish object;
    const CTCDestroyThingOnActionFinish* p = &object;
    if ((long)p->GetInterfaceType() != 0xFD) { printf("004d6391_TEST FAIL\n"); return 1; }
    printf("004d6391_TEST PASS\n");
    return 0;
}
