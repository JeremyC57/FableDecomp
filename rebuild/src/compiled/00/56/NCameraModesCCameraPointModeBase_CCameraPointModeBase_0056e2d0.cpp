#pragma optimize("s",on)
// NCameraModes::CCameraPointModeBase::~CCameraPointModeBase() -- stores the class vtable, destroys the camera-point
// pointer at +0xa4, then chains to the NCameraModes::CBase destructor (tail jump)
// Leaf: ??1CCameraPointModeBase@NCameraModes@@MAE@XZ
// Retail: Fable.exe @ 0x0056E2D0, 32 bytes: push esi ; mov esi,ecx ; lea ecx,[esi+0A4h] ; mov [esi],0x0124DDDC ;
//   call 0x00A01C10 ; mov ecx,esi ; pop esi ; jmp 0x0056E0E0.
// Identity: vtable 0x0124DDDC has RTTI .?AVCCameraPointModeBase@NCameraModes@@ (base .?AVCBase@NCameraModes@@), and
// Ego_r/FableWin name the destructor ??1CCameraPointModeBase@NCameraModes@@MAE@XZ (protected virtual). Family pass 3 had
// modelled it as CInputProcessManager (whose vtable is 0x0129CA38). Base destructor ??1CBase@NCameraModes@@UAE@XZ
// (PDB + catalog 0x0056E0E0). The member is Ego_r's PCameraPoint (CIntelligentPointer<const CThing>, Ego_r +200,
// retail +0xa4); its destructor is the base ~CBaseIntelligentPointer (0x00A01C10), modelled as that base type.
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
private:
    CBaseObjectPointer* PData;                           // +0x04
};

namespace NCameraModes {

class CBase {
public:
    virtual ~CBase();                                    // 0x0056E0E0
private:
    unsigned char _pad_0x04[0xa0];
};

class CCameraPointModeBase : public CBase {
protected:
    virtual ~CCameraPointModeBase();
    CBaseIntelligentPointer PCameraPoint;                // +0xa4
};

CCameraPointModeBase::~CCameraPointModeBase()
{
}

}
