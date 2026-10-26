// CUnprovenKeyedList_00c24760::IsKeyEqual(const CUnprovenKey_00c24760* key, CUnprovenItem_00c24760* item) const -- libpk keyed-list comparator: equality of the probe key and the item's key (at +0x18), through the key's own operator==
// Leaf: ?IsKeyEqual@CUnprovenKeyedList_00c24760@@UBEHPBVCUnprovenKey_00c24760@@PAVCUnprovenItem_00c24760@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C247A0, 22 bytes (+ int3 padding to 32), thiscall (`this` unused), `ret 8`.
// Identity (descriptive): CUnprovenKeyedList_00c24760::IsKeyEqual(const CUnprovenKey_00c24760* key, CUnprovenItem_00c24760* item) const; evidence: entry 2 of the comparator group {0x00C24760 equal, 0x00C24780 less, 0x00C247A0 key-equal, 0x00C247C0 key-less} referenced from .rdata at 0x012B3B10 and 0x012B3D18 (two list vtables, each group followed by its own deleting destructor 0x00C24C00 / 0x00C250D0); the equal pair calls 0x00C25CE0, the less pair 0x00C25D50; the body passes the key at +0x18
// Label: the manifest's "sub_00c247a0" is a gap-scan placeholder (families J030 / J031: the item-item and key-item comparators of
//   three libpk keyed lists whose key is a member object compared by out-of-line operators).
// Callees: operator==(const CUnprovenKey_00c24760&, const CUnprovenKey_00c24760&) 0x00C25CE0 (`__fastcall`-shaped: the two key addresses in ecx / edx, a bool in al).
// Naming: libpk (no PDB, no RTTI; trace strings name PKCTRList.h) -- the list, item and key classes and the member names are
//   descriptive CUnproven*_<addr> stand-ins keyed by the group's first comparator; the vtable order of the comparator group
//   (equal, less, key equal, key less) is the J029 one. The key compare helpers are register-argument (`__fastcall`) here, the
//   form libpk's LTCG build gives them.
// Shape: `return *key == item->Key;` -- the key at +0x18 of the item, the bool result widened to the int return (`movzx eax,al`).
class CUnprovenKey_00c24760 {                       // descriptive: 16-byte key; operator== / operator< 0x00C25CE0 / 0x00C25D50 compare +0x0C, +0x08, then +0x04
public:
    unsigned long Data[4];                  // +0x00..+0x0c (the operators compare +0x0c, +0x08, +0x04)
};
bool __fastcall operator==(const CUnprovenKey_00c24760& a, const CUnprovenKey_00c24760& b);   // 0x00C25CE0
bool __fastcall operator<(const CUnprovenKey_00c24760& a, const CUnprovenKey_00c24760& b);    // 0x00C25D50

class CUnprovenItem_00c24760 {
public:
    unsigned char _pad00[0x18];                // descriptive
    CUnprovenKey_00c24760 Key;                       // +0x18
};

class CUnprovenKeyedList_00c24760 {
public:
    virtual int IsEqual(CUnprovenItem_00c24760* a, CUnprovenItem_00c24760* b) const;                       // 0x00C24760
    virtual int IsLess(CUnprovenItem_00c24760* a, CUnprovenItem_00c24760* b) const;                        // 0x00C24780
    virtual int IsKeyEqual(const CUnprovenKey_00c24760* key, CUnprovenItem_00c24760* item) const;      // 0x00C247A0
    virtual int IsKeyLess(const CUnprovenKey_00c24760* key, CUnprovenItem_00c24760* item) const;       // 0x00C247C0
};

int CUnprovenKeyedList_00c24760::IsKeyEqual(const CUnprovenKey_00c24760* key, CUnprovenItem_00c24760* item) const
{
    return *key == item->Key;
}