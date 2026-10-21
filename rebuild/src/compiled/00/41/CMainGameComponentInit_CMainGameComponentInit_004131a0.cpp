// CMainGameComponentInit::CMainGameComponentInit() -- default-construct the world, holy-site, quest and save names
// Retail: Fable.exe @ 0x004131A0, 36 bytes; Ego_r.pdb ??0CMainGameComponentInit@@QAE@XZ. Member ctors in declaration
// order: CWideString() 0x0099AED0 on +0x00 and +0x04, CCharString() 0x0099E4B0 on +0x08, CWideString() on +0x0C.
// Layout: Ego_r.pdb CMainGameComponentInit (16 bytes; no engine/ header exists).
class CWideString {
public:
    CWideString();                                       // 0x0099AED0
    void* PData;
};
class CCharString {
public:
    CCharString();                                       // 0x0099E4B0
    void* PData;
};
class CMainGameComponentInit {
public:
    CMainGameComponentInit();
    CWideString InitialWorldName;                        // +0x00
    CWideString InitialWorldHolySiteName;                // +0x04
    CCharString InitialQuestName;                        // +0x08
    CWideString SaveGameName;                            // +0x0c
};
CMainGameComponentInit::CMainGameComponentInit()
{
}