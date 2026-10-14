// InitStreamEntry_00c8f080(SStreamKey_00c8f080* key, CStreamEntry_00c8f080* entry) -- libpk entry set-up: clear the 0x70-byte entry, bind it to its key, and when the key is live allocate the 0x48-byte state block, initialise the entry's byte buffer and seed the block's threshold with -9999.0f; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8F080, length 68 bytes (the oracle row carries 12 bytes of int3 padding).
// Callees: calloc (IAT 0x01440058: `push 48h ; push 1`, cdecl `add esp,8`), CByteBuffer_00c8f080::Init @ 0x00C90F10 (thiscall on the member at +4:
// five zero words and a 0x100-byte malloc; the manifest label "CVsyncCallbackTimerInfo" there is propagated). Shape: the libpk module is
// compiled `__fastcall` (/Gr): the key arrives in ecx (moved to edx), the entry in edx (esi); `rep stosd` is the 28-word memset; the two
// redundant zero stores after it (+0x4c then +0x44) are explicit assignments; the float constant is stored as an immediate dword through a LOCAL copy of the
// calloc result kept in edi across the Init call (re-reading the member would reload it after the call). The
// manifest label "CEntry::CEntry" is propagated (no `this` return); names are descriptive.
#include <string.h>
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);   // IAT 0x01440058 (`call dword ptr [__imp_calloc]`)

struct SStreamKey_00c8f080 {
    long m_live;                                   // +0x00 non-zero when the key is live
};

struct SStreamState_00c8f080 {                     // 0x48 bytes, calloc'd
    long  m_0;
    float m_threshold;                             // +0x04 seeded with -9999.0f
    unsigned char _rest[0x40];
};

struct CByteBuffer_00c8f080 {                      // 0x14 bytes
    long m_0, m_4;
    unsigned char* m_pData;                        // +0x08
    unsigned char* m_pCursor;                      // +0x0c
    long m_capacity;                               // +0x10
    void Init();                                   // 0x00C90F10
};

struct CStreamEntry_00c8f080 {                     // 0x70 bytes
    long                   m_0;
    CByteBuffer_00c8f080   m_buffer;               // +0x04
    unsigned char          _pad0[0x28];            // +0x18..+0x40
    SStreamKey_00c8f080*   m_pKey;                 // +0x40
    long                   m_44;                   // +0x44
    long                   m_48;
    long                   m_4c;                   // +0x4c
    unsigned char          _pad1[0x18];            // +0x50..+0x68
    SStreamState_00c8f080* m_pState;               // +0x68
    long                   m_6c;
};

long __fastcall InitStreamEntry_00c8f080(SStreamKey_00c8f080* key, CStreamEntry_00c8f080* entry)
{
    memset(entry, 0, sizeof(*entry));
    entry->m_pKey = key;
    entry->m_4c = 0;
    entry->m_44 = 0;
    if (key->m_live != 0) {
        SStreamState_00c8f080* state = (SStreamState_00c8f080*)calloc(1, sizeof(SStreamState_00c8f080));
        entry->m_pState = state;
        entry->m_buffer.Init();
        state->m_threshold = -9999.0f;
    }
    return 0;
}
