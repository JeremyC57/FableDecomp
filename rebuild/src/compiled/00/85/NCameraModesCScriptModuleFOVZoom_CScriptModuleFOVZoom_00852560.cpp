// NCameraModes::CScriptModuleFOVZoom::CScriptModuleFOVZoom(const C3DVector& lookAt, const C3DVector& from, const C3DVector& to, float fovFraction, long frames, bool shortenOnOcclusion) -- constructor: store the look-at, start and options, default 0 frames to 15, compute the per-frame increment and the end frame
// Leaf: ??0CScriptModuleFOVZoom
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852560, length 144 bytes (130 + int3 padding).
// Callees: NCameraModes::CScriptModuleBase::CScriptModuleBase() @ 0x005ED070,
// CScriptModuleFOVZoom::CalculateIncrementVector @ 0x008520F0, GFGetCurrentFrame @ 0x0049D870.
// Vtable: 0x01275A1C (NCameraModes::CScriptModuleFOVZoom, rtti-vtables.tsv).
// Shape: C3DVector carries an inline user copy constructor (the implicit copy goes through a
// `lea` destination register); initialiser list in declaration order (LookAtPos, FOVFraction, StartPos, ShortenOnOcclusion,
// ResetCamera = true); the body sets WorldFrames = frames ? frames : 15, passes the `from` PARAMETER
// (not the member) to CalculateIncrementVector, and adds the raw `frames` to the current frame.
// Layout: PDB NCameraModes::CScriptModuleFOVZoom (see 0x008520F0). Parameter names are descriptive.
long __fastcall GFGetCurrentFrame();                 // 0x0049D870

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4c - 4];
    CScriptModuleBase();                             // 0x005ED070
};

namespace NCameraModes {

struct CScriptModuleFOVZoom : CScriptModuleBase {    // vtable 0x01275A1C
    C3DVector     LookAtPos;                         // +0x4c
    long          EndFrame;                          // +0x58
    long          WorldFrames;                       // +0x5c
    float         FOVIncrement;                      // +0x60
    float         FOVFraction;                       // +0x64
    C3DVector     Increment;                         // +0x68
    C3DVector     StartPos;                          // +0x74
    bool          ShortenOnOcclusion;                // +0x80
    bool          ResetCamera;                       // +0x81
    virtual void v00();
    void CalculateIncrementVector(const C3DVector& from, const C3DVector& to, long frames);   // 0x008520F0
    CScriptModuleFOVZoom(const C3DVector& lookAt, const C3DVector& from, const C3DVector& to,
                         float fovFraction, long frames, bool shortenOnOcclusion);
};

CScriptModuleFOVZoom::CScriptModuleFOVZoom(const C3DVector& lookAt, const C3DVector& from, const C3DVector& to,
                                           float fovFraction, long frames, bool shortenOnOcclusion)
    : LookAtPos(lookAt), FOVFraction(fovFraction), StartPos(from),
      ShortenOnOcclusion(shortenOnOcclusion), ResetCamera(true)
{
    WorldFrames = frames ? frames : 15;
    CalculateIncrementVector(from, to, WorldFrames);
    EndFrame = GFGetCurrentFrame() + frames;
}

}