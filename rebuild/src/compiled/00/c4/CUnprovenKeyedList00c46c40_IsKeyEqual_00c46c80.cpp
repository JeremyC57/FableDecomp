// CUnprovenKeyedList_00c46c40::IsKeyEqual(const CUnprovenKey_00c46c40* key, CUnprovenItem_00c46c40* item) const -- libpk keyed-list comparator: equality of the probe key and the item's key (at +0x4), through the key's own operator==
// Leaf: ?IsKeyEqual@CUnprovenKeyedList_00c46c40@@UBEHPBVCUnprovenKey_00c46c40@@PAVCUnprovenItem_00c46c40@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C46C80, 22 bytes (+ int3 padding to 32), thiscall (`this` unused), `ret 8`.
// Identity (descriptive): CUnprovenKeyedList_00c46c40::IsKeyEqual(const CUnprovenKey_00c46c40* key, CUnprovenItem_00c46c40* item) const; evidence: entry 2 of the comparator group {0x00C46C40 equal, 0x00C46C60 less, 0x00C46C80 key-equal, 0x00C46CA0 key-less} referenced from .rdata at 0x012B7DB0 and 0x012B7DF8 (two list vtables, each group followed by its own deleting destructor 0x00C46CC0 / 0x00C46D80); the equal pair calls 0x00C466B0, the less pair 0x00C466C0; the body passes the key at +0x4
// Label: the manifest's "sub_00c46c80" is a gap-scan placeholder (families J030 / J031: the item-item and key-item comparators of
//   three libpk keyed lists whose key is a member object compared by out-of-line operators).
// Callees: operator==(const CUnprovenKey_00c46c40&, const CUnprovenKey_00c46c40&) 0x00C466B0 (`__fastcall`-shaped: the two key addresses in ecx / edx, a bool in al).
// Naming: libpk (no PDB, no RTTI; trace strings name PKCTRList.h) -- the list, item and key classes and the member names are
//   descriptive CUnproven*_<addr> stand-ins keyed by the group's first comparator; the vtable order of the comparator group
//   (equal, less, key equal, key less) is the J029 one. The key compare helpers are register-argument (`__fastcall`) here, the
//   form libpk's LTCG build gives them.
// Shape: `return *key == item->Key;` -- the key at +0x4 of the item, the bool result widened to the int return (`movzx eax,al`).
class CUnprovenKey_00c46c40 {                       // descriptive: 12-byte float key; 0x00C466B0 (jmp 0x00C46DC0) compares floats +0x00 / +0x04, 0x00C466C0 orders on +0x08 first
public:
    float Data[3];                          // three floats (operator== compares +0x00 / +0x04 through 0x00C46DC0)
};
bool __fastcall operator==(const CUnprovenKey_00c46c40& a, const CUnprovenKey_00c46c40& b);   // 0x00C466B0
bool __fastcall operator<(const CUnprovenKey_00c46c40& a, const CUnprovenKey_00c46c40& b);    // 0x00C466C0

class CUnprovenItem_00c46c40 {
public:
    unsigned char _pad00[0x4];                // descriptive
    CUnprovenKey_00c46c40 Key;                       // +0x4
};

class CUnprovenKeyedList_00c46c40 {
public:
    virtual int IsEqual(CUnprovenItem_00c46c40* a, CUnprovenItem_00c46c40* b) const;                       // 0x00C46C40
    virtual int IsLess(CUnprovenItem_00c46c40* a, CUnprovenItem_00c46c40* b) const;                        // 0x00C46C60
    virtual int IsKeyEqual(const CUnprovenKey_00c46c40* key, CUnprovenItem_00c46c40* item) const;      // 0x00C46C80
    virtual int IsKeyLess(const CUnprovenKey_00c46c40* key, CUnprovenItem_00c46c40* item) const;       // 0x00C46CA0
};

int CUnprovenKeyedList_00c46c40::IsKeyEqual(const CUnprovenKey_00c46c40* key, CUnprovenItem_00c46c40* item) const
{
    return *key == item->Key;
}