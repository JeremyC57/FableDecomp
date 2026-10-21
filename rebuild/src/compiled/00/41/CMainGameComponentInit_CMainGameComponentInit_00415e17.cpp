// CMainGameComponentInit::CMainGameComponentInit(const CMainGameComponentInit& other) -- member-wise copy of the four start-up names
// Leaf: ??0CMainGameComponentInit@@QAE@ABV0@@Z
// Retail: Fable.exe @ 0x00415E17, 55 bytes, `ret 4`, returns this; one caller (0x00418EBB). Ego_r.pdb
// ??0CMainGameComponentInit@@QAE@ABV0@@Z. The implicit copy constructor: CWideString copy constructor (0x0099B720) for
// InitialWorldName (+0) and InitialWorldHolySiteName (+4), CCharString copy constructor (0x0099EC30) for
// InitialQuestName (+8), CWideString again for SaveGameName (+0xc).
// Layout: Ego_r.pdb CMainGameComponentInit (16 bytes, names above); no engine/ header.
// Opt: size TU (unaligned entry); the implicit copy constructor is emitted out of line through CopyMainGameComponentInit.
#pragma optimize("s", on)
#include <new>
class CWideString {
public:
    void* PData;
    CWideString(const CWideString& s);                   // 0x0099B720
};
class CCharString {
public:
    void* PData;
    CCharString(const CCharString& s);                   // 0x0099EC30
};
class CMainGameComponentInit {
public:
    CWideString InitialWorldName;                        // +0x00
    CWideString InitialWorldHolySiteName;                // +0x04
    CCharString InitialQuestName;                        // +0x08
    CWideString SaveGameName;                            // +0x0c
};
void CopyMainGameComponentInit(void* p, const CMainGameComponentInit& init)   // emission helper, not a retail function
{
    new (p) CMainGameComponentInit(init);
}