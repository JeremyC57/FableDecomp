// NParticleEngine::CPSCLight::~CPSCLight() -- destructor of the particle-system light component: free the user-point array, release the light primitive handle, then the component base
// Grade: DIFFER(119v121) -- the handle's first null test: the lane hoists `mov ecx,[edi+5Ch] ; test ecx,ecx` above `lea esi,[edi+58h]` and the handle vtable store, retail loads it after the store through esi into eax (`mov eax,[esi+4] ; test ; cmp [eax+4],1 ; mov ecx,eax`); everything else (volatile re-read of the second release, both zero stores, the novtable component) matches. Tried: plain member (second release folded away, 101 bytes), volatile member + locals (handle destructor out of line), __forceinline + volatile member (119v121), __forceinline + volatile re-read helper (119v121).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1AEB0, length 121 bytes.
// Callees: free @ 0x00BFEA14 (the CArray buffer), the primitive's virtual slots 3 (+0x0c, told that its last handle goes away, with the
// handle) and 1 (+0x04, destroy on the last reference), CBaseClass::~CBaseClass() @ 0x0099A2E0 (the handle's root), CBase::~CBase() @
// 0x0099A300 (the component's root, tail jump). Vtable stored: 0x0122F598 CEnginePrimitiveHandle.
// Label correction: the manifest's "CEngineInternalAttachedPrimitiveInfo::~CEngineInternalAttachedPrimitiveInfo" is a propagated name;
// the only caller is the deleting destructor 0x00B1AE90 = slot 0 of vtable 0x012A0624 (RTTI NParticleEngine::CPSCLight). Layout = Ego_r.pdb
// CPSCLight: PrimHandle (CEnginePrimitiveHandle: vptr + primitive) +0x58, LightUserPointsArray (CArray<C3DVector>, retail 12 bytes) +0x60.
// Shape: no vtable store of the component itself (`__declspec(novtable)`: its own vptr store is dead before the base destructor), the
// members in reverse order: the array buffer freed when non-null, then the handle's inline destructor (its vtable, the last-handle
// notification when the primitive's count is 1, `operator=(0)` = release + a second unconditional zero store, then its base's release
// re-reading the pointer, then the out-of-line root destructor), and the root destructor as a tail jump.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14

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
    void Release()
    {
        if (--m_handleCount == 0)
            Destroy();
    }
};

class __declspec(novtable) CPrimitivePointer : public CBaseClass {   // the intelligent-pointer base: vptr + primitive
public:
    CEngineInternalPrimitiveBase* m_pPrimitive;   // +0x04
    CEngineInternalPrimitiveBase* Reread() { return *(CEngineInternalPrimitiveBase* volatile*)&m_pPrimitive; }
    __forceinline void Clear()                             // operator=(0): release, then a second unconditional zero store
    {
        CEngineInternalPrimitiveBase* primitive = Reread();
        if (primitive) {
            primitive->Release();
            m_pPrimitive = 0;
        }
        m_pPrimitive = 0;
    }
    __forceinline ~CPrimitivePointer()
    {
        CEngineInternalPrimitiveBase* primitive = Reread();
        if (primitive) {
            primitive->Release();
            m_pPrimitive = 0;
        }
    }
};

class CEnginePrimitiveHandle : public CPrimitivePointer {   // vtable 0x0122F598
public:
    virtual void v00();
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (m_pPrimitive && m_pPrimitive->m_handleCount == 1)
            m_pPrimitive->OnLastHandleReleased(this);
        Clear();
    }
};

struct C3DVector {
    float X, Y, Z;
};

template <class T>
struct CArray {                              // retail CArray: 12 bytes, the buffer freed when non-null
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    ~CArray()
    {
        if (m_pData)
            free(m_pData);
    }
};

struct CBase {
    virtual void v00();
    ~CBase();                                // 0x0099A300
};

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
