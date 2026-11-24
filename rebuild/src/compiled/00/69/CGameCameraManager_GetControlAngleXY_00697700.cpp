// Leaf: ?GetControlAngleXY@CGameCameraManager@@UBEMXZ
// CGameCameraManager::GetControlAngleXY @ 0x00697700
//
// Virtual const float accessor. Under VC7.1 (MSVC 2003) a this-only
// float getter compiled with /O2 emits `fld [ecx+disp]; ret` where the
// float return value is left in ST0. We model the __fastcall member as
// a __fastcall free function so `this` arrives in ECX (byte-identical),
// and place the returned float member at exactly offset 0xB8.

// Layout: single float member sits at +0xB8. Pad the leading region.
class CGameCameraManager
{ public:
    char _pad0[0xb4];      // 0x00 .. 0xB7
    float m_controlAngleXY; // 0xB8  <-- fld DWORD PTR [ecx+0xB8]
    virtual float GetControlAngleXY() const;
};

// __fastcall models __fastcall: object pointer (this) in ECX.
// (const accessor => object pointer is read-only; treat as CGameCameraManager*.)
float CGameCameraManager::GetControlAngleXY() const
{
    CGameCameraManager* thisptr = const_cast<CGameCameraManager*>(this);
    return thisptr->m_controlAngleXY;
}
// Flatten route (flatten_pilot v6, NF335): GetControlAngleXY_CGameCameraManager -> CGameCameraManager::GetControlAngleXY, the RTTI-placed PDB member; class model: the row's local class model.
