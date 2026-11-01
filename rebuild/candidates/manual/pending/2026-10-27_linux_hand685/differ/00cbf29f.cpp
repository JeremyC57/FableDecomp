// CutsceneCameraPreloader(std::vector<CCharString>& lines, bool bSkipIfMacroPresent, long* pTimeCode) -- preload the camera points a cutscene script uses: unless a "DoCameraPreloading" line is present (then false), set the camera preload flag, run every UseCamera / CameraLookAt / CameraLookBetween / CameraFOVLookBetween line's first argument through CameraUseCameraPoint, clear the flag, zero the time code when no "StartTimeCode" line was seen, then wait frame by frame (blank progress display) until the current region is loaded and preloaded or the script runner is stopping; true
// Grade: DIFFER(930v936) -- instruction-identical except one register web: retail keeps the scaled line index (j*4) in edi across the StartTimeCode test and the || chain and materialises the "" address (0x0129AAF4) only inside the chain (ebx, both arms of its first diamond); VC7.1 here extends the "" web back into the StartTimeCode test (edi) and homes j*4 in a frame word (+4 frame, one store, two reloads). Next: this is an allocator-level split; probes 4-12 (second literal/accessor copies, extern global for "", named chain bool, line reference, inlined per-line helper, _M_start[i], /O1 /Oy-, == 0) all leave 930v936 -- likely retail LTCG; try the row again only with a lane that can reproduce link-time codegen, or a model where the first prefix test is compiled as a separate out-of-line static (not inlined) with a same-TU body.
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?CutsceneCameraPreloader@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBF29F, 936 bytes, `__fastcall` global (lines in ecx, the bool in dl, the long* on
//   the stack: `ret 4`), frame-pointer size TU (the cutscene macro helpers beside CutsceneMacro_ReadLightScene 0x00CBF050).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?CutsceneCameraPreloader@@YI_NAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@_NPAJ@Z;
//   the literals "DoCameraPreloading" 0x012C18C8, "StartTimeCode" 0x012C18B8, "UseCamera" 0x012C18AC, "CameraLookAt"
//   0x012C189C, "CameraLookBetween" 0x012C1888, "CameraFOVLookBetween" 0x012C1870 are the cutscene script commands; the
//   interface slots are CGameScriptInterfaceBase's (403 CameraSetCameraPreloadFlag, 412 CameraUseCameraPoint, 15
//   GetRegionName, 13 IsRegionLoadedAndPreloaded, 7 NewScriptFrame). The catalog label is right.
// Callees: CCharString(const char*, long) 0x0099EBF0, strncmp 0x00BFEAF8 (the inline prefix test of the landed 0x00E6E120:
//   "" = 0x0129AAF4 shares the null test of the text and the length), ~CCharString 0x0099EAE0, FindCharPos(char) 0x0099E5A0,
//   GetSubString(unsigned long, unsigned long) 0x0099EC70 (hidden result), RemoveWhiteSpaceBeginEnd 0x0099F560 (PDB
//   ?RemoveWhiteSpaceBeginEnd@CCharString@@QAEXXZ; catalogued as a propagated _Dest_val / "Trim": NAME_FIXES), the copy
//   constructor 0x0099EC30, NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) 0x009E9F40 (the ""
//   literal 0x0122D70E becomes a CCharString temporary homed in the dead pTimeCode argument slot), pDSTGame 0x0143E8F8
//   virtual slots 403 / 412 / 15 / 13 / 7, g_pScriptRunner 0x013D2838 (stop byte +5). -1.0f = 0x0122DEE0.
// Shape: the prefix test is an inline helper taking `const CCharString&` so each literal becomes a full-expression
//   temporary: the four-way `||` chain constructs them lazily and destroys them through VC7.1's temporary flags word
//   ([ebp-8], bits 1/2/4/8, reverse order); the time-code flag is a bool in [ebp-1]; the camera point name is
//   GetSubString(space + 1, comma or length) trimmed in place; the wait loop is `while (!IsRegionLoadedAndPreloaded(region))`
//   with the stop test between the progress display and the frame yield.
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int n);   // 0x00BFEAF8
class CCharStringData { public:                    // the shared string block
    char* m_pData;                                 // +0x00
    long  m_length;                                // +0x04
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }   // "" = 0x0129AAF4
    long Length() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    long FindCharPos(char c) const;                // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
    void RemoveWhiteSpaceBeginEnd();               // 0x0099F560
};
class C3DVector;
class CRightHandedSet;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport vector<CCharString>
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long i) { return *(_M_start + i); }
};
}
struct CScriptRunner {
    virtual void v00();
    virtual void v01();
    virtual void YieldFrame();                      // slot 2 (+8)
    unsigned char m_pad4;                           // +0x04
    bool          m_bStopRequested;                 // +0x05
};
extern CScriptRunner* g_pScriptRunner;              // 0x013D2838
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}

// Prefix test of a cutscene line (inlined everywhere; the literal argument becomes a CCharString temporary).
static __forceinline bool LineStartsWith(const CCharString& line, const CCharString& prefix)
{
    return strncmp(line.GetPString(), prefix.GetPString(), prefix.Length()) ? false : true;
}

bool __fastcall CutsceneCameraPreloader(std::vector<CCharString>& lines, bool bSkipIfMacroPresent, long* pTimeCode)
{
    bool bNoTimeCode = true;
    long count = lines.size();
    if (bSkipIfMacroPresent) {
        for (long i = 0; i < count; i++) {
            if (LineStartsWith(lines[i], "DoCameraPreloading"))
                return false;
        }
    }
    pDSTGame->CameraSetCameraPreloadFlag(true);
    for (long j = 0; j < count; j++) {
        if (LineStartsWith(lines[j], "StartTimeCode"))
            bNoTimeCode = false;
        if (LineStartsWith(lines[j], "UseCamera") || LineStartsWith(lines[j], "CameraLookAt")
            || LineStartsWith(lines[j], "CameraLookBetween") || LineStartsWith(lines[j], "CameraFOVLookBetween")) {
            long start = lines[j].FindCharPos(' ') + 1;
            long end = lines[j].Length();
            long comma = lines[j].FindCharPos(',');
            if (comma != -1)
                end = comma;
            CCharString name = lines[j].GetSubString(start, end);
            name.RemoveWhiteSpaceBeginEnd();
            pDSTGame->CameraUseCameraPoint(name, 0, 0, -1.0f, 0, -1);
        }
    }
    pDSTGame->CameraSetCameraPreloadFlag(false);
    if (bNoTimeCode)
        *pTimeCode = 0;
    CCharString region = pDSTGame->GetRegionName();
    while (!pDSTGame->IsRegionLoadedAndPreloaded(region)) {
        NProgressDisplay::DisplayProgress("", -1.0f, false, false);
        if (g_pScriptRunner && g_pScriptRunner->m_bStopRequested)
            break;
        pDSTGame->NewScriptFrame();
    }
    return true;
}
