class CGraphicDataBank { public:
    char pad[0x1e0];
    char* f1e0;
};

void __fastcall NotifyEntryRemoved(CGraphicDataBank* bank, int /*edx*/, unsigned long idx)
{
    char* base = bank->f1e0;
    *(int*)(base + idx * 0x2c + 0x24) = -1;
}