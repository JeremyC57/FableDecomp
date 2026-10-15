// CObjectAugmentationSoundSet::CObjectAugmentationSoundSet(const CObjectAugmentationSoundSet& other) -- one-argument constructor that ignores its argument: not yet played, empty sound name, no sound handle (-1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007688E0, length 34 bytes (`ret 4`). Callee: CCharString::CCharString(const
// char*, long) @ 0x0099EBF0 with the shared empty literal @ 0x0122D70E.
// The body is byte-identical to the default constructor @ 0x00765CC0 except for `ret 4`: one stack argument that is
// never read. Modelled as a copy constructor that deliberately does not copy (a playing sound handle must not be
// shared between two sets); no direct caller exists to confirm the argument's type, so this is the best reading.
// Layout from the PDB: Played +0x00 (bool), Sound +0x04 (CCharString), Handle +0x08.
// Leaf: ??0CObjectAugmentationSoundSet
class CCharString {
public:
    CCharString(const char* text, long length);   // 0x0099EBF0
    void* m_pStringData;
};

struct CObjectAugmentationSoundSet {
    bool          Played;    // +0x00
    CCharString   Sound;     // +0x04
    unsigned long Handle;    // +0x08

    CObjectAugmentationSoundSet(const CObjectAugmentationSoundSet& other);
};

CObjectAugmentationSoundSet::CObjectAugmentationSoundSet(const CObjectAugmentationSoundSet& /*other*/)
    : Played(false), Sound("", -1), Handle(0xFFFFFFFF)
{
}