// CMusicManager::IsDangerMusicEnabled @ 0x006bd760
// Reads pointer fields at +8, +0xc, +4, +0x10, returns first non-null,
// else field at +0 (mov eax,[ecx]). Returns raw pointer value in eax.

class CMusicManager { public:
    void* f0;   // +0x00
    void* f4;   // +0x04
    void* f8;   // +0x08
    void* fc;   // +0x0c
    void* f10;  // +0x10
};

void* __fastcall IsDangerMusicEnabled(CMusicManager* manager)
{
    void* p;
    if ((p = manager->f8) != 0) return p;
    if ((p = manager->fc) != 0) return p;
    if ((p = manager->f4) != 0) return p;
    if ((p = manager->f10) != 0) return p;
    return manager->f0;
}