// CActiveFile::OnReadFinished @ 009c3510
// push esi; mov esi,ecx; mov eax,[esi]; cmp eax,-1; je end;
// push eax; call [0x143fe30]; mov [esi],-1; end: pop esi; ret

// Import (NF415, 2026-11-25): was `(*g_pCloseSlot)(handle)` through a const pointer to the baked IAT slot 0x0143FE30; the slot is
//   KERNEL32!CloseHandle (Fable.exe import directory; reloc_bindings __imp__CloseHandle@4 -> 0143fe30). winbase.h BOOL CloseHandle(HANDLE).
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);   // IAT 0x0143FE30

struct CActiveFile
{
    void* handle;   // +0x00
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    if (this->handle != (void*)-1)
    {
        CloseHandle(this->handle);
        this->handle = (void*)-1;
    }
}