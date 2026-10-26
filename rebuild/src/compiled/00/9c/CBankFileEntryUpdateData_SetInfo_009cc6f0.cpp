// CBankFileEntryUpdateData::SetInfo(const CArray<char>&) -- replace the owned info buffer with a copy of the array
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC6F0, oracle length 89 bytes.
// Callees: operator delete[] @ 0x00BFEB1C, operator new[] @ 0x00BFEB22 (both __cdecl, `add esp,4`); the copy is the
//          intrinsic memcpy (`rep movsd ; rep movsb`).
//
// Retail bytes:  push ebx ; mov ebx,ecx ; mov eax,[ebx+8] ; push esi ; xor esi,esi ; cmp eax,esi ; je A ; push eax ; call operator delete[] ; add esp,4 ; mov [ebx+8],esi ;
//                A: mov [ebx+4],esi ; mov esi,[esp+0Ch] ; mov eax,[esi+4] ; sub eax,[esi] ; mov [ebx+4],eax ; je E ; push eax ; call operator new[] ; mov [ebx+8],eax ;
//                mov ecx,[esi] ; mov edx,[esi+4] ; add esp,4 ; sub edx,ecx ; je B ; mov esi,ecx ; B: mov ecx,[ebx+4] ; push edi ; mov edi,eax ; mov eax,ecx ; shr ecx,2 ;
//                rep movsd ; mov ecx,eax ; and ecx,3 ; rep movsb ; pop edi ; E: pop esi ; pop ebx ; ret 4
// Free the old buffer (size then zeroed), take the array's byte count, and when it is non-zero
// allocate and memcpy from the array's data accessor; that inline accessor hands back the first
// element for a non-empty array and the array object's own address otherwise (`mov esi,ecx` is
// skipped when end == first), which is why the source register is conditionally rewritten.  The
// element count is re-read from the member for the copy length.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22 (the lane <new> lacks it)
struct CArray_char {                 // retail CArray<char>: 12 bytes
    char* m_pFirst;
    char* m_pLast;
    char* m_pEnd;
    long GetCount() const { return m_pLast - m_pFirst; }
    const char* GetData() const { return m_pLast - m_pFirst != 0 ? m_pFirst : reinterpret_cast<const char*>(this); }
};

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int count);

class CBankFileEntryUpdateData { public:
    unsigned char _pad[4];
    unsigned long m_infoSize;        // +0x04
    char*         m_pInfo;           // +0x08
    void SetInfo(const CArray_char& info);   // 0x009CC6F0
};

void CBankFileEntryUpdateData::SetInfo(const CArray_char& info)
{
    if (m_pInfo != 0) {
        delete[] m_pInfo;
        m_pInfo = 0;
    }
    m_infoSize = 0;
    m_infoSize = info.GetCount();
    if (m_infoSize != 0) {
        m_pInfo = new char[m_infoSize];
        memcpy(m_pInfo, info.GetData(), m_infoSize);
    }
}