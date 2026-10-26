// NCameraModes::CBase::GetCamera() -- the camera manager's camera (non-const)
// Leaf: ?GetCamera@CBase@NCameraModes@@IAEAAVCCamera@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084D070, 7 bytes: mov eax,[ecx+40h] ; add eax,4 ; ret.
// Identity (sweep3; was CVertexBufferWin32::DoSizeof; label GetCamera):
//   Ego_r.pdb and FableWin.pdb list the protected NCameraModes::CBase accessors GetWorld, GetCamera const, GetCamera,
//   GetCamTools in this order (Ego_r 0x00844C40..0x00844C70, 0x10 apart), as retail 0x0084D050 / 0x0084D060 /
//   0x0084D070 / 0x0084D080, and their bodies read Ego_r CBase::CameraManager (+0x50; PC +0x40) then CGameCameraManager
//   World (+0x74, loaded), Camera (+0x04, address) and CamTools (+0x78, address) at the same offsets on PC.
//   This is the second, non-const GetCamera (IAE after the const IBE one, as 0x0084D070 after its byte twin 0x0084D060);
//   its one caller CScriptedControl::OnInit 0x0087BA90 is non-const.
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
    CCamera& GetCamera();
};

}

namespace NCameraModes {

CCamera& CBase::GetCamera()
{
    return *reinterpret_cast<CCamera*>(CameraManager->Camera);
}

}
