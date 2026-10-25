// Test for 0x00711CF0 CAnimateThing::CCompleteAnimation::CCompleteAnimation: the source object must clear the main
// set's vectors, ChangeAlphaBy, PAnimator and HadUpdate (leaving the other bits), and give both lists their own
// self-linked 0x5C-byte sentinel node.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
inline void* operator new(size_t, void* p) { return p; }
struct Node { Node* next; Node* prev; unsigned char value[0x54]; };
class CAnimateThing {
public:
    class CCompleteAnimation {
    public:
        CCompleteAnimation();
        unsigned char raw[0x60];
    };
};
int main()
{
    unsigned char buf[0x60];
    memset(buf, 0xff, sizeof(buf));
    CAnimateThing::CCompleteAnimation* c = new ((void*)buf) CAnimateThing::CCompleteAnimation;
    (void)c;
    bool ok = true;
    for (int k = 4; k < 0x1c; ++k) if (buf[k] != 0) ok = false;              // Anims, InternalAlphas
    for (int k = 0x38; k < 0x40; ++k) if (buf[k] != 0) ok = false;           // ChangeAlphaBy, PAnimator
    if (buf[0x51] != 0xf7) ok = false;                                       // only HadUpdate cleared
    if (buf[0x1c] != 0xff || buf[0x50] != 0xff || buf[0x5c] != 0xff) ok = false;
    Node* a; Node* b;
    memcpy(&a, buf + 0x54, 4); memcpy(&b, buf + 0x58, 4);
    if (!a || !b || a == b || a->next != a || a->prev != a || b->next != b || b->prev != b) ok = false;
    if (ok && _msize(a) != 0x5c) ok = false;
    if (!ok) { puts("00711cf0_TEST FAIL"); return 1; }
    free(a); free(b);
    puts("00711cf0_TEST PASS");
    return 0;
}
