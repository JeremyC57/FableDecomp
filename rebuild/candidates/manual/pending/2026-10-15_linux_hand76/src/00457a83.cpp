// CSpecialAbilitiesTurncoatSpellDef::CSpecialAbilitiesTurncoatSpellDef() -- base CParentDefClassBase() (0x00430370), this class's vtable, then the five retail float vectors +0x3c..+0x74 built empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00457A83, length 65 bytes.
// Leaf: ??0CSpecialAbilitiesTurncoatSpellDef
// The chunk label (CSoundDef::CSoundDef) is propagated from the byte-identical CSoundDef constructor
// 0x00458413: the vtable stored here, 0x0123453C, is CSpecialAbilitiesTurncoatSpellDef (rtti-vtables.tsv;
// its slot 20, GetSizeofClass, is 0x00457AC4, right behind this body).
// Callees: 0x00430370 (the shared def root constructor). Layout from the PDB (Ego_r: five vector<float>
// from +0x40, 144 bytes) shifted to the retail build (def root ends at +0x3c, 12-byte retail vectors).
// Shape: `xor eax,eax` before the vtable store, the vtable store before fifteen `mov [esi+N],eax`: each
// vector is a member object with an inline constructor.
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _base[0x38];                  // +0x04..+0x3b: the def root's own members
    CParentDefClassBase();                      // 0x00430370
};

template <class T>
struct CRetailVector {                          // VC6-era Dinkumware vector: first / last / end
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) { }
};

struct CSpecialAbilitiesTurncoatSpellDef : CParentDefClassBase {
    CRetailVector<float> StaminaCostPerSec;         // +0x3c
    CRetailVector<float> ResistanceDamagePerSec;    // +0x48
    CRetailVector<float> TurncoatTime;              // +0x54
    CRetailVector<float> TurncoatRange;             // +0x60
    CRetailVector<float> TurncoatXPUpEveryNSecs;    // +0x6c
    virtual void v00();
    CSpecialAbilitiesTurncoatSpellDef();
};

CSpecialAbilitiesTurncoatSpellDef::CSpecialAbilitiesTurncoatSpellDef()
    : CParentDefClassBase()
{
}
