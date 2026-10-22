#include <stdio.h>

struct CEngineLandscapeMemoryManager {
    char pad0[0x34];
    int frame;
    char pad1[0x40 - 0x34 - 4];
    unsigned char flag;
};

void __fastcall FrameStart(CEngineLandscapeMemoryManager* manager)
{
    manager->frame = (manager->frame + 1) % 3;
    manager->flag = 1;
}

int main()
{
    CEngineLandscapeMemoryManager m;
    m.frame = 0; m.flag = 0;
    FrameStart(&m);
    if (m.frame != 1 || m.flag != 1) { printf("FAIL a\n"); return 1; }
    FrameStart(&m);
    if (m.frame != 2) { printf("FAIL b\n"); return 1; }
    FrameStart(&m);
    if (m.frame != 0) { printf("FAIL c\n"); return 1; }
    m.frame = 5; m.flag = 0;
    FrameStart(&m);
    if (m.frame != 0 || m.flag != 1) { printf("FAIL d\n"); return 1; }
    printf("OK_00bd9ac0\n");
    return 0;
}