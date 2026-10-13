// CRetailVector_00618ef0::resize(unsigned long, const T&) -- 8-byte elements: shrinking copies the (empty) tail down with memmove through the inlined helper and moves `last`, growing calls InsertN @ 0x00617FD0 at the end
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00618EF0, oracle length 87 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov edx,[n] ; push esi ; push edi ; mov edi,ecx ; mov eax,[edi+4] ; mov esi,[edi] ; mov ecx,eax ; sub ecx,esi ; sar ecx,3 ; cmp edx,ecx ; jae G ; cmp eax,eax ; lea ecx,[esi+edx*8] ; jne M ; mov eax,ecx ; mov [edi+4],eax ; pop edi ; pop esi ; ret 8 ; M: mov esi,eax ; sub esi,eax ; push esi ; push eax ; push ecx ; call memmove ; add esp,0Ch ; add eax,esi ; mov [edi+4],eax ; pop edi ; pop esi ; ret 8 ; G: mov esi,[value] ; push esi ; sub edx,ecx ; push edx ; push eax ; mov ecx,edi ; call InsertN ; pop edi ; pop esi ; ret 8
// Retail vector resize for 8-byte scalars: when the new size is smaller, `erase(begin() + n,
// end())` is inlined as the copy-down helper -- `if (last != end)` is kept although both are
// the end pointer (`cmp eax,eax`), the tail is moved with memmove (@ 0x00BFEAE6) and the new end
// is the destination plus the byte count -- followed by the store of `last`; otherwise
// InsertN @ 0x00617FD0 appends `n - size()` copies of the value at the end. The manifest label
// is propagated across element sizes; names are descriptive. One of 17 bodies of this shape.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);
struct SElement8 { unsigned char _bytes[8]; };

struct CRetailVector_00618ef0 {
    SElement8* first;                 // +0
    SElement8* last;                  // +4
    SElement8* end;                   // +8
    void InsertN(SElement8* where, unsigned long count, const SElement8& value);   // 0x00617FD0
    static SElement8* CopyDown(SElement8* src, SElement8* srcEnd, SElement8* dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long bytes = (unsigned long)((char*)srcEnd - (char*)src);
        return (SElement8*)((char*)memmove(dest, src, bytes) + bytes);
    }
    void Resize_CRetailVector_00618ef0(unsigned long n, const SElement8& value);
};

void CRetailVector_00618ef0::Resize_CRetailVector_00618ef0(unsigned long n, const SElement8& value)
{
    if (n < (unsigned long)(last - first))
        last = CopyDown(last, last, first + n);   // erase(begin() + n, end())
    else
        InsertN(last, n - (last - first), value);
}
