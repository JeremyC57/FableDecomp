// SNamedTextEntry_009a9ad0::SNamedTextEntry_009a9ad0(const SNamedTextEntry_009a9ad0& other) -- copy constructor of a { CCharString key; { four CWideStrings, a word } value } pair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A9AD0, length 79 bytes.
// Leaf: ??0SNamedTextEntry_009a9ad0
// The chunk label (CSpecialAbilitiesDrainLifeDef::operator=) is propagated garbage: every callee is a COPY
// CONSTRUCTOR, so this builds a new object; named by address.
// Callees: 0x0099EC30 (CCharString::CCharString(const CCharString&), on +0), 0x0099B720
// (CWideString::CWideString(const CWideString&), four times on +4..+0x10).
// Shape: the key copied on `this`, then the value's address formed once on both sides (`lea edi,[ebx+4]`,
// `add esi,4`) and its four strings and trailing word copied through those re-based pointers: the value is a
// nested record whose own memberwise copy constructor is inlined (a map node's pair<key, value> shape), the
// word moved through eax last; `this` returned from ebx, `ret 4`.
class CCharString {
public:
    void* m_pData;
    CCharString(const CCharString& other);      // 0x0099EC30
};

class CWideString {
public:
    void* m_pData;
    CWideString(const CWideString& other);      // 0x0099B720
};

struct SLocalisedText_009a9ad0 {                // 0x14 bytes
    CWideString m_text0;                        // +0x00
    CWideString m_text1;                        // +0x04
    CWideString m_text2;                        // +0x08
    CWideString m_text3;                        // +0x0c
    long        m_value;                        // +0x10
    SLocalisedText_009a9ad0(const SLocalisedText_009a9ad0& other)
        : m_text0(other.m_text0), m_text1(other.m_text1), m_text2(other.m_text2), m_text3(other.m_text3),
          m_value(other.m_value) { }
};

struct SNamedTextEntry_009a9ad0 {
    CCharString             first;              // +0x00
    SLocalisedText_009a9ad0 second;             // +0x04
    SNamedTextEntry_009a9ad0(const SNamedTextEntry_009a9ad0& other);
};

SNamedTextEntry_009a9ad0::SNamedTextEntry_009a9ad0(const SNamedTextEntry_009a9ad0& other)
    : first(other.first), second(other.second)
{
}