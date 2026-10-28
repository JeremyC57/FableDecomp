// CStringTable_009a8150::SetEntry(const CCharString& value, const CCharString& key) -- `m_entries[key] = value` on the map<CCharString, CCharString> at +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A8150, length 28 bytes (32 with padding).
// Leaf: SetEntry
// The chunk label (CSpellContainerList::AddChild) is propagated garbage; named by address.
// Callees: 0x009AC700 (map<CCharString, CCharString>::operator[], thiscall, the key; returns the mapped
// string), 0x0099EFB0 (CCharString::operator=(const CCharString&)).
// Shape: both arguments loaded, the VALUE pushed first (it is the argument of the outer operator= call,
// evaluated before its object expression) and left on the stack across the operator[] call, which takes the
// KEY; then `mov ecx,eax ; call operator=` and `ret 8` (no tail jump: the pending argument is in the way).
// So the first parameter is the value and the second the key.
class CCharString {
public:
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CStringMap_009a8150 {                    // map<CCharString, CCharString>
    void* m_pHead;
    long  m_count;
    CCharString& operator[](const CCharString& key);    // 0x009AC700
};

struct CStringTable_009a8150 {
    unsigned char       _00[0x18];
    CStringMap_009a8150 m_entries;              // +0x18
    void SetEntry(const CCharString& value, const CCharString& key);
};

void CStringTable_009a8150::SetEntry(const CCharString& value, const CCharString& key)
{
    m_entries[key] = value;
}