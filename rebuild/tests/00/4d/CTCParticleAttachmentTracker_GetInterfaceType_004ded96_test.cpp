// Test for 0x004DED96 CTCParticleAttachmentTracker::GetInterfaceType: the virtual returns the class's interface type 0xD8.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCParticleAttachmentTracker {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCParticleAttachmentTracker::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD8;
}
int main()
{
    CTCParticleAttachmentTracker object;
    const CTCParticleAttachmentTracker* p = &object;
    if ((long)p->GetInterfaceType() != 0xD8) { printf("004ded96_TEST FAIL\n"); return 1; }
    printf("004ded96_TEST PASS\n");
    return 0;
}
