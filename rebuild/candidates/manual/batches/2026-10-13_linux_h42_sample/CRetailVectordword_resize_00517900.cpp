// CRetailVector_dword::resize(unsigned long, const long&) -- shrink through the inlined erase (memmove copy @ 0x00BFEAE6) or grow through Insert @ 0x005149F0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00517900, oracle length 86 bytes.
// Grade: candidate -- still UNGRADED in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  mov edx,[esp+4] ; push esi ; push edi ; mov edi,ecx ; mov eax,[edi+4] ; mov esi,[edi] ; mov ecx,eax ; sub ecx,esi ; sar ecx,2 ; cmp edx,ecx ; jae G ; cmp eax,eax ; lea ecx,[esi+edx*4] ; jne C ; mov eax,ecx ; mov [edi+4],eax ; pop edi ; pop esi ; ret 8 ; C: mov esi,eax ; sub esi,eax ; push esi ; push eax ; push ecx ; call memmove ; add esp,0Ch ; add eax,esi ; mov [edi+4],eax ; pop edi ; pop esi ; ret 8 ; G: mov esi,[value] ; push esi ; sub edx,ecx ; push edx ; push eax ; mov ecx,edi ; call Insert ; pop edi ; pop esi ; ret 8
// Retail vector resize for 4-byte elements: when `n < size()` the tail is erased through the
// inlined `erase(first + n, last)` -- `last = copy(last, last, first + n)` with the memmove
// copy (the compiler keeps the `cmp eax,eax` of the empty-range test and the dead memmove
// branch); otherwise `Insert(last, n - size(), value)` @ 0x005149F0 appends the missing
// elements. The manifest label (`vector<unsigned_int>::resize`) is propagated. One of 17 bodies.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);

struct CRetailVector_dword {
    long* first;
    long* last;
    long* end;
    void Insert(long* where, unsigned long count, const long& value);   // 0x005149F0
    static long* Copy(long* f, long* l, long* result)
    {
        if (f != l) {
            unsigned long bytes = (char*)l - (char*)f;
            result = (long*)((char*)memmove(result, f, bytes) + bytes);
        }
        return result;
    }
    long* Erase(long* f, long* l)
    {
        long* s = Copy(l, last, f);
        last = s;
        return f;
    }
    void resize(unsigned long n, const long& value);
};

void CRetailVector_dword::resize(unsigned long n, const long& value)
{
    if (n < (unsigned long)(last - first))
        Erase(first + n, last);
    else
        Insert(last, n - (last - first), value);
}
