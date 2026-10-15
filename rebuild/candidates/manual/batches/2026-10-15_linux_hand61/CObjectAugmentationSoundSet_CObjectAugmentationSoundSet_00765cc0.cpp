// CObjectAugmentationSoundSet::CObjectAugmentationSoundSet() -- default constructor: not yet played, empty sound name, no sound handle (-1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00765CC0, length 32 bytes. Callee: CCharString::CCharString(const char*, long) @ 0x0099EBF0
// with the shared empty literal @ 0x0122D70E.
// Layout from the PDB (CObjectAugmentationSoundSet, 12 bytes): Played +0x00 (bool), Sound +0x04 (CCharString),
// Handle +0x08. Shape: the initialiser list in declaration order -- the CCharString arguments are pushed first, the
// Played byte is stored between the pushes and the member constructor call, Handle = -1 follows the call.
// Several byte-identical copies exist (0x00765CC0, 0x00768FA0, and the one-argument twin 0x007688E0): the inline constructor emitted out of line per module.
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

    CObjectAugmentationSoundSet();
};

CObjectAugmentationSoundSet::CObjectAugmentationSoundSet()
    : Played(false), Sound("", -1), Handle(0xFFFFFFFF)
{
}
