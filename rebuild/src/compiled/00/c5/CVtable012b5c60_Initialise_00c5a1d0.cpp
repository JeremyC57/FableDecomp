// CVtable_012B5C60::Initialise(CAudioObject* sound)  retail 0x00c5a1d0 -- slot 1 of the derived libpk voice vtable 0x012B5C60
// (and of 0x012B5CD0, which inherits it; was catalogued CTCStealth::OnCreate, a propagated label)
// Leaf: ?Initialise@CVtable_012B5C60@@UAEJPAVCAudioObject@@@Z
// xor eax,eax; [ecx+0x1e8]=0x1000; [ecx+0x1ec]=0; [ecx+0xd8]=0; jmp CVtable_012B5C10::Initialise 0x00C596E0 (chunk 586: the
// voice base's slot 1, ?Initialise@CVtable_012B5C10@@UAEJPAVCAudioObject@@@Z). Descriptive class names (libpk has no PDB / RTTI).

class CAudioObject;
struct CVtable_012B5C10 {                                // the libpk voice base, vtable 0x012B5C10 (no RTTI)
    virtual ~CVtable_012B5C10();                         // slot 0
    virtual long Initialise(CAudioObject* sound);         // slot 1 (0x00C596E0)
};
struct CVtable_012B5C60 : CVtable_012B5C10 {             // derived voice, vtable 0x012B5C60
    char pad_04[0xd8 - 4];
    unsigned int f_d8;      // +0xd8
    char pad2[0x1e8 - 0xd8 - 4];
    unsigned int f_1e8;     // +0x1e8
    unsigned int f_1ec;     // +0x1ec
    virtual long Initialise(CAudioObject* sound);         // slot 1 (0x00C5A1D0)
};

long CVtable_012B5C60::Initialise(CAudioObject* sound)
{
    f_1e8 = 0x1000;
    f_1ec = 0;
    f_d8 = 0;
    return CVtable_012B5C10::Initialise(sound);
}