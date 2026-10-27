// CContainmentVolume::CContainmentVolume() -- store the vtable and zero the three member dwords
// Leaf: ??0CContainmentVolume@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a752d0, 20 bytes, thiscall, `ret`.
// Identity (proven): it stores vtable 0x0129DC94 (RTTI .?AVCContainmentVolume@@) and zeroes Ego_r.pdb CContainmentVolume's ContainmentPlanes / Size / MemoryPool; bound ??0CContainmentVolume@@QAE@XZ; callers CPhysicsMeshObject::GetNavigationLevelVolumes 0x00A12EE0, C3DMeshVolume::LoadBinary 0x00A2E860 and C3DMeshVolume::C3DMeshVolume 0x00A2E9A0 (landed).
// Shape (family L109): `mov eax,ecx ; xor ecx,ecx`, the vtable 0x0129DC94, the three zero stores, `this` returned.
class CPlane;
class CMovableResourceMemoryPool;
class CContainmentVolume {
public:
    CContainmentVolume();
    virtual ~CContainmentVolume();
    CPlane*                     ContainmentPlanes;         // +0x04
    unsigned long               Size;                      // +0x08
    CMovableResourceMemoryPool* MemoryPool;                // +0x0C
};
CContainmentVolume::CContainmentVolume()
    : ContainmentPlanes(0), Size(0), MemoryPool(0)
{
}