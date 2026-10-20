// CTCBetting::GetBetText() -- while betting is active, the betting def's "Bet" text followed by the current bet amount; otherwise an empty string
// Leaf: GetBetText
// Grade: DIFFER(207v177) -- retail keeps the def out pointer in esi (zeroed through it, reloaded only on the GetSubDef miss) and gives each miss its own path; the lane stores an immediate 0, merges both misses into one reload and colours the result pointer esi instead of edi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E50C0, 175 bytes (`ret 4`: the CWideString result's hidden pointer).
// Label: the catalog says CTCSinglePersonOccupiable::IsOfType (propagated); the body sits between CTCBetting members
// (0x007E4E30 FrameUpdate, 0x007E51D0), is called from CTCBetting::DrawPreparePrimitivesForRendering (0x007E5704), reads the
// betting globals and the 'CBettingDef' sub def -- named GetBetText here (the PDB name is not known).
// Callees: CDefClassBase::GetSubDef_007e5820(CDefPointer&) @ 0x007E5820 (the 'CBettingDef' instantiation), IntToWideString(long)
// @ 0x00405F00 (__fastcall, hidden result in ecx), CTCBetting::GetText(unsigned long) @ 0x007E4D70, operator+(const CWideString&,
// const CWideString&) @ 0x0099BE70 (__fastcall), ~CWideString @ 0x0099B510, CWideString(const wchar_t*) @ 0x0099B6B0 (L"" @
// 0x0122D70C); globals g_bettingActive @ 0x013BAE1D, g_bettingAmount @ 0x013BAE20 (the landed GetBettingActive/Amount spellings).
// Layout: CThing PDef +0x70; CBettingDef (def at Ego_r.pdb offsets) Bet text id +0x44.
// Shape: nested tests (the two misses reach the empty-string path separately), then `return GetText(id) + IntToWideString(amount);` built straight into the hidden result (the right
// operand first); the def out pointer starts in the zero register, is released without a null test on the found path
// (already dereferenced) and with one on the empty-string path.
#include <stddef.h>
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall IntToWideString(long value);     // 0x00405F00
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
extern bool g_bettingActive;                            // 0x013BAE1D
extern long g_bettingAmount;                            // 0x013BAE20
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CBettingDef : CDefBase {
    unsigned char _pad08[0x44 - 0x08];
    unsigned long Bet;                                  // +0x44 text id
};
struct CDefClassBase : CDefBase {
    bool GetSubDef_007e5820(CDefPointer<CBettingDef>& out);   // 0x007E5820
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
struct CTCBetting {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CWideString GetText(unsigned long textId);          // 0x007E4D70
    CWideString GetBetText();
};

CWideString CTCBetting::GetBetText()
{
    CDefPointer<CBettingDef> bettingDef;
    if (g_bettingActive) {
        if (ParentThing->PDef->GetSubDef_007e5820(bettingDef))
            return GetText(bettingDef->Bet) + IntToWideString(g_bettingAmount);
    }
    return CWideString(L"");
}
