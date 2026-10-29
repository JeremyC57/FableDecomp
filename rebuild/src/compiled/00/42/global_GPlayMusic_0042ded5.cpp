// GPlayMusic(float) -- start the front-end music stream: the misc-things def's FrontEndMusic file under the music directory, .wma renamed to .ogg
// Leaf: ?GPlayMusic@@YIXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042DED5, length 201 bytes, `ret 4` (__fastcall; the float travels on the stack).
// Name: FableWin.pdb / Ego_r.pdb hold exactly one `void __fastcall X(float)` free function, `?GPlayMusic@@YIXM@Z`; this body sits
// in the CNewFrontendGameComponent TU (InitialiseInput 0x0042DEC5 before it, Draw 0x0042DF9E after) and its three callers
// (0x0042F00A, 0x0042F07A, 0x0042F1FD) pass `fldz` without setting ecx. The catalog's "CFrontEnd_PlayAutoSaveSound" is a label.
// Callees: NUISystem::CManager::GetInstance @ 0x0041E5F2; CManager slot 42 (+0xA8) = GetMiscThingsDef @ 0x0041D8E7 (landed,
// returns CDefPointer<CUIMiscThingsDef> through a hidden pointer); the NResourceDirectoryNames L"Data\\Sound\\" getter @ 0x0041A3D0
// (one of five identical "Data\Sound\" getters 0x0041A2B0/390/3B0/3D0/3F0; by its TU position after the dialogue pair it is
// GetDVDMusicDir -- the catalog's "CUserProfileManager::GetAutoSaveFileName" is a propagated label);
// operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (__fastcall: result ecx, left edx, right on the stack);
// CWideString::CWideString(const wchar_t*) @ 0x0099B6B0, CWideString::Replace(const CWideString&, const CWideString&) @ 0x0099C1E0
// (Ego_r `?Replace@CWideString@@QAEXABV1@0@Z`), CWideString::~CWideString @ 0x0099B510; the sound manager's (0x013B8394,
// CSoundManagerLH vtable 0x0129D2C4) slot 17 (+0x44) = 0x00A3E4B0 (`ret 0x18`: file, two longs, two floats, a long).
// Strings: L".ogg" 0x01230C48, L".wma" 0x01230C3C.
// Pragmas: #pragma optimize("y", off) + #pragma optimize("s", on) -- the CNewFrontendGameComponent TU (frame pointer, as the
// landed Shutdown 0x0042EBB6).
// Shape: one full expression builds the name -- right-hand side first (the def pointer temporary [ebp-8], its +0x1CC member),
// then the directory temporary [ebp-0xC], then operator+ straight into the named local [ebp-4]; the temporaries die in reverse
// (the string, then the def pointer's inline RemoveReference: count at +4, slot 1 deletes). Replace takes two implicit
// const wchar_t* -> CWideString temporaries built right to left (".ogg" first). Layout: retail FrontEndMusic is at +0x1CC
// (Ego_r.pdb CUIMiscThingsDef has it at +0x1D4; no engine header exists, so a local model).
// Model: local -- engine/CManager.h and CIVCountedPointeeBase.h exist, but the generated header is data-only (pack(1), no member
// functions, no virtuals: the vptr and counted pointers are flattened into raw words); GetMiscThingsDef is a CManager virtual
// (slot 42) and the def pointer's release is an inline member, so both are local.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>                                      // wchar_t
class CWideString {
public:
    CWideString(const wchar_t* text);                    // 0x0099B6B0
    ~CWideString();                                      // 0x0099B510
    void Replace(const CWideString& from, const CWideString& to);   // 0x0099C1E0
    void* PStringData;
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70

namespace NResourceDirectoryNames {
CWideString __fastcall GetDVDMusicDir();                 // 0x0041A3D0
}

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                         // slot 1 (+0x04)
    unsigned long IVRefCount;                            // +0x04
    void RemoveReference() { if (--IVRefCount == 0) Delete(); }
};

template <class T> class CDefPointer {
public:
    CDefPointer(const CDefPointer& other);
    ~CDefPointer() { if (PObject) PObject->RemoveReference(); }
    T* operator->() const { return PObject; }
    T* PObject;
};

namespace NUISystem {
class CUIMiscThingsDef : public CIVCountedPointeeBase {
public:
    unsigned char _body[0x1cc - 8];
    CWideString   FrontEndMusic;                         // +0x1CC
};

class CManager {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual CDefPointer<CUIMiscThingsDef> GetMiscThingsDef();   // slot 42 (+0xA8) 0x0041D8E7
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2
};
}

class CSoundManagerLH {                                  // the object at 0x013B8394
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void PlayStream(const CWideString& fileName, long channel, long flags, float gain, float fade, long loops);   // slot 17 (+0x44) 0x00A3E4B0
};
extern CSoundManagerLH* g_pSoundSystem;                 // 0x013B8394

void __fastcall GPlayMusic(float fade)
{
    if (g_pSoundSystem) {
        CWideString fileName = NResourceDirectoryNames::GetDVDMusicDir()
                             + NUISystem::CManager::GetInstance()->GetMiscThingsDef()->FrontEndMusic;
        fileName.Replace(L".wma", L".ogg");
        g_pSoundSystem->PlayStream(fileName, 0, 0, 1.0f, fade, -1);
    }
}