// std::fill(pair<unsigned long, CSoundPair>* first, pair<unsigned long, CSoundPair>* last, const pair<unsigned long, CSoundPair>& val) -- assign val to every element of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x009D1BB0, 49 bytes, `ret 4`: __fastcall (first ecx, last edx, the value reference on the
// stack). Call sites 0x009D3ACA, 0x009D3B27 (the sound module's vector<pair<unsigned long, CSoundPair> > insert path).
// Types (Ego_r.pdb): CSoundPair (8 bytes) = FirstSound +0, LastSound +4; the pair is 12 bytes, all POD.
// Shape: the pair's implicit (trivial) copy assignment copies the whole 12-byte object as one block: VC7.1 sets up the
// string-move registers (`mov esi,eax` / `mov edi,ecx`) and moves the three words through ebx; the loop head is
// aligned behind a `jmp` (/O2).
struct CSoundPair {                             // 8 bytes
    long FirstSound;                            // +0x00
    long LastSound;                             // +0x04
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
typedef pair<unsigned long, CSoundPair> CSoundEntry;
template void __fastcall fill(CSoundEntry*, CSoundEntry*, const CSoundEntry&);
}