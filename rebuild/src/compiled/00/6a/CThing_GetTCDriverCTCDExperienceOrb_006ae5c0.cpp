// CThing::GetTCDriver<CTCDExperienceOrb>(CTCDExperienceOrb** out) -- if the thing uses the driver component tested here (interface bit 105), hand back its driver pointer and return true
// Leaf: GetTCDriver
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AE5C0, 27 bytes. No callees.
// Identity: the catalog name is propagated from the byte-identical 0x004ACB50 family; this instantiation tests
// interface bit 105 (bitset word 3 at +0x2c, mask 0x200), so it is a GetTCDriver<T> for a different driver
// type. The driver type is kept as the catalog's for the symbol; only the bit differs.
// Shape: the inlined HasTC probe is the raw bitset word test in the `if` (`mov eax,[ecx+2Ch] ; test ah,2`);
// PTCDriver is retail +0x5c (PDB +0x6c). `mov al,1` / `xor al,al` = bool.
class CTCDriverBase;
class CTCDExperienceOrb;

class CThing { public:
    void*          __vftable;                        // +0x00
    unsigned char  _pad04[0x20 - 0x04];
    unsigned long  UsingComponents[9];               // +0x20 bitset<274>
    unsigned char  _pad44[0x5c - 0x44];
    CTCDriverBase* PTCDriver;                        // +0x5c (PDB +0x6c)

    template <class T> bool GetTCDriver(T** out);
};

template <class T> bool CThing::GetTCDriver(T** out)
{
    if (UsingComponents[3] & 0x200) {
        *out = (T*)PTCDriver;
        return true;
    }
    return false;
}

template bool CThing::GetTCDriver<CTCDExperienceOrb>(CTCDExperienceOrb** out);