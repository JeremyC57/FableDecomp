// _global::_Cons_val(SKeyedNamedValue*, const SKeyedNamedValue&) -- STL construct helper: placement-copy-construct a 12-byte (long, CCharString, long) record into raw container storage
// Flags: /Gr
// Leaf: _Cons_val
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00786d60, 35 bytes.
// Callee: CCharString copy ctor 0x0099EC30.
// Shape: link-time __fastcall (/Gr STL TU): destination in ecx (esi), source in edx (edi); the
// placement new's null test guards the inlined IMPLICIT copy constructor: the leading long (stored
// while the string's argument is set up), the out-of-line string copy, the trailing long.
// Label correction: the propagated `_Cons_val<allocator<CNavRegion>,..>` cannot be right -- Ego_r.pdb
// CNavRegion is 0x14 bytes (RegionID, a 12-byte list of connections, MostRecentSearchID) with no
// string; this record is {long +0, CCharString +4, long +8}.  No direct callers were found
// (xref.py), so the record type stays descriptive.
inline void* operator new(unsigned int, void* where) { return where; }

namespace FableCons_00786d60 {

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

struct SKeyedNamedValue {
    long        m_key;                                  // +0x00
    CCharString m_name;                                 // +0x04
    long        m_value;                                // +0x08
};

void __fastcall _Cons_val(SKeyedNamedValue* ptr, const SKeyedNamedValue& value)
{
    new ((void*)ptr) SKeyedNamedValue(value);
}

}