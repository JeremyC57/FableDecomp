// Behaviour test for retail 0x009a0080: the candidate source below is embedded verbatim (from src/009a0080.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CBasicString<char>::Truncate(unsigned long) -- allocate the aligned prefix, release the old buffer, update the packed
//   capacity/length, and clear tail padding
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A0080, 0xa6 bytes. Allocator calls are operator new[] @ 0x00BFEB22 and
//   operator delete[] @ 0x00BFEB1C (MSVCR71 imports); the lane's <new> lacks the array forms, so they are declared here.
#include <string.h>
#include <stddef.h>
void* __cdecl operator new[](size_t size);             // 0x00BFEB22
void __cdecl operator delete[](void* p);               // 0x00BFEB1C

// Ego_r.pdb CBasicString<char> is a packed 13-byte subobject: PData +0,
// StringLength +4, DataLength:31 +8, UseFastExtend:1 +0xc. The actual packed
// CCharStringData layout corroborates this template field placement.
#pragma pack(push, 1)
template<class T> struct CBasicString {
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
    void Truncate(unsigned long length);
};
#pragma pack(pop)

template<class T> void CBasicString<T>::Truncate(unsigned long length) {
    if (length < StringLength) {
        unsigned long allocation = (length + 4) & ~3UL;
        T* data = new T[allocation];
        data = (T*)memcpy(data, PData, length);
        data[length] = 0;
        if (PData) {
            delete[] PData;
            PData = 0;
        }
        DataLength = allocation;
        PData = data;
        StringLength = length;
        PData[StringLength] = 0;
        unsigned long used = StringLength + 1;
        unsigned long end = (used + 3) & ~3UL;
        memset(PData + used, 0, end - used);
    }
}
template struct CBasicString<char>;

// ===== harness =====
#include <stdio.h>
#include <stdlib.h>
static int g_new, g_del; static size_t g_size; static void* g_deleted;
void* __cdecl operator new[](size_t n) { ++g_new; g_size = n; return malloc(n); }
void __cdecl operator delete[](void* p) { ++g_del; g_deleted = p; free(p); }
int main() {
    int ok = sizeof(CBasicString<char>) == 13;
    CBasicString<char> s; char* old = (char*)malloc(32); memcpy(old, "abcdefghijklmnop", 17);
    s.PData = old; s.StringLength = 16; s.DataLength = 32; s.UseFastExtend = true;
    s.Truncate(5);
    ok = ok && g_new == 1 && g_size == 8 && g_del == 1 && g_deleted == old;
    ok = ok && s.StringLength == 5 && s.DataLength == 8 && s.UseFastExtend && !strcmp(s.PData, "abcde");
    ok = ok && s.PData[6] == 0 && s.PData[7] == 0;                       /* tail padding cleared */
    char* keep = s.PData; s.Truncate(5); s.Truncate(9);                  /* not shorter: untouched */
    ok = ok && s.PData == keep && g_new == 1 && s.StringLength == 5;
    s.Truncate(0); ok = ok && g_size == 4 && s.StringLength == 0 && s.PData[0] == 0 && s.DataLength == 4;
    printf(ok ? "009a0080_TEST PASS\n" : "009a0080_TEST FAIL\n"); return ok ? 0 : 1;
}