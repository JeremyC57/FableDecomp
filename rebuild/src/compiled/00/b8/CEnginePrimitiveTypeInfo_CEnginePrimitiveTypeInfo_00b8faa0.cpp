// CEnginePrimitiveTypeInfo::CEnginePrimitiveTypeInfo() -- both bases (CEngineComponent, CBaseClassNonCopyable at +0x0C), both vtables, the 47 primitive-manager slots cleared
// Leaf: ??0CEnginePrimitiveTypeInfo@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8FAA0, 48 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtables 0x012A3D4C (the CBaseClassNonCopyable-in-CEnginePrimitiveTypeInfo table) and
//   0x012A3D14 (primary), RTTI .?AVCEnginePrimitiveTypeInfo@@ : CEngineComponent@0, CBaseClassNonCopyable@12; Ego_r.pdb
//   ??0CEnginePrimitiveTypeInfo@@QAE@XZ, PrimitiveManagers CEnginePrimitiveManager*[47] at +0x10 (`rep stosd`, ecx 0x2F).
// Callees: CEngineComponent::CEngineComponent 0x00B59710, CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0.
// Layout note: engine/CEnginePrimitiveTypeInfo.h flattens both bases into pads; the constructor has to run them, so the
//   class is modelled locally with its RTTI bases (Header debt).
// Shape: the two base constructors, then the slot array cleared by a counted loop, which VC7.1 turns into `rep stosd` (memset puts
//   the `xor eax,eax` before the `lea edi`).
class CBaseClass {
public:
    CBaseClass();                                    // 0x0099A2D0
    virtual ~CBaseClass();                           // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                         // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                         // 0x0099A300 (protected virtual: Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CEngineComponent {
public:
    CEngineComponent();                              // 0x00B59710
    virtual ~CEngineComponent();                     // 0x00B59730
    void** GlobalPointerAddress;                     // +0x04
    bool RenderEnabled;                              // +0x08
    bool KeepAlive;                                  // +0x09
};
class CEnginePrimitiveManager;

class CEnginePrimitiveTypeInfo : public CEngineComponent, public CBaseClassNonCopyable {
public:
    CEnginePrimitiveTypeInfo();
    CEnginePrimitiveManager* PrimitiveManagers[47];  // +0x10
};

CEnginePrimitiveTypeInfo::CEnginePrimitiveTypeInfo()
{
    for (long i = 0; i < 47; ++i)
        PrimitiveManagers[i] = 0;
}