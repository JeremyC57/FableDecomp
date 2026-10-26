// CUnprovenKeyedList_00c2c250::IsLess(CUnprovenItem_00c2c250* a, CUnprovenItem_00c2c250* b) const -- libpk keyed-list comparator: ordering of the two items' keys (the key member at +0x8), through the key's own operator<
// Leaf: ?IsLess@CUnprovenKeyedList_00c2c250@@UBEHPAVCUnprovenItem_00c2c250@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2C270, 25 bytes (+ int3 padding to 32), thiscall (`this` unused), `ret 8`.
// Identity (descriptive): CUnprovenKeyedList_00c2c250::IsLess(CUnprovenItem_00c2c250* a, CUnprovenItem_00c2c250* b) const; evidence: entry 1 of the comparator group {0x00C2C250 equal, 0x00C2C270 less, 0x00C2C290 key-equal, 0x00C2C2B0 key-less} referenced from .rdata at 0x012B4510 and 0x012B45A0 (two list vtables, each group followed by its own deleting destructor 0x00C2C2D0 / 0x00C2C460); the equal pair calls 0x00C38360, the less pair 0x00C38380; the body passes the key at +0x8
// Label: the manifest's "sub_00c2c270" is a gap-scan placeholder (families J030 / J031: the item-item and key-item comparators of
//   three libpk keyed lists whose key is a member object compared by out-of-line operators).
// Callees: operator<(const CUnprovenKey_00c2c250&, const CUnprovenKey_00c2c250&) 0x00C38380 (`__fastcall`-shaped: the two key addresses in ecx / edx, a bool in al).
// Naming: libpk (no PDB, no RTTI; trace strings name PKCTRList.h) -- the list, item and key classes and the member names are
//   descriptive CUnproven*_<addr> stand-ins keyed by the group's first comparator; the vtable order of the comparator group
//   (equal, less, key equal, key less) is the J029 one. The key compare helpers are register-argument (`__fastcall`) here, the
//   form libpk's LTCG build gives them.
// Shape: `return a->Key < b->Key;` -- the key at +0x8 of the item, the bool result widened to the int return (`movzx eax,al`).
class CUnprovenKey_00c2c250 {                       // descriptive: 8-byte key; 0x00C38360 / 0x00C38380 compare +0x04 then +0x00 (unsigned)
public:
    unsigned long Data[2];                  // +0x00, +0x04 (compared high word first)
};
bool __fastcall operator==(const CUnprovenKey_00c2c250& a, const CUnprovenKey_00c2c250& b);   // 0x00C38360
bool __fastcall operator<(const CUnprovenKey_00c2c250& a, const CUnprovenKey_00c2c250& b);    // 0x00C38380

class CUnprovenItem_00c2c250 {
public:
    unsigned char _pad00[0x8];                // descriptive
    CUnprovenKey_00c2c250 Key;                       // +0x8
};

class CUnprovenKeyedList_00c2c250 {
public:
    virtual int IsEqual(CUnprovenItem_00c2c250* a, CUnprovenItem_00c2c250* b) const;                       // 0x00C2C250
    virtual int IsLess(CUnprovenItem_00c2c250* a, CUnprovenItem_00c2c250* b) const;                        // 0x00C2C270
    virtual int IsKeyEqual(const CUnprovenKey_00c2c250* key, CUnprovenItem_00c2c250* item) const;      // 0x00C2C290
    virtual int IsKeyLess(const CUnprovenKey_00c2c250* key, CUnprovenItem_00c2c250* item) const;       // 0x00C2C2B0
};

int CUnprovenKeyedList_00c2c250::IsLess(CUnprovenItem_00c2c250* a, CUnprovenItem_00c2c250* b) const
{
    return a->Key < b->Key;
}