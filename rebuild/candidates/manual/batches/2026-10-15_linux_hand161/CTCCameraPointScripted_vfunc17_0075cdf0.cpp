// CTCCameraPointScripted::vfunc_17(CCharString& error) -- validity check: start position set, a look direction or thing set, and an end position whenever there is a transition time; writes the reason into `error` and returns false on the first failure
//
// Retail 0x0075CDF0, 116 bytes (`ret 4`). Vtable 0x01269C84 (CTCCameraPointScripted) slot 17.
// Callee: CCharString::operator=(const char*) @ 0x0099EFE0. Strings 0x01269D6C "start pos not set",
// 0x01269D48 "look at direction or thing not set", 0x01269D14 "I'm transitioning but there's no end
// pos to go to". 0x0129BA3C is an extern float variable (0.0001f).
// Layout: Ego_r.pdb CTCCameraPointScripted (0x88): ThingUID (ulong64) +0x70, TransitionTime +0x7c,
// LookDirSet +0x80, StartPosSet +0x81, EndPosSet +0x82.
// Shape: three early-return `if`s; `ThingUID == 0` gives `mov eax,[lo] ; or eax,[hi]`;
// `TransitionTime > eps` gives `fcomp ; test ah,41h ; jne`.
struct CCharString {
    void* PStringData;
    CCharString& operator=(const char* s);          // 0x0099EFE0
};

extern float g_CameraPointMinTransitionTime;        // 0x0129BA3C (0.0001f)

#pragma pack(push, 1)
struct C3DVector { float X, Y, Z; };

struct CTCCameraPointScripted {
    virtual void v00();
    unsigned char      _pad04[0x40 - 0x04];     // +0x04 CTCBase + CTCCameraPointDefinitionBase
    C3DVector          StartPos;                // +0x40
    C3DVector          EndPos;                  // +0x4c
    C3DVector          LookPos;                 // +0x58
    C3DVector          LookPosEnd;              // +0x64
    unsigned __int64   ThingUID;                // +0x70
    long               PAT;                     // +0x78 EEditorPointAcquisionType
    float              TransitionTime;          // +0x7c
    bool               LookDirSet;              // +0x80
    bool               StartPosSet;             // +0x81
    bool               EndPosSet;               // +0x82
    unsigned char      _pad83[5];

    bool vfunc_17(CCharString& error) const;
};
#pragma pack(pop)

bool CTCCameraPointScripted::vfunc_17(CCharString& error) const
{
    if (!StartPosSet)
    {
        error = "start pos not set";
        return false;
    }
    if (!LookDirSet && ThingUID == 0)
    {
        error = "look at direction or thing not set";
        return false;
    }
    if (!EndPosSet && TransitionTime > g_CameraPointMinTransitionTime)
    {
        error = "I'm transitioning but there's no end pos to go to";
        return false;
    }
    return true;
}
