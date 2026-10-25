// NResourceDirectoryNames::GetDebugSaveDir() -- the debug save folder: forwards to the user profile manager singleton's GetDebugSaveDir
// Leaf: ?GetDebugSaveDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041A1A0, 20 bytes, a `__fastcall` free function: hidden result pointer in ecx, `ret`.
// Identity (label correction): the catalog's "pair<CCharString,CTCBase::CRegisteredTCInfo>" is propagated. FableWin.pdb
//   lists ?GetDebugSaveDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ between GetCacheDir and GetMeshBankDir, and
//   retail 0x0041A1A0 sits right before the landed GetMeshBankDir 0x0041A1C0 in the same run of directory getters.
//   The callee pair is ?Get@CUserProfileManager@@SIAAV1@XZ (0x0040D2A0, static __fastcall) and
//   ?GetDebugSaveDir@CUserProfileManager@@QBE?AVCWideString@@XZ (0x00406780: GetSaveDir() combined with L"Data\\").
// Callees: CUserProfileManager::Get @ 0x0040D2A0, CUserProfileManager::GetDebugSaveDir @ 0x00406780.
// Shape: `return Get().GetDebugSaveDir();` -- the caller's hidden result pointer is pushed straight through (VC7.1 RVO),
//   before the singleton call that supplies `this`.
class CWideString {
public:
    void* PData;                                        // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};

class CUserProfileManager {
public:
    static CUserProfileManager& __fastcall Get();      // 0x0040D2A0
    CWideString GetDebugSaveDir() const;               // 0x00406780
};

namespace NResourceDirectoryNames {
CWideString __fastcall GetDebugSaveDir()
{
    return CUserProfileManager::Get().GetDebugSaveDir();
}
}