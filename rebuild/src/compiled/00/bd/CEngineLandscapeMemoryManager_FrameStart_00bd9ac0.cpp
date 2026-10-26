class CEngineLandscapeMemoryManager { public:
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