// Test for 0x004DB44B CTCPlot::GetInterfaceType: the virtual returns the class's interface type 0xF1.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCPlot {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCPlot::GetInterfaceType() const
{
    return (ETCInterfaceType)0xF1;
}
int main()
{
    CTCPlot object;
    const CTCPlot* p = &object;
    if ((long)p->GetInterfaceType() != 0xF1) { printf("004db44b_TEST FAIL\n"); return 1; }
    printf("004db44b_TEST PASS\n");
    return 0;
}
