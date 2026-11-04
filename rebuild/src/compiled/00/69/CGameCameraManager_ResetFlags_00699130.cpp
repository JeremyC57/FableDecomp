// CGameCameraManager::ResetFlags() -- clear the activated flag of every live available camera point, then empty the camera point list
// Leaf: ?ResetFlags@CGameCameraManager@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00699130, 183 bytes, plain `ret`. Ego_r.pdb ?ResetFlags@CGameCameraManager@@AAEXXZ (private, 0x006BFD70).
// Callees: CBaseIntelligentPointer::GetPItem const 0x00A01B50 (three per point: Get(), then operator-> twice);
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inside CThing::GetTC<T>); the element's virtual destructor
//   (slot 0) and free 0x00BFEA14 in the inline STLport list clear().
// Layout: CGameCameraManager AvailableCamPoints +0xAC (STLport list, header-node pointer); CTCCameraPointDefinitionBase (Ego_r.pdb)
//   Activated +0x3C, interface TCI_CAMERA_POINT (63).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMPOINTS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameCameraManager.h"
class CTCCameraPointDefinitionBase : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CAMERA_POINT; }
    unsigned char _pad_after_base[0x3c - sizeof(CTCBase)];
    bool Activated;                             // +0x3c
};

void CGameCameraManager::ResetFlags()
{
    for (std::list<CIntelligentPointer<CThing> >::iterator it = AvailableCamPoints.begin(); it != AvailableCamPoints.end(); ++it) {
        if ((*it).Get() && !(*it)->IsDead()) {
            CTCCameraPointDefinitionBase* point;
            if ((*it)->GetTC(&point))
                point->Activated = false;
        }
    }
    AvailableCamPoints.clear();
}