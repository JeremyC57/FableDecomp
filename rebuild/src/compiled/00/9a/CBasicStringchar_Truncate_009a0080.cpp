// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: calls scalar operator new/delete where retail calls operator new[] 0xbfeb22 / operator delete[] 0xbfeb1c)
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