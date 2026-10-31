// Grade: DIFFER(636v682) 173 diff lines (parked 732v682, 203 lines): the lowest-set-bit scan as a do-while keeps retail's rolled 32-step loop; left: retail keeps the list walk's item in a frame word ([esp+14h]) and indexes item->Keys[i] ^ list->Keys[i] through a combined lea/sub address where VC7.1 here strength-reduces to a pointer, the frame is 8 bytes short (0x145c v 0x1464) and the bits counter is not homed
// CEngineTreeFilerBase::ConvertListToTree(CNodeHeader* list) -- turn a sibling list of key nodes into a radix tree: find the lowest key bit where the list differs, widen the bucket index until the 10-bit key prefixes stop colliding, then hash the list into a new branch node and recurse into each multi-entry bucket
// Leaf: ?ConvertListToTree@CEngineTreeFilerBase
// Flags: /GS
#include <string.h>
void* __cdecl operator new[](unsigned int size);               // 0x00BFEB22
struct CNodeHeader {
    unsigned char Type;                                        // +0x00
    unsigned char Word;                                        // +0x01
    unsigned char Shift;                                       // +0x02
    unsigned char Bits;                                        // +0x03
    long          Unknown04;                                   // +0x04
    CNodeHeader*  Prev;                                        // +0x08
    CNodeHeader*  Next;                                        // +0x0c
    CNodeHeader*  FirstChild;                                  // +0x10
    CNodeHeader*  LastChild;                                   // +0x14
    CNodeHeader*  Parent;                                      // +0x18
    unsigned long Keys[1];                                     // +0x1c (branch nodes: child buckets)
};
struct CEngineTreeFilerBase {
    void* __vftable;
    long  KeyWords;                                            // +0x04
    CNodeHeader* ConvertListToTree(CNodeHeader* list);
};
CNodeHeader* CEngineTreeFilerBase::ConvertListToTree(CNodeHeader* list)
{
    unsigned long diff[16];
    unsigned long keys[1024];
    unsigned char used[1024];
    if (!list->Next)
        return list;
    long count = KeyWords;
    for (long i = 0; i < count; ++i)
        diff[i] = 0;
    for (CNodeHeader* item = list->Next; item; item = item->Next)
        for (long i = 0; i < count; ++i)
            diff[i] |= item->Keys[i] ^ list->Keys[i];
    long shift = 256;
    long word;
    for (word = 0; word < count; ++word) {
        if (diff[word]) {
            long b = 0;
            do {
                if ((diff[word] & (1 << b)) && shift >= b)
                    shift = b;
            } while (++b < 32);
            break;
        }
    }
    long bits = 1;
    for (;;) {
        unsigned long mask = (1 << bits) - 1;
        memset(used, 0, 1 << bits);
        CNodeHeader* item;
        for (item = list; item; item = item->Next) {
            unsigned long key = item->Keys[word];
            unsigned long slot = (key >> shift) & mask;
            if (used[slot]) {
                unsigned long prefix = 0x3ff << shift;
                if ((key & prefix) != (keys[slot] & prefix))
                    break;
            }
            else {
                used[slot] = 1;
                keys[slot] = key;
            }
        }
        if (!item)
            break;
        ++bits;
    }
    long size = 4 << bits;
    CNodeHeader* node = (CNodeHeader*)operator new[](size + 0x1c);
    node->Word = (unsigned char)word;
    node->Bits = (unsigned char)bits;
    node->Shift = (unsigned char)shift;
    node->FirstChild = 0;
    node->LastChild = 0;
    node->Prev = 0;
    node->Next = 0;
    node->Parent = 0;
    node->Unknown04 = 0;
    node->Type = 0;
    CNodeHeader** buckets = (CNodeHeader**)node->Keys;
    memset(buckets, 0, size);
    long buckets_count = 1 << bits;
    unsigned long mask = buckets_count - 1;
    CNodeHeader* item = list;
    do {
        unsigned long slot = (item->Keys[word] >> shift) & mask;
        CNodeHeader* next = item->Next;
        item->Next = buckets[slot];
        item->Prev = 0;
        buckets[slot] = item;
        item = next;
    } while (item);
    for (long i = 0; i < buckets_count; ++i) {
        if (buckets[i]) {
            if (buckets[i]->Next)
                buckets[i] = ConvertListToTree(buckets[i]);
            buckets[i]->Parent = node;
            buckets[i]->Prev = node->LastChild;
            if (node->LastChild) {
                node->LastChild->Next = buckets[i];
                node->LastChild = buckets[i];
            }
            else {
                node->LastChild = buckets[i];
                node->FirstChild = buckets[i];
            }
        }
    }
    return node;
}
