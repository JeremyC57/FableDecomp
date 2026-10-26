// std::pair<CCharString,CSymbolMap::CSymbolInfo>::~pair() -- destroy a symbol-map entry: the info's entry name and group name, then the key string
// Leaf: ??1?$pair@VCCharString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B86B0, length 31 bytes, `jmp` tail; no call or address references (an
// unreferenced COMDAT; 0x004B86B0 and 0x006FB460 are the same destructor emitted in two TUs). The manifest label
// (_Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo>>>) names the right element type.
// Layout: Ego_r.pdb CSymbolMap::CSymbolInfo (0xc) = CCharString GroupName +0, CCharString EntryName +4, long Val +8; the pair
// is 0x10 (key +0, info +4).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (x3).
// Shape: the implicit pair destructor emitted through a qualified explicit destructor call; the inlined ~CSymbolInfo keeps
// `&second` in edi (EntryName at [edi+4], GroupName at [edi]), the key string is the tail `jmp`.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};

class CSymbolMap {
public:
    class CSymbolInfo { public:                                     // 0xc bytes
        CCharString GroupName;                               // +0x00
        CCharString EntryName;                               // +0x04
        long        Val;                                     // +0x08
    };
};

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

typedef std::pair<CCharString, CSymbolMap::CSymbolInfo> TSymbolEntry;

void DestroySymbolEntry(TSymbolEntry* entry)                 // emits the implicit destructor; not part of the retail body
{
    entry->TSymbolEntry::~TSymbolEntry();
}