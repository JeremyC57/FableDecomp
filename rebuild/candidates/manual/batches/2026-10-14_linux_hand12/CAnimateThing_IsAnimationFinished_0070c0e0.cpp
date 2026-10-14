// CAnimateThing::IsAnimationFinished() -- finished when there is no current animation, its first entry has no frames left, or the "finished" flag is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C0E0, length 31 bytes (+ int3 padding).
// Callees: none.
// Layout: the current animation's entry vector at +0x04 (first) / +0x08 (last), its element starting with a
// signed short count; the state flags byte at +0x51 (bit 1 = finished).
// Shape: nested ifs with the `return 0` innermost (the three tests jump to the shared `mov eax,1` tail, the
// `xor eax,eax` sits in the middle); a long result (full register), not a bool -- separate `return 1` tests
// make VC extract the flag bit arithmetically instead. The element is reached through the vector's iterator object
// (`mov edx,eax` re-derives `*begin()`), the shape of the landed CRetailVector bodies.
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) { }
        T& operator*() const { return *p; }
    };
    iterator begin() const { return iterator(first); }
    bool empty() const { return first == last; }
};

struct SAnimFrameEntry {
    short m_framesLeft;                         // +0x00 signed
    short m_pad;
};

struct CAnimateThing {
    long m_value0;                              // +0x00
    CRetailVector<SAnimFrameEntry> m_entries;   // +0x04 .. +0x0f
    unsigned char _pad[0x41];                   // +0x10 .. +0x50
    unsigned char m_flags;                      // +0x51 bit 1: finished
    long IsAnimationFinished() const;
};

long CAnimateThing::IsAnimationFinished() const
{
    if (!m_entries.empty()) {
        if ((*m_entries.begin()).m_framesLeft > 0) {
            if (!(m_flags & 2))
                return 0;
        }
    }
    return 1;
}
