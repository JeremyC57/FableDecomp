// NCameraModes::CSimplePointBase::CSimplePointBase(CGameCameraManager*, const CDefPointer<CCameraModeDef>&, const C3DVector& position, const C3DVector& lookAt, bool allowLookAround) -- the camera-mode root, this class's vtable, the camera position and look-at point copied and the look-around flag stored
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00873A10, length 112 bytes.
// Leaf: ??0CSimplePointBase
// Callees: NCameraModes::CBase::CBase(CGameCameraManager*, ...) @ 0x0084E420 (two arguments; manifest
//          "CActionParam_Constructor" is garbage, see the landed CSnapToPoint 0x0056E330).
// Label: the catalog's "CLookAtFront::CLookAtFront" is propagated -- the only vtable stored, 0x01277644, is
// NCameraModes::CSimplePointBase (CLookAtFront's is 0x01277714). Ego_r.pdb CSimplePointBase: StartPos +0xc8,
// Position +0xd4, LookAtPos +0xe0, AllowLookAround +0xec; the retail root is 0x24 bytes narrower (+0xa4/+0xb0/
// +0xbc/+0xc8 here).
// Shape: C3DVector members copied word by word through an inline memberwise user copy constructor (the loads
// interleave with the stores); the flag byte last; `ret 14h`.
struct CGameCameraManager;
struct CCameraModeDef;
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : x(other.x), y(other.y), z(other.z) { }
};
template <class T> struct CDefPointer;
namespace NCameraModes {
struct CBase {
    virtual void v00();
    unsigned char _body[0xa0];                     // +0x04..+0xa3 (retail)
    CBase(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
};
struct CSimplePointBase : CBase {                  // vtable 0x01277644
    C3DVector StartPos;                            // +0xa4
    C3DVector Position;                            // +0xb0
    C3DVector LookAtPos;                           // +0xbc
    bool      AllowLookAround;                     // +0xc8
    virtual void v00();
    CSimplePointBase(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def,
                     const C3DVector& position, const C3DVector& lookAt, bool allowLookAround);
};

CSimplePointBase::CSimplePointBase(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def,
                                   const C3DVector& position, const C3DVector& lookAt, bool allowLookAround)
    : CBase(manager, def), Position(position), LookAtPos(lookAt), AllowLookAround(allowLookAround)
{
}
}