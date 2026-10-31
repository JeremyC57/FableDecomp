#include <stdio.h>
#include <string.h>
#include <new>

/* model under test: the CGameEvent(long) initialiser-list ctor of the source */
class CGameEvent {
public:
    long          Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool          Valid;
    bool          Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};

int main()
{
    unsigned char buf[sizeof(CGameEvent)];
    memset(buf, 0xAA, sizeof(buf));
    CGameEvent* ev = new (buf) CGameEvent(0x12345678);
    int ok = 1;
    if ((void*)ev != (void*)buf) ok = 0;
    if (sizeof(CGameEvent) != 0x28) ok = 0;
    if (ev->Type != 0x12345678) ok = 0;
    if (ev->Player != -1) ok = 0;
    if (ev->EndPos != 0) ok = 0;
    if (ev->Valid != true) ok = 0;
    if (ev->Replacement != false) ok = 0;
    if (ev->Data[0] != 0xAA) ok = 0;   /* data left untouched */
    if (ok) printf("OK_0x004159b0\n");
    else printf("FAIL_0x004159b0\n");
    return ok ? 0 : 1;
}
