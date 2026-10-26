// CSpecialAbilitiesMultiArrowDef::CSpecialAbilitiesMultiArrowDef() -- base CParentDefClassBase(), own vtable, CastSound def-string = -1, the two vectors empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045725B, length 42 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370 (vtable 0x01230CBC, rtti-vtables.tsv).
// Vtable 0x01234194 = CSpecialAbilitiesMultiArrowDef; the manifest's "CCombatAbilityStrafeDef" label is
// propagated. Retail layout = Ego_r.pdb minus 4 (the base's CVectorMap is 4 bytes smaller, so the
// packed base is 0x39 bytes and the first long member lands at +0x3c): NumberOfShots[4] +0x3c,
// NumberInVolley[4] +0x4c, AmmoEffectCreationDelaySecs +0x5c, CastSound (CDefString) +0x60,
// BowHighlightWidth +0x64, BowHighlightColor (vector<long>, 12 bytes) +0x68,
// MultiArrowPerArrowDamageMultiplier (vector<float>) +0x74; size 0x80. Only the def string and the
// two vectors have constructors; the plain floats/longs stay uninitialised. The engine header for
// CParentDefClassBase is a flat POD, hence the `_Calls` model. `or dword ptr [esi+60h],-1` is the
// size-optimised -1 store, so the TU carries `#pragma optimize("s", on)`.
#pragma optimize("s", on)
#pragma pack(push, 1)
struct CParentDefClassBase_Calls {                          // retail 0x39 bytes (PDB 0x3d)
    virtual void v00();
    unsigned char _body[0x35];
    CParentDefClassBase_Calls();                            // 0x00430370
};
#pragma pack(pop)

class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};

template<class T> struct CRetailVector {                    // retail std::vector: {first, last, end}
    T* first; T* last; T* end_of_storage;
    CRetailVector() : first(0), last(0), end_of_storage(0) { }
};

class CSpecialAbilitiesMultiArrowDef : public CParentDefClassBase_Calls { public:
    long  NumberOfShots[4];                                 // +0x3c
    long  NumberInVolley[4];                                // +0x4c
    float AmmoEffectCreationDelaySecs;                      // +0x5c
    CDefString CastSound;                                   // +0x60
    float BowHighlightWidth;                                // +0x64
    CRetailVector<long>  BowHighlightColor;                 // +0x68
    CRetailVector<float> MultiArrowPerArrowDamageMultiplier;// +0x74
    virtual void v00();                                     // own vtable 0x01234194
    CSpecialAbilitiesMultiArrowDef();
};

CSpecialAbilitiesMultiArrowDef::CSpecialAbilitiesMultiArrowDef()
    : CParentDefClassBase_Calls()
    , CastSound()
    , BowHighlightColor()
    , MultiArrowPerArrowDamageMultiplier()
{
}