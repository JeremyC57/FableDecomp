// NParticleEngine::CPSCLight::Kill() -- vtable slot 32: kill the light: clear its engine primitive handle (the primitive is told when this was its last handle, then the counted pointer is released)
// Leaf: ?Kill@CPSCLight@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B19C20, 58 bytes (+6 int3 padding).
// Name: FableWin.pdb ?Kill@CPSCLight@NParticleEngine@@UAEXXZ (the only no-argument void virtual CPSCLight adds beyond
// EnterTimelineEnd, slot 25 0x00B1AE60); slot 32 of the vtable 0x012A0624 (rtti-vtables.tsv).
// Callees: the primitive's virtual slots 3 (+0x0C, told its last handle goes away, with the handle) and 1 (+0x04, destroy
// on the last reference).
// Layout: as the landed ~CPSCLight 0x00B1AEB0: PrimHandle (CEnginePrimitiveHandle: vptr + CIVCountedPointer Data) +0x58.
// Shape: the handle's inline Clear() from the landed destructor, verbatim: the null-safe `GetRefCount() == 1` test
// (handle address formed in esi first), then `Data = 0` = the counted release plus the second unconditional zero
// store. Plain `ret`.
struct CBaseClass {
    virtual void v00();
    ~CBaseClass();                           // 0x0099A2E0
};

class CEnginePrimitiveHandle;

struct CEngineInternalPrimitiveBase {        // the referenced primitive: count at +4
    virtual void v00();
    virtual void Destroy();                  // slot 1 (+0x04)
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0x0c)
    long m_handleCount;                      // +0x04
};

struct CIVCountedPointer {                   // the counted primitive pointer (as the landed handle ??_G 0x0041C500)
    CEngineInternalPrimitiveBase* Object;
    long GetRefCount() const { return Object ? Object->m_handleCount : 0; }
    __forceinline void Release()
    {
        CEngineInternalPrimitiveBase* object = Object;
        if (object) {
            if (--object->m_handleCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->m_handleCount;
        return *this;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};

class CEnginePrimitiveHandle : public CBaseClass {   // vtable 0x0122F598
public:
    CIVCountedPointer Data;                  // +0x04
    virtual void v00();
    __forceinline void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data.Object->OnLastHandleReleased(this);
        Data = 0;
    }
};

namespace NParticleEngine {
struct CPSCLight {                          // vtable 0x012A0624
    virtual void v00();
    unsigned char          _body[0x58 - 4];  // the component base's state and the light parameters
    CEnginePrimitiveHandle PrimHandle;       // +0x58
    virtual void Kill();
};

void CPSCLight::Kill()
{
    PrimHandle.Clear();
}
} // namespace NParticleEngine