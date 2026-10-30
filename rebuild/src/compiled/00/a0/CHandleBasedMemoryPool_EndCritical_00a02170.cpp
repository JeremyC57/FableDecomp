struct CRIT { int x; };
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void*);   // IAT 0x0143FE10 = KERNEL32.dll!LeaveCriticalSection

class CHandleBasedMemoryPool { public:
    char pad[0x24];
    char flag;
    char pad2[0x44c - 0x25];
    CRIT cs;
};

void __fastcall EndCritical(CHandleBasedMemoryPool* self)
{
    if (self->flag) {
        LeaveCriticalSection(&self->cs);
    }
}