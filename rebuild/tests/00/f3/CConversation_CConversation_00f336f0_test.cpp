// Behaviour test for CConversation::CConversation (0x00F336F0): the base is constructed, then the four CCharString vectors start
// empty (twelve zero dwords at +0x2C..+0x58 on the 32-bit target) while Lines (+0x28) keeps whatever was there.
#include <stdio.h>
#include <string.h>
#include <new>
static int g_baseBuilt;
struct DefBase { unsigned char Body[0x28]; DefBase() { ++g_baseBuilt; } };
struct StrVector { void* Start; void* Finish; void* EndOfStorage; StrVector() : Start(0), Finish(0), EndOfStorage(0) {} };
struct Conversation : DefBase {
    long Lines;
    StrVector Speaker, Dialogue, Animation, AnimLoop;
    Conversation() {}
};
int main()
{
    static unsigned char raw[sizeof(Conversation)];
    memset(raw, 0xCD, sizeof(raw));
    Conversation* c = new (raw) Conversation();
    if (g_baseBuilt != 1) { printf("00f336f0_TEST FAIL\n"); return 1; }
    if (c->Speaker.Start || c->Dialogue.Finish || c->Animation.EndOfStorage || c->AnimLoop.Start || c->AnimLoop.EndOfStorage) {
        printf("00f336f0_TEST FAIL\n"); return 1;
    }
    if (((unsigned char*)&c->Lines)[0] != 0xCD || ((unsigned char*)&c->Lines)[3] != 0xCD) { printf("00f336f0_TEST FAIL\n"); return 1; }
    printf("00f336f0_TEST PASS\n");
    return 0;
}
