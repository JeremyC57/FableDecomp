// Test for 0x004DB03C CTCSmashableBarrier::GetInterfaceType: the virtual returns the class's interface type 0x4E.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSmashableBarrier {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSmashableBarrier::GetInterfaceType() const
{
    return (ETCInterfaceType)0x4E;
}
int main()
{
    CTCSmashableBarrier object;
    const CTCSmashableBarrier* p = &object;
    if ((long)p->GetInterfaceType() != 0x4E) { printf("004db03c_TEST FAIL\n"); return 1; }
    printf("004db03c_TEST PASS\n");
    return 0;
}
