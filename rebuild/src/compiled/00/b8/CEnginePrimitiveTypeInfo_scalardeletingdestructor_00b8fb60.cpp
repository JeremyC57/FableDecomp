// CEnginePrimitiveTypeInfo::`scalar deleting destructor'(unsigned int) -- the implicit destructor (~CBaseClassNonCopyable on the base at +0x0C, then ~CEngineComponent), then free the object on flag bit 0
// Leaf: ??_GCEnginePrimitiveTypeInfo@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8FB60, 48 bytes, thiscall, `ret 4`.
// Identity (proven): slot 0 of the primary vtable 0x012A3D14 (RTTI .?AVCEnginePrimitiveTypeInfo@@); Ego_r.pdb
//   ??_GCEnginePrimitiveTypeInfo@@UAEPAXI@Z (donor right after RegisterPrimitiveManager). The catalog label
//   "CEngineSubPrimitiveText `scalar_deleting_destructor'" is propagated (wrong class). Slot 0 of the secondary table
//   0x012A3D4C is 0x00B8FB50, its this-adjusting thunk.
// Callees: CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (protected virtual, Ego_r.pdb MAE), CEngineComponent::~CEngineComponent
//   0x00B59730, operator delete 0x00BFE9BC.
// Shape: no user destructor (no vptr store); the second base's destructor runs on `this ? this+0x0C : 0` (retail's null-
//   tested adjust); bases are destroyed in reverse order. The vtable (and this ??_G) is emitted by the class's real
//   constructor, the byte-exact 0x00B8FAA0 body.
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
void __cdecl operator delete(void* p);                       // 0x00BFE9BC
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