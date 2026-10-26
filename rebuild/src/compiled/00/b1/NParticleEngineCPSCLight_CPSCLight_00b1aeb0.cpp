// NParticleEngine::CPSCLight::~CPSCLight() -- destructor of the particle-system light component: free the user-point array, release the light primitive handle, then the component base
// Grade: RELOCATION_MATCH -- the member handle is the CEnginePrimitiveHandle model of the landed ??_G 0x0041C500: an inline Clear testing a null-safe `GetRefCount() == 1` (handle address formed first, Data loaded into eax after the vtable store, `mov ecx,eax` for the slot-3 call), `Data = 0` through the counted pointer's inline operator=, then the counted pointer's own inline destructor re-reading Data
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1AEB0, length 121 bytes.
// Callees: free @ 0x00BFEA14 (the CArray buffer), the primitive's virtual slots 3 (+0x0c, told that its last handle goes away, with the
// handle) and 1 (+0x04, destroy on the last reference), CBaseClass::~CBaseClass() @ 0x0099A2E0 (the handle's root), CBase::~CBase() @
// 0x0099A300 (the component's root, tail jump). Vtable stored: 0x0122F598 CEnginePrimitiveHandle.
// Label correction: the manifest's "CEngineInternalAttachedPrimitiveInfo::~CEngineInternalAttachedPrimitiveInfo" is a propagated name;
// the only caller is the deleting destructor 0x00B1AE90 = slot 0 of vtable 0x012A0624 (RTTI NParticleEngine::CPSCLight). Layout = Ego_r.pdb
// CPSCLight: PrimHandle (CEnginePrimitiveHandle: vptr + primitive) +0x58, LightUserPointsArray (CArray<C3DVector>, retail 12 bytes) +0x60.
// Shape: no vtable store of the component itself (`__declspec(novtable)`: its own vptr store is dead before the base destructor), the
// members in reverse order: the array buffer freed when non-null, then the handle's inline destructor (its vtable, the last-handle
// notification when the primitive's count is 1, `operator=(0)` = release + a second unconditional zero store, then the counted pointer's release
// re-reading the pointer, then the out-of-line root destructor), and the root destructor as a tail jump.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14

class CBaseClass { public:
    virtual void v00();
    ~CBaseClass();                           // 0x0099A2E0
};

class CEnginePrimitiveHandle;

class CEngineInternalPrimitiveBase { public:        // the referenced primitive: count at +4
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
    __forceinline ~CEnginePrimitiveHandle() { Clear(); }
};

class C3DVector { public:
    float X, Y, Z;
};

template <class T>
class CArray { public:                              // retail CArray: 12 bytes, the buffer freed when non-null
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    ~CArray()
    {
        if (m_pData)
            free(m_pData);
    }
};

namespace N_0099a300 { struct CBase {
    virtual void v00();
    ~CBase();                                // 0x0099A300
}; } using N_0099a300::CBase;

namespace NParticleEngine {

class __declspec(novtable) CPSCLight : public CBase {   // vtable 0x012A0624
public:
    unsigned char          _body[0x58 - 4];  // the component base's state and the light parameters
    CEnginePrimitiveHandle PrimHandle;           // +0x58
    CArray<C3DVector>      LightUserPointsArray; // +0x60
    virtual void v00();
    ~CPSCLight();
};

CPSCLight::~CPSCLight()
{
}

}