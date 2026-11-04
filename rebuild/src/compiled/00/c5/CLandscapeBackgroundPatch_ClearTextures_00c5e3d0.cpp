// CSalFile::Close  @ retail 0x00c5e3d0
// Vendor naming wave V1 (one address, one name): 0x00C5E3D0 ?Close@CSalFile@@QAEXXZ (was ?ClearTextures@CLandscapeBackgroundPatch@@QAEXXZ)
// void __fastcall (modeled as __fastcall self in ecx)

// call dword ptr [0x143fe30] = KERNEL32.dll!CloseHandle (__imp__CloseHandle@4).
// push eax; call [mem]; no stack cleanup at call site -> callee cleans up (stdcall).
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);

class CSalFile
{ public:
    int field_0;      // +0x00
    int textureHandle; // +0x04

    void Close();
};

void CSalFile::Close()
{
    int h = this->textureHandle;
    if (h != -1)
    {
        CloseHandle((void*)h);
        this->textureHandle = -1;
    }
}