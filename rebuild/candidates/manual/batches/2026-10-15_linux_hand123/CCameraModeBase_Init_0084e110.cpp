// CCameraModeBase::Init() -- (re)start a camera mode: reset it, capture its three float parameters when the shared camera tracking is valid, and for a type-8 mode or valid tracking run one frame update and roll the tracked camera into the "last" slot
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084E110, 136 bytes (+8 int3), __thiscall, no arguments.
// The catalog label (CBase::Init) is propagated; both callers are CGameCameraManager (SetCameraMode 0x00697E50,
// PreloadScriptedCamera 0x00699B79), which call it right after configuring a camera mode object.
// Callees: the mode's virtual slots 21 (+0x54, reset), 18 / 19 / 20 (+0x48 / +0x4C / +0x50, float getters), 2 (+8,
// the mode type) and CCameraModeBase::FrameUpdate 0x0084DC10.
// Layout (by use): the shared camera tracking pointer at +0x40, the captured-parameters flag at +0x80 and the three
// floats at +0x84..+0x8C; the tracking block holds two 0x38-byte CCamera records (Ego_r.pdb CCamera: position,
// right-handed set, flags, FOVs, zoom) at +4 (current) and +0x3C (last) and its valid flag at +0xB0.
// Shape: the float results are stored straight from x87 (`fstp`); the type test and the validity re-test share
// the FrameUpdate call; the camera roll is an inline helper on the tracking block (valid flag re-read after the
// call, a POD CCamera assignment -> `rep movsd` of 14 dwords with edi saved inside the branch, the flag set).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Forward, Up; };

struct CCamera {                                    // 0x38 bytes, POD
    C3DVector WorldPosition;
    CRightHandedSet RHSet;
    bool HeightLocked;
    bool ViewVecZLocked;
    unsigned long FOVFlags;
    float HorizontalFOV;
    float VerticalFOV;
    float Zoom;
};

struct CCameraTracking {
    long m_00;                                      // +0x00
    CCamera Current;                                // +0x04
    CCamera Last;                                   // +0x3C
    unsigned char _pad_0x74[0x3c];                  // +0x74
    bool Valid;                                     // +0xB0
    void RollCamera()
    {
        if (Valid) {
            Last = Current;
            Valid = true;
        }
    }
};

class CCameraModeBase {
public:
    virtual void v00();
    virtual void v01();
    virtual long GetType();                         // slot 2
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual float GetParameterA();                  // slot 18
    virtual float GetParameterB();                  // slot 19
    virtual float GetParameterC();                  // slot 20
    virtual void Reset();                           // slot 21
    unsigned char _pad_0x04[0x3c];                  // +0x04
    CCameraTracking* m_pTracking;                   // +0x40
    unsigned char _pad_0x44[0x3c];                  // +0x44
    bool m_parametersCaptured;                      // +0x80
    float m_parameterA;                             // +0x84
    float m_parameterB;                             // +0x88
    float m_parameterC;                             // +0x8C
    void FrameUpdate();                             // 0x0084DC10
    void Init();
};

void CCameraModeBase::Init()
{
    Reset();
    if (m_pTracking->Valid) {
        m_parameterA = GetParameterA();
        m_parameterB = GetParameterB();
        m_parameterC = GetParameterC();
        m_parametersCaptured = true;
    }
    if (GetType() == 8 || m_pTracking->Valid) {
        FrameUpdate();
        m_pTracking->RollCamera();
    }
}
