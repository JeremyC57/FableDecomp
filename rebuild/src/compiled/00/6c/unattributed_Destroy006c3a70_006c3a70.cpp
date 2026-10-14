// Destroy_006c3a70(CNamedEntryList*) -- the counted-pointer deleter for a {CCharString name, array of
// 0x14-byte entries that start with a CCharString} object: destroy every entry (stride 0x14), free the array
// storage, destroy the name and operator delete the object; null is ignored. The pointer arrives in ecx.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C3A70, length 69 bytes.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (per entry and for the name), free @ 0x00BFEA14,
// operator delete @ 0x00BFE9BC. The manifest label (CChickenSign::~CChickenSign) does not fit: the PDB's
// CChickenSign holds a vector<CWideString> at +0x1c, this object is a CCharString at +0 followed by a
// retail 12-byte array {Begin +4, End +8, Capacity +0xc}; named by what it does.
#include <stdlib.h>
class CCharString {
public:
    void* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
struct SNamedEntry {                            // 0x14 bytes: the key string and 16 bytes of plain data
    CCharString   Key;                          // +0
    unsigned char Data[0x10];                   // +4
};
struct CRetailEntryArray {                      // retail CArray<SNamedEntry>, malloc-backed
    SNamedEntry* Begin;                         // +0
    SNamedEntry* End;                           // +4
    SNamedEntry* Capacity;                      // +8
    ~CRetailEntryArray()
    {
        SNamedEntry* first = Begin;
        SNamedEntry* last = End;
        for (; first != last; ++first)
            first->~SNamedEntry();
        if (Begin != 0)
            free(Begin);
    }
};
struct CNamedEntryList_006c3a70 {
    CCharString       Name;                     // +0x00
    CRetailEntryArray Entries;                  // +0x04
};

void __fastcall Destroy_006c3a70(CNamedEntryList_006c3a70* object)
{
    delete object;
}