// SBufferRecord_00c8db30::Clear() -- free the buffer held in the first word, then zero the whole eight-word record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8DB30, length 42 bytes (48 with padding).
// Leaf: Clear
// The chunk label (CSoundInit::CSoundInit) is propagated; this is no constructor (`ret` with no `mov eax,esi`).
// Callees: MSVCR71 free (IAT 0x01440154, `call dword ptr [__imp_free]`; its `add esp,4` is scheduled into
// the zero stores). Shape: the buffer word read and pushed unconditionally (no null test), then a 32-byte
// intrinsic memset as eight stores of a zero register through the pointer kept in esi -- the libogg/libvorbis
// `_ogg_free(x->buffer); memset(x, 0, sizeof(*x));` clear idiom (the exe links libVorbis I 20030909).
#include <string.h>

extern "C" __declspec(dllimport) void __cdecl free(void* block);   // IAT 0x01440154

struct SBufferRecord_00c8db30 {
    void* m_pBuffer;                            // +0x00: malloc'd storage
    long  m_4, m_8, m_c, m_10, m_14, m_18, m_1c;
    void Clear();
};

void SBufferRecord_00c8db30::Clear()
{
    free(m_pBuffer);
    memset(this, 0, sizeof(*this));
}
