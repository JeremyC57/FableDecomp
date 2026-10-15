// CSpecialAbilitiesDrainLifeDef::CSpecialAbilitiesDrainLifeDef() -- base CParentDefClassBase() (0x00430370), this class's vtable, then the four retail vectors +0x3c..+0x68 built empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004578DE, length 56 bytes.
// Leaf: ??0CSpecialAbilitiesDrainLifeDef
// The chunk label (CSpecialAbilitiesBattleChargeDef) is propagated: the vtable stored here, 0x01234404, is
// CSpecialAbilitiesDrainLifeDef (rtti-vtables.tsv; its slot 20, GetSizeofClass, is 0x00457916, right
// behind this body). Callees: 0x00430370 (the shared def root constructor). Layout from the PDB (Ego_r:
// four vectors from +0x40 and SpellDelaySecs; 132 bytes) shifted to the retail build (def root ends at
// +0x3c, 12-byte retail vectors): vectors +0x3c/+0x48/+0x54/+0x60, SpellDelaySecs +0x6c.
// Shape: `xor eax,eax` before the vtable store, the vtable store before twelve `mov [esi+N],eax`: each
// vector is a member object with an inline constructor; the float is left uninitialised.
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

class CCharString;

struct CSpecialAbilitiesDrainLifeDef : CParentDefClassBase {
    CRetailVector<float>       AmountOfHealthSucked;    // +0x3c
    CRetailVector<long>        NumEffects;              // +0x48
    CRetailVector<float>       SpellRange;              // +0x54
    CRetailVector<CCharString> ShotName;                // +0x60
    float                      SpellDelaySecs;          // +0x6c
    virtual void v00();
    CSpecialAbilitiesDrainLifeDef();
};

CSpecialAbilitiesDrainLifeDef::CSpecialAbilitiesDrainLifeDef()
    : CParentDefClassBase()
{
}
