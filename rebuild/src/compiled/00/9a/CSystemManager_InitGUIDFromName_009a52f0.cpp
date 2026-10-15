// CSystemManager::InitGUIDFromName(const char*) -- build the application GUID from the first 16 name bytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A52F0, length 61 bytes. No callees (strlen is the
// inlined intrinsic: the `mov cl,[eax] ; inc eax ; test cl,cl ; jne` scan minus the start + 1).
//
// Shape: the 16 bytes of ApplicationGUID (+0x0c in both the PDB and the retail layout) are filled from
// the name, padded with zero bytes once the string is exhausted: `name[i]` for i < strlen(name), else 0.
// The odd `sub esi,edx` / `lea edi,[esi+ecx] ; mov bl,[edi+edx]` addressing is VC7.1's strength
// reduction of `name[i]` against the destination pointer, not a source construct. The two `nop`-style
// fillers (`nop`, `lea ecx,[ecx]`) are loop-head alignment. No engine header exists for CSystemManager
// (PDB 0x178 bytes); only the leading members up to the GUID are modelled.
#include <string.h>

struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};

struct CSystemManager {
    void*         _vtable;                  // +0x00
    unsigned char _pad04[4];                // +0x04
    bool          WindowsQuit;              // +0x08
    bool          ApplicationActive;        // +0x09
    bool          ApplicationHasInputFocus; // +0x0a
    unsigned char _pad0b;
    _GUID         ApplicationGUID;          // +0x0c
    _GUID         BestGUID;                 // +0x1c

    void InitGUIDFromName(const char* name);
};

void CSystemManager::InitGUIDFromName(const char* name)
{
    char* guid = (char*)&ApplicationGUID;
    long length = strlen(name);
    for (long i = 0; i < 16; ++i)
        guid[i] = (i < length) ? name[i] : '\0';
}