// Test for 0x0055DE00 CTCPhotojournalCapture::GetInterfaceType: the virtual returns the class's interface type 0x10E.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCPhotojournalCapture {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCPhotojournalCapture::GetInterfaceType() const
{
    return (ETCInterfaceType)0x10E;
}
int main()
{
    CTCPhotojournalCapture object;
    const CTCPhotojournalCapture* p = &object;
    if ((long)p->GetInterfaceType() != 0x10E) { printf("0055de00_TEST FAIL\n"); return 1; }
    printf("0055de00_TEST PASS\n");
    return 0;
}
