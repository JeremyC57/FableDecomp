// CWorld::DeleteCheckPointSave() -- delete the check-point auto-save: when the profile's check-point save path exists, delete it through the drive manager, then delete its ".hs" companion file when that exists too (static)
// Leaf: DeleteCheckPointSave
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049E740, 114 bytes, plain `ret` (static __fastcall, no arguments).
//   PDB name ?DeleteCheckPointSave@CWorld@@SIXXZ.
// Callees: 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get (static, returns the manager reference), 0x00408040
//   CUserProfileManager::GetAutoSaveCheckPointPathName() const (hidden CWideString result pushed first), 0x00999230
//   CAFile::PathExists(const CWideString&) (static __fastcall), 0x009E4BC0 CDriveManager::DeleteFile(const CWideString&)
//   (__fastcall, result dropped), 0x0099BF30 operator+(const CWideString&, const wchar_t*) (__fastcall, hidden result in
//   ecx), 0x0099B510 ~CWideString. String L".hs" 0x01236A74. Global 0x013CAA28 the system drive manager counted
//   pointer (object word +0, as the landed CSystemManager::Uninitialise 0x009A60D0 models it).
// Shape: two named CWideString locals (the path, then path + L".hs" in the inner scope, destroyed before the path);
//   the counted pointer's object is read per call (`mov ecx,[13CAA28h]`), the path argument pushed around it.
// Label correction: catalogued as `_global::DeleteCheckPointSave`; the PDB places it on CWorld (static).
#include "engine/CWorld.h"

typedef unsigned short wchar16;                    // wchar_t under the era defaults
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);       // 0x0099BF30

struct CAFile {
    static bool __fastcall PathExists(const CWideString& path);                        // 0x00999230
};

struct CUserProfileManager {
    CWideString GetAutoSaveCheckPointPathName() const;                                 // 0x00408040
};

template <class T>
struct CTBaseSingleton {
    static T& __fastcall Get();                                                        // 0x0040D2A0 (CUserProfileManager)
};

enum EDriveReturn { DRIVE_OK };
struct CDriveManager {
    EDriveReturn DeleteFile(const CWideString& path);                                  // 0x009E4BC0
};

template <class T>
struct CCountedPointer {
    T*    Data;                                    // +0
    void* Info;                                    // +4 count block
    T* operator->() const { return Data; }
};

extern CCountedPointer<CDriveManager> g_DriveManager;   // 0x013CAA28

struct CWorld_Methods : CWorld {
    static void __fastcall DeleteCheckPointSave();
};

void __fastcall CWorld_Methods::DeleteCheckPointSave()
{
    CWideString path = CTBaseSingleton<CUserProfileManager>::Get().GetAutoSaveCheckPointPathName();
    if (CAFile::PathExists(path)) {
        g_DriveManager->DeleteFile(path);
        CWideString hsPath = path + (const wchar16*)L".hs";
        if (CAFile::PathExists(hsPath))
            g_DriveManager->DeleteFile(hsPath);
    }
}