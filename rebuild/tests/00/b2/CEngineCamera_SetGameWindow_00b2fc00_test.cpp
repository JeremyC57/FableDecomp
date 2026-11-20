// Behaviour test for 0x00B2FC00 CEngineCamera::SetGameWindow (links the REAL row object). NF172 (q4582): rewritten for the renamed row
// (the old test only printed the pass line). The camera is raw storage (GameWindow at +0x10); the member is reached through a linker
// alias (thiscall = fastcall with a spare edx).
#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CallSetGameWindow(void* self, int spare, const void* window);
#pragma comment(linker, "/alternatename:@CallSetGameWindow@12=?SetGameWindow@CEngineCamera@@QAEXPBVC2DBoxF@@@Z")

static unsigned char gCamera[0x228];

int main()
{
    float box[4] = { 0.0f, 0.0f, 640.0f, 480.0f };
    const void* stored = 0;
    memset(gCamera, 0xcd, sizeof(gCamera));
    CallSetGameWindow(gCamera, 0, box);
    memcpy(&stored, gCamera + 0x10, sizeof(stored));
    if (stored != box || gCamera[0x0f] != 0xcd || gCamera[0x14] != 0xcd)
    {
        printf("FAIL_00b2fc00\n");
        return 1;
    }
    printf("OK_00b2fc00");
    return 0;
}
