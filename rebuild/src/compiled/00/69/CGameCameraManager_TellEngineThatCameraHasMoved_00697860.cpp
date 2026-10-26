// CGameCameraManager::TellEngineThatCameraHasMoved(bool fastMode) -- build the message "telling engine that the camera has moved and" + (" " | " not ") + "using fast mode", then tell the engine's renderer (global 0x013B86A0 -> +0x28 -> +0x2c, virtual slot 56) with the flag and 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00697860, length 96 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the literal @ 0x0125C2C0, -1), operator+= @ 0x0099F100 twice (" " @ 0x01258C1C or " not "
// @ 0x0125C2B8, then "using fast mode" @ 0x0125C2A8), virtual slot 56 (+0xe0) of the renderer, ~CCharString @ 0x0099EAE0. Shape: a one-word frame for the
// string, the flag kept in ebx, the ternary literal selected with `mov eax,A ; jne ; mov eax,B`, `ret 4`.
extern const char g_text_0125c2c0[];               // "telling engine that the camera has moved and"
extern const char g_text_01258c1c[];               // " "
extern const char g_text_0125c2b8[];               // " not "
extern const char g_text_0125c2a8[];               // "using fast mode"

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    CCharString& operator+=(const char* text);     // 0x0099F100
    ~CCharString();                                // 0x0099EAE0
};

struct CRenderer_00697860 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void SetCameraMoved(bool fastMode, long flag);   // slot 56 (+0xe0)
};
struct CEngineDevice_00697860 {
    unsigned char       _pad[0x2c];
    CRenderer_00697860* m_pRenderer;               // +0x2c
};
struct CEngine_00697860 {
    unsigned char           _pad[0x28];
    CEngineDevice_00697860* m_pDevice;             // +0x28
};
extern CEngine_00697860* g_pEngine_013b86a0;       // 0x013B86A0

class CGameCameraManager { public:
    void TellEngineThatCameraHasMoved(bool fastMode);
};

void CGameCameraManager::TellEngineThatCameraHasMoved(bool fastMode)
{
    CCharString message(g_text_0125c2c0, -1);
    message += fastMode ? g_text_01258c1c : g_text_0125c2b8;
    message += g_text_0125c2a8;
    g_pEngine_013b86a0->m_pDevice->m_pRenderer->SetCameraMoved(fastMode, 1);
}