// CVectorMap_00819ec0::UpperBound(const long&) -- binary search for the first 40-byte entry whose signed dword key at +0 is greater than the key
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00819EC0, oracle length 83 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,[ecx] ; mov ecx,[ecx+4] ; sub ecx,esi ; mov eax,magic ; imul ecx ; sar edx,k ; mov ecx,edx ; shr ecx,1Fh ; add ecx,edx ; test ecx,ecx ; jle R ; mov eax,[key] ; push ebx ; push edi ; mov edi,[eax] ; L: mov eax,ecx ; cdq ; sub eax,edx ; sar eax,1 ; lea edx,[eax*N] ; mov ebx,[esi+edx*4] ; cmp edi,ebx ; lea edx,[esi+edx*4] ; jl H ; lea esi,[edx+0x28] ; or edx,-1 ; sub edx,eax ; add ecx,edx ; jmp T ; H: mov ecx,eax ; T: test ecx,ecx ; jg L ; pop edi ; pop ebx ; R: mov eax,esi ; pop esi ; ret 4
// Dinkumware `upper_bound` over the map's contiguous 40-byte entries (count = byte span divided
// by 40 through the multiply-by-reciprocal idiom): halve the count, and unless the key is less
// than the middle entry's key step past the middle (`count -= half + 1`), else keep the lower
// half; the key reference is read once. The negated comparison puts the advance in the
// fall-through block. The manifest label is propagated; the entry's payload is opaque. One of
// 8 bodies of this shape.
struct SEntry {
    long m_key;                            // +0 signed key
    unsigned char _payload[0x28 - 4];
};

struct CVectorMap_00819ec0 {
    SEntry* m_begin;
    SEntry* m_end;
    SEntry* UpperBound_CVectorMap_00819ec0(const long& key);
};

SEntry* CVectorMap_00819ec0::UpperBound_CVectorMap_00819ec0(const long& key)
{
    SEntry* first = m_begin;
    long count = m_end - m_begin;
    while (0 < count) {
        long half = count / 2;
        SEntry* mid = first + half;
        if (!(key < mid->m_key)) {
            first = ++mid;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
