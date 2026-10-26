// NCameraModes::CBase::GetCamTools() const -- the camera manager's camera tools
// Leaf: ?GetCamTools@CBase@NCameraModes@@IBEABVCTools@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084D080, 7 bytes: mov eax,[ecx+40h] ; add eax,78h ; ret.
// Identity (sweep3, FINDINGS chunk 481; was CVertexBufferWin32::DoSizeof):
//   Ego_r.pdb and FableWin.pdb list the protected NCameraModes::CBase accessors GetWorld, GetCamera const, GetCamera,
//   GetCamTools in this order (Ego_r 0x00844C40..0x00844C70, 0x10 apart), as retail 0x0084D050 / 0x0084D060 /
//   0x0084D070 / 0x0084D080, and their bodies read Ego_r CBase::CameraManager (+0x50; PC +0x40) then CGameCameraManager
//   World (+0x74, loaded), Camera (+0x04, address) and CamTools (+0x78, address) at the same offsets on PC.
class CWorld;
class CCamera;

namespace NCameraModes {
class CTools;
}

class CGameCameraManager {                     // only the members the accessors reach (PC offsets = Ego_r)
public:
    unsigned char         _pad00[0x04];
    unsigned char         Camera[0x38];          // +0x04 CCamera
    unsigned char         OldCamera[0x38];       // +0x3C CCamera
    CWorld*               World;                 // +0x74
    unsigned char         CamTools[0x24];        // +0x78 NCameraModes::CTools
};

namespace NCameraModes {

class CBase {
public:
    unsigned char         _pad00[0x40];
    CGameCameraManager*   CameraManager;         // +0x40 (Ego_r +0x50)
protected:
    const CTools& GetCamTools() const;
};

}

namespace NCameraModes {

const CTools& CBase::GetCamTools() const
{
    return *reinterpret_cast<const CTools*>(CameraManager->CamTools);
}

}
