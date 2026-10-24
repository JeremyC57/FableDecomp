// std::_Construct(CConversationLine*, const CConversationLine&) -- placement copy of a conversation line: speaker, listener, the line string and the subtitle flag (catalog label CBodyPartMesh is propagated; the 16-byte layout is CConversationLine's)
// Retail: Fable.exe @ 0x006E6570, 41 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N006e6570 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CConversationLine {                       // PDB sizeof 0x10
    long        PersonSpeaking;                  // +0x00
    long        PersonSpokenTo;                  // +0x04
    CCharString LineToSay;                       // +0x08
    bool        ShowSubtitle;                    // +0x0C
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CConversationLine* p, const CConversationLine& value)
{
    if (p != 0)
        new (p) CConversationLine(value);
}
}