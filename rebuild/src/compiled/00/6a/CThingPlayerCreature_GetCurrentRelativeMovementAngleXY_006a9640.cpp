// CThingPlayerCreature::GetCurrentRelativeMovementAngleXY() -- the XY angle of the display engine's camera transform when its override flag is set, else the camera object's own angle
// Retail: Fable.exe 0x006A9640, 65 bytes.
// Callees: 0x004C7AB0 (a `this`-ignoring member returning the world's display engine, labelled
// CViewBase::GetDisplayEngine), 0x0049D9D0 (engine state record: flag byte at +0x220), display-engine
// vtable slot 6 (+0x18, returns a 0x38-byte transform by hidden pointer), the transform's XY angle
// 0x00A0C380 (fpatan of +0x18/+0x1c), slot 7 (+0x1c) of the engine's camera at +0x34 (float result).
// Shape: the engine in a local (esi); the transform is a by-value temporary in the 0x38-byte frame whose
// angle member is called on the returned pointer; both float results return from the call (no tail jump:
// the frame must be released).
struct CTransform {
    unsigned char _data[0x38];
    float GetAngleXY() const;                            // 0x00A0C380
};

struct CEngineState {
    unsigned char _pad00[0x220];
    bool          m_useTransform;                        // +0x220
};

struct CCamera {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void v004(); virtual void v005(); virtual void v006();
    virtual float GetAngleXY();                          // slot 7 (+0x1c)
};

struct CDisplayEngine {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void v004(); virtual void v005();
    virtual CTransform GetCameraTransform();             // slot 6 (+0x18)
    unsigned char _pad04[0x30];
    CCamera*      m_pCamera;                             // +0x34
    CEngineState* GetState();                            // 0x0049D9D0
};

struct CThingPlayerCreature {
    CDisplayEngine* GetDisplayEngine();                  // 0x004C7AB0
    float GetCurrentRelativeMovementAngleXY();
};

float CThingPlayerCreature::GetCurrentRelativeMovementAngleXY()
{
    CDisplayEngine* engine = GetDisplayEngine();
    if (engine->GetState()->m_useTransform)
        return engine->GetCameraTransform().GetAngleXY();
    return engine->m_pCamera->GetAngleXY();
}