// CThing::GetTCDriver<CTCDPhysicalObstruction>(CTCDPhysicalObstruction** out) -- if the thing uses the physical-obstruction driver (interface 0xed), hand back its driver pointer and return true
// Leaf: GetTCDriver
// Retail: Fable.exe (TLC, VC7.1) @ 0x007890EA, 39 bytes. Callees: bitset<274>::test(size_t) @ 0x00410DE0 (out of
// line, on UsingComponents at +0x20).
// Shape: unlike the inlined-word-test instantiations (0x004ACB50), this one calls the bitset's out-of-line test;
// PTCDriver is retail +0x5c (PDB +0x6c). The unaligned start and the shared `pop esi ; ret 4` tail entered by
// `mov al,1 ; jmp` mark a size-optimised TU (`#pragma optimize("s", on)`).
#pragma optimize("s", on)
struct CTCDriverBase;
struct CTCDPhysicalObstruction;

struct CTCBitset {                                   // std::bitset<274>
    unsigned long m_words[9];
    bool test(unsigned long position) const;         // 0x00410DE0
};

struct CThing {
    void*          __vftable;                        // +0x00
    unsigned char  _pad04[0x20 - 0x04];
    CTCBitset      UsingComponents;                  // +0x20
    unsigned char  _pad44[0x5c - 0x44];
    CTCDriverBase* PTCDriver;                        // +0x5c (PDB +0x6c)

    template <class T> bool GetTCDriver(T** out);
};

template <class T> bool CThing::GetTCDriver(T** out)
{
    if (UsingComponents.test(0xed)) {
        *out = (T*)PTCDriver;
        return true;
    }
    return false;
}

template bool CThing::GetTCDriver<CTCDPhysicalObstruction>(CTCDPhysicalObstruction** out);