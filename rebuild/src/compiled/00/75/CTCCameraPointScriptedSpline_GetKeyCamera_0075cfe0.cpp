// CTCCameraPointScriptedSpline::GetKeyCamera(float t) -- map a spline parameter in [0,1] to a key camera index (the last one for t >= 1)
//
// Retail 0x0075CFE0, 101 bytes (`ret 4`). Callee: _ftol @ 0x00BFEA70. Constants 0x0122DED8 (1.0f),
// 0x0122DCB4 (4294967296.0f, the unsigned-to-float fix-up).
// Layout: KeyCameras (STLport vector of 0x38-byte CKeyCamera) at +0x68 {first, last, end}, as in
// the landed InitialiseSpline 0x0075D050.
// Shape: size() is the signed byte distance divided by 56 (`imul 92492493h ; sar edx,5`) returned as
// an unsigned size_type, so `(size() - 1) * t` converts through `fild` plus the 2^32 fix-up; each arm
// recomputes size(). `t >= 1.0f` gives `fcomp ; test ah,1 ; jne`.
struct CKeyCamera { unsigned char _body[0x38]; };

class CTCCameraPointScriptedSpline { public:
    unsigned char _pad00[0x68];
    CKeyCamera*   KeyCameras_first;                 // +0x68 vector<CKeyCamera>
    CKeyCamera*   KeyCameras_last;                  // +0x6c
    CKeyCamera*   KeyCameras_end;                   // +0x70
    unsigned long NumKeyCameras() const { return KeyCameras_last - KeyCameras_first; }
    long GetKeyCamera(float t) const;
};

long CTCCameraPointScriptedSpline::GetKeyCamera(float t) const
{
    if (t >= 1.0f)
        return NumKeyCameras() - 1;
    return (long)((NumKeyCameras() - 1) * t);
}