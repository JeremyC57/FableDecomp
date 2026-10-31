// NCameraModes::CMoveOnCylinder::OnInit() -- start the move-on-cylinder camera: give it a go-to physics leaf, take the observed thing's facing as the X axis, Z straight up, Y = X ^ Z, zero the angle, height parameter 0.75, and cut the camera
// Grade: DIFFER(332v332) -- the slot-70 call's vptr/argument registers: retail `mov eax,[ecx] ; lea edx,[esp+8] ; push edx ; call [eax+118h]`, ours edx/eax swapped (parked residue, unchanged by a named physics local or a named facing temporary). Renamed this round: Ego_r CPhysicsLeaf<GoTo, Rigid, Rigid> ctor (RTTI of its vtable), AddSubComponent, SetSingleCut, CTCPhysicsBase
// Leaf: ?OnInit@CMoveOnCylinder@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085C7D0, 332 bytes, plain `ret`. Ego_r.pdb ?OnInit@CMoveOnCylinder@NCameraModes@@UAEXXZ.
// Callees: operator new 0x00BFEA1A; CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelRigid, CPhysicsCoordRigid>
//   (CBase&, const CGameCameraManager&) 0x0085C950 (its first vtable store 0x01276AEC has RTTI
//   .?AV?$CPhysicsLeaf@VCPhysicsTargetModelGoTo@NCameraModes@@VCPhysicsForceModelRigid@2@VCPhysicsCoordRigid@2@@NCameraModes@@;
//   the parked "GotoOutsideSimple, Cartesian" instantiation is vtable 0x01275974, a different ctor);
//   CBase::AddSubComponent(CCountedPointer<CSubComponentBase>, bool) 0x0084E810 (Ego_r name; the counted-pointer deleter it
//   stores, 0x00850000, is bound as CCountedPointer<CSubComponentBase>::DeleteData); CBase::GetObservedThing() const
//   0x0084F1B0; CTCPhysicsBase::GetFacingVector() const (slot 70, +0x118, C3DVector by hidden pointer).
// Inline: CThing::GetPhysicsTC() const (Ego_r ?GetPhysicsTC@CThing@@QBEPBVCTCPhysicsBase@@XZ), CGameCameraManager::SetSingleCut()
//   (Ego_r ?SetSingleCut@CGameCameraManager@@QAEXXZ: OldCamera = Camera, `rep movsd` x 0xE, then SingleCut).
// Layout: Ego_r.pdb CMoveOnCylinder AxisX/AxisY/AxisZ/Theta/HeightParam at +0xC8.. less the 0x24 the retail
//   NCameraModes::CBase lacks (+0xA4, +0xB0, +0xBC, +0xC8, +0xCC), as the landed CFirstPerson::OnInit 0x008735D0;
//   CGameCameraManager Camera +0x04, OldCamera +0x3C, SingleCut +0xB0 (retail offsets); CThing physics +0x60.
//   engine/CTCPhysicsBase.h flattens the vptr, so CTCPhysicsBase is local with layout-only filler slots v000..v069.
// Header debt: CCountedPointer is local. With engine/CCountedPointer.h (its `if (data) Info = new CCPPointerInfo(1,
//   DeleteData, (void*)Data)`) VC7.1 swaps every cross-product operand pair (28 diff lines; `if (Data)` in a copy of the
//   header: 12). This local model builds the count block through a one-argument helper ctor (CInfo, descriptive shape
//   helper, no PDB name) under `if (Data)` and keeps retail's x87 operand order; same 8-byte layout and member names.
class CCPPointerInfo { public:
    unsigned long RefCount;                                  // +0x0
    void (__fastcall* DeleteFunc)(void*);                     // +0x4
    void* Data;                                              // +0x8
};
template <class T> class CCountedPointer { public:
    struct CInfo : public CCPPointerInfo {
        CInfo(T* data) { RefCount = 1; DeleteFunc = DeleteData; Data = data; }
    };
    T* Data;                                                 // +0x0
    CCPPointerInfo* Info;                                    // +0x4
    static void __fastcall DeleteData(void* data);
    CCountedPointer(T* data) : Data(data)
    {
        if (Data)
            Info = new CInfo(Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other);
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CCamera { public:
    unsigned char _body[0x38];
};
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetFacingVector() const;      // slot 70 (+0x118)
};
class CThing { public:
    unsigned char _pad[0x60];
    CTCPhysicsBase* PPhysicsTC;                     // +0x60
    CTCPhysicsBase* GetPhysicsTC() { return PPhysicsTC; }
    const CTCPhysicsBase* GetPhysicsTC() const { return PPhysicsTC; }
};
class CThingGameObject : public CThing { public: };

class CGameCameraManager { public:
    void* __vftable;                        // +0x00
    CCamera Camera;                         // +0x04
    CCamera OldCamera;                      // +0x3C
    unsigned char _pad74[0x3c];
    bool SingleCut;                         // +0xB0
    void SetSingleCut() { OldCamera = Camera; SingleCut = true; }
};

__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    const float* pa = &a.X; const float* pb = &b.X;
    return C3DVector((float)(double)(pb[2] * pa[1] - pa[2] * pb[1]), (float)(double)(*(&b.Y - 1) * pa[2] - pa[0] * pb[2]), (float)(double)(pb[1] * pa[0] - pa[1] * *(&b.Y - 1)));
}

namespace NCameraModes {
class CSubComponentBase { public:
    virtual ~CSubComponentBase();
};
class CPhysicsTargetModelGoTo : public CSubComponentBase { public: };
class CPhysicsForceModelRigid;
class CPhysicsCoordRigid;
class CBase;
template <class TTarget, class TForce, class TCoord> class CPhysicsLeaf : public TTarget { public:
    unsigned char _body[0x10];
    CPhysicsLeaf(CBase& mode, const CGameCameraManager& manager);   // 0x0085C950 (GoTo, Rigid, Rigid)
};
class CBase { public:
    virtual void v00();
    unsigned char _pad04[0x3c];
    CGameCameraManager* CameraManager;      // +0x40
    unsigned char _pad44[0x60];
    const CThingGameObject* GetObservedThing() const;                                  // 0x0084F1B0
    void AddSubComponent(CCountedPointer<CSubComponentBase> component, bool flag);    // 0x0084E810
};
class CMoveOnCylinder : public CBase { public:
    C3DVector AxisX;                        // +0xA4
    C3DVector AxisY;                        // +0xB0
    C3DVector AxisZ;                        // +0xBC
    float Theta;                            // +0xC8
    float HeightParam;                      // +0xCC
    virtual void OnInit();
};
}

void NCameraModes::CMoveOnCylinder::OnInit()
{
    AddSubComponent(CCountedPointer<CSubComponentBase>(
        new CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelRigid, CPhysicsCoordRigid>(*this, *CameraManager)), false);
    AxisX = GetObservedThing()->GetPhysicsTC()->GetFacingVector();
    AxisZ = C3DVector(0.0f, 0.0f, 1.0f);
    AxisY = CrossProduct(AxisX, AxisZ);
    Theta = 0.0f;
    HeightParam = 0.75f;
    CameraManager->SetSingleCut();
}
