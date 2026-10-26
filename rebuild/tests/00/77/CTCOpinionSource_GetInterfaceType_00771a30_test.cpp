// Test for 0x00771A30 CTCOpinionSource::GetInterfaceType: the virtual returns the class's interface type 0x50.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCOpinionSource {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCOpinionSource::GetInterfaceType() const
{
    return (ETCInterfaceType)0x50;
}
int main()
{
    CTCOpinionSource object;
    const CTCOpinionSource* p = &object;
    if ((long)p->GetInterfaceType() != 0x50) { printf("00771a30_TEST FAIL\n"); return 1; }
    printf("00771a30_TEST PASS\n");
    return 0;
}
