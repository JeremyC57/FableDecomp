// CVtable_012b3840::Visit(CUnprovenEntry_012b3840* entry) -- remember the visited entry (+4) when its key word (+0x20) equals the searched key (+8)
// Leaf: ?Visit@CVtable_012b3840@@UAEXPAUCUnprovenEntry_012b3840@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C22D90, 18 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 0 of the two-slot non-RTTI visitor vtable 0x012B3840 (slot 1 its scalar deleting
//   destructor 0x00C242B0). The neighbouring visitor vtables 0x012B3840 / 0x012B3848 differ only in the key word's offset
//   (+0x20 / +0x40) of the entries they walk: a find-by-key functor handed to a libpk container's visit loop. libpk has
//   no PDB or RTTI, so the functor is named by its vtable and the entry type by address.
// Shape: the compare against the key member, then the plain store of the argument.
struct CUnprovenEntry_012b3840 {
    unsigned char _pad00[0x20];
    long          m_key;                        // +0x20
};

class CVtable_012b3840 {                        // vtable 0x012B3840: { Visit, scalar deleting destructor }
public:
    virtual void Visit(CUnprovenEntry_012b3840* entry);
    CUnprovenEntry_012b3840* m_pFound;          // +0x04
    long               m_key;             // +0x08
};

void CVtable_012b3840::Visit(CUnprovenEntry_012b3840* entry)
{
    if (entry->m_key == m_key)
        m_pFound = entry;
}