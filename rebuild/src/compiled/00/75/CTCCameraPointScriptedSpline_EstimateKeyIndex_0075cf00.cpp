// CTCCameraPointScriptedSpline::EstimateKeyIndex() -- spread the key cameras evenly: each key's DeltaIndex is the even spacing 1/(n-1), divided by its duration in frames when it has one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075CF00, length 220 bytes (+ int3 padding)
// (?EstimateKeyIndex@CTCCameraPointScriptedSpline@@QAEXXZ). No calls.
// Globals: 1.0f 0x0122DED8, 2^32 0x0122DCB4 (the unsigned-to-float fix-up), 15.0f 0x0124E640, 0.0f 0x0122DEDC.
// Layout = Ego_r.pdb CTCCameraPointScriptedSpline: KeyCameras +0x68 (STLport vector of 0x38-byte CKeyCamera: DeltaIndex +0x00,
// Duration +0x28).
// Shape: size() is re-evaluated (divide by 56) for the guard, the spacing and every loop test; `(float)(size() - 1)` converts an
// unsigned value; the loop runs `i != size()` (jne) with the element reached as `*(begin() + i)`.
struct C3DVector { float X, Y, Z; };
struct CTCCameraPointScriptedSpline_CKeyCamera {   // CTCCameraPointScriptedSpline::CKeyCamera, 0x38 bytes
    float     DeltaIndex;                       // +0x00
    C3DVector Pos;                              // +0x04
    C3DVector LookDirection;                    // +0x10
    float     FOV;                              // +0x1c
    float     Roll;                             // +0x20
    float     ShuttleSpeed;                     // +0x24
    float     Duration;                         // +0x28
    float     PauseTime;                        // +0x2c
    void*     EventName;                        // +0x30 CCharString
    float     SpeedFraction;                    // +0x34
};
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
class CTCCameraPointScriptedSpline {
public:
    virtual void v00();
    unsigned char _pad04[0x68 - 4];
    CRetailVector<CTCCameraPointScriptedSpline_CKeyCamera> KeyCameras;   // +0x68
    void EstimateKeyIndex();
};

void CTCCameraPointScriptedSpline::EstimateKeyIndex()
{
    if (KeyCameras.size() > 1) {
        float spacing = 1.0f / (float)(KeyCameras.size() - 1);
        for (unsigned long i = 0; i != KeyCameras.size(); ++i) {
            CTCCameraPointScriptedSpline_CKeyCamera& key = KeyCameras[i];
            float frames = key.Duration * 15.0f;
            if (frames == 0.0f)
                key.DeltaIndex = spacing;
            else
                key.DeltaIndex = spacing / frames;
        }
    }
}