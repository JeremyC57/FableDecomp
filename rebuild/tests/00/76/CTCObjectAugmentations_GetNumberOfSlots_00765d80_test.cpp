// Test for 0x00765D80 CTCObjectAugmentations::GetNumberOfSlots: links the source object; the result is the element
// count of the Augmentations vector (+0x14 / +0x18, 4-byte def pointers), 0 for an empty vector.
#include <stdio.h>
#include <string.h>
class CTCObjectAugmentations {
public:
    virtual void v00();
    long GetNumberOfSlots() const;
};
void CTCObjectAugmentations::v00() { }
static unsigned char g_obj[0x40];
static void* g_slots[8];
int main()
{
    CTCObjectAugmentations proto; memcpy(g_obj, &proto, 4);
    void* first = g_slots; void* last = g_slots + 5; void* eos = g_slots + 8;
    memcpy(g_obj + 0x14, &first, 4); memcpy(g_obj + 0x18, &last, 4); memcpy(g_obj + 0x1c, &eos, 4);
    const CTCObjectAugmentations* a = (const CTCObjectAugmentations*)g_obj;
    bool ok = a->GetNumberOfSlots() == 5;
    last = g_slots; memcpy(g_obj + 0x18, &last, 4);
    ok = ok && a->GetNumberOfSlots() == 0;
    if (!ok) { puts("00765d80_TEST FAIL"); return 1; }
    puts("00765d80_TEST PASS");
    return 0;
}
