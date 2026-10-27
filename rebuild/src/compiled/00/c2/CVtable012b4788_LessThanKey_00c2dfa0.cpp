// CVtable_012b4788::LessThanKey(const CString& key, const CUnprovenItem_012b4788* b) const -- whether key sorts before item b's name
// Leaf: ?LessThanKey@CVtable_012b4788@@UBEHABVCString@@PBUCUnprovenItem_012b4788@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2DFA0, thiscall (this unused), `ret 8`, int result widened from a bool.
// Identity (descriptive): slot 3 of the twin non-RTTI comparator vtables 0x012B4788 / 0x012B47D0 (slot 4 the scalar
//   deleting destructor), the libpk container comparator family (Equal / Less / EqualToKey / LessThanKey) of the
//   landed CVtable_012b28d8 rows, here ordering named items by the libpk MFC-style CString they keep at +8.
//   libpk has no PDB or RTTI, so the class is named by its vtable and the item type by address.
// Callees: CString::Compare(const char*) const @ 0x00C0BCE0 (bound ?Compare@CString@@QBEHPBD@Z by the landed
//   CNamedSet::Find 0x00C101D0), reached through MFC's inline comparison operators and `operator LPCTSTR`.
// Shape: MFC's `operator==` / `operator<` return bool (`neg ; sbb al,al ; inc al` / `test ; setl cl`), widened to
//   the int result with `movzx`.
class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;                            // +0x00
    int Compare(const char* lpsz) const;        // 0x00C0BCE0
    operator const char*() const { return m_pchData; }
};

inline bool operator==(const CString& s1, const CString& s2)
{
    return s1.Compare(s2) == 0;
}
inline bool operator<(const CString& s1, const CString& s2)
{
    return s1.Compare(s2) < 0;
}

struct CUnprovenItem_012b4788 {
    unsigned char _pad00[0x08];
    CString       m_name;                       // +0x08 the sort key
};

class CVtable_012b4788 {                        // comparator; this unused (vtables 0x012B4788 / 0x012B47D0)
public:
    virtual int Equal(const CUnprovenItem_012b4788* a, const CUnprovenItem_012b4788* b) const;   // slot 0
    virtual int Less(const CUnprovenItem_012b4788* a, const CUnprovenItem_012b4788* b) const;    // slot 1
    virtual int EqualToKey(const CString& key, const CUnprovenItem_012b4788* b) const;          // slot 2
    virtual int LessThanKey(const CString& key, const CUnprovenItem_012b4788* b) const;         // slot 3
};

int CVtable_012b4788::LessThanKey(const CString& key, const CUnprovenItem_012b4788* b) const
{
    return key < b->m_name;
}