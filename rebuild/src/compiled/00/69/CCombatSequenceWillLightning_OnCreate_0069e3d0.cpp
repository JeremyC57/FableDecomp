// CCombatSequence_WillLightning::OnCreate() -- (label propagated; really a CGameCameraManager-side helper) build a new camera mode over this manager and its +0xDC counted pointer, and hand it to SetCameraMode by value
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E3D0, 134 bytes (incl. padding).
// Callees: operator new 0x00BFEA1A, camera-mode constructor 0x0069DFE0, CCountedPointer::Reset 0x0069C2F0,
//          CGameCameraManager::SetCameraMode 0x00697E20, operator delete 0x00BFE9BC.
//
// Shape: the manifest's CCombatSequence_WillLightning::OnCreate label is wrong (not in any vtable; the
// body calls SetCameraMode 0x00697E20 on `this`, so `this` is the camera manager). A 0xE8-byte camera
// mode is constructed with (this, &descriptor 0x013BA8D8, &m_pTarget) -- its constructor 0x0069DFE0
// copies that counted pointer into +0xA4/+0xA8 with an add-ref -- wrapped in a default-constructed
// counted pointer through Reset, then passed BY VALUE to SetCameraMode: the inline copy constructor
// builds it in the argument area (`sub esp,8 ; mov eax,esp`) and adds a reference (`inc [esi]`); the
// callee destroys its parameter. The local's destructor at the end reuses the block pointer cached
// in esi (`dec [esi] ; jne`): the `--count == 0` form. That caching across the SetCameraMode call
// only happens when Reset (0x0069C2F0, the landed body) is compiled EARLIER in the same TU under
// `#pragma auto_inline(off)`: VC7.1 then knows Reset's side effects and keeps the block in esi and
// `this` in edi (an external Reset makes it re-read the local at every use). The block's destroy
// function is the one Reset stores (0x0056C0F0).
void __fastcall Destroy_0056c0f0(void* object);   // 0x0056C0F0

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_0056c0f0), m_pObject(object) { }
};

struct CCameraModeBase {
    virtual void v00();
};

template <class T> class CCountedPointer { public:
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    CCountedPointer& operator=(T* object) { Reset(object); return *this; }
    void Reset(T* object);                  // 0x0069C2F0 (defined below, earlier in the TU)
    ~CCountedPointer()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            operator delete(m_pCount);
        }
    }
};

#pragma auto_inline(off)
template <class T> void CCountedPointer<T>::Reset(T* object)
{
    if (m_pCount) {
        --m_pCount->m_references;
        if (m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            delete m_pCount;
        }
    }
    m_pCount = 0;
    m_pObject = object;
    if (object)
        m_pCount = new SCountBlock(m_pObject);
}
#pragma auto_inline(on)

struct SCameraDescriptor;
extern const SCameraDescriptor g_cameraDescriptor;   // 0x013BA8D8

class CThing;
struct CCombatSequence_WillLightning;

struct CCameraMode_0069dfe0 : CCameraModeBase {
    unsigned char _body[0xe4];              // sizeof = 0xe8
    CCameraMode_0069dfe0(CCombatSequence_WillLightning* manager, const SCameraDescriptor* desc,
                         const CCountedPointer<CThing>* target);   // 0x0069DFE0
};

struct CCombatSequence_WillLightning {      // the camera manager (CGameCameraManager)
    unsigned char _pad[0xdc];
    CCountedPointer<CThing> m_pTarget;      // +0xdc
    void SetCameraMode(CCountedPointer<CCameraModeBase> mode);   // 0x00697E20
    void OnCreate();
};

void CCombatSequence_WillLightning::OnCreate()
{
    CCountedPointer<CCameraModeBase> mode;
    mode = new CCameraMode_0069dfe0(this, &g_cameraDescriptor, &m_pTarget);
    SetCameraMode(mode);
}