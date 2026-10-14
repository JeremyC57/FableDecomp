// CEngine::SetSeaBodyToRender(CCharString name) -- hand the name (by value) to the sea-body manager's inline SetName, which copies it into its name member at +0x27c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23F00, length 57 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (the inlined SetName's by-value parameter, built in the 4-byte local), CCharString::operator=
// @ 0x0099EFB0 (on the member @ 0x01436E54 + 0x27c), ~CCharString @ 0x0099EAE0 twice: the inlined parameter's copy, then the by-value argument (callee-destroyed,
// `lea ecx,[esp+8]`). A non-static member whose `this` is unused with one by-value argument (`ret 4`).
struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CSeaBodyManager_01436e54 {
    unsigned char _pad[0x27c];
    CCharString   m_bodyName;                      // +0x27c
    void SetBodyName(CCharString name) { m_bodyName = name; }   // inlined: the by-value parameter becomes the local copy
};
extern CSeaBodyManager_01436e54* g_pSeaBodyManager_01436e54;   // 0x01436E54

struct CEngine {
    void SetSeaBodyToRender(CCharString name);   // a member (`ret 4` pops the by-value argument), `this` unused
};

void CEngine::SetSeaBodyToRender(CCharString name)
{
    g_pSeaBodyManager_01436e54->SetBodyName(name);
}