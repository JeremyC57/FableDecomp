// CEnginePrimitiveManagerOcclusionBase::~CEnginePrimitiveManagerOcclusionBase() -- release the counted occlusion data at +0x14/+0x18 (the inlined CCountedPointer destructor), then the vertex shader at +0xc, then ~CBase
// Leaf: ??1CEnginePrimitiveManagerOcclusionBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B38590, length 92 bytes.
// Callees: the count block's destroy (slot +4, the object at +8 in ecx), operator delete @ 0x00BFE9BC, the shared shader's virtual deleting destructor (slot 0,
// flag 1), CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300 (a tail jump). Vtables 0x0129B3D0 = CVertexShader, 0x0129A030 = CShaderBase. Shape: members are destroyed in reverse
// order (the counted pointer first: `dec [eax] ; cmp [eax],0 ; jne ; call [eax+4] ; delete`, both words cleared), then the vertex shader's inlined destructor;
// no vtable store of this class's own (abstract: `__declspec(novtable)`).
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct SCountBlock_00b38590 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};

struct CCountedPointer_00b38590 {                  // 8 bytes: the object and its count block
    void*                m_pObject;                // +0x00
    SCountBlock_00b38590* m_pCount;                // +0x04
    ~CCountedPointer_00b38590()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CRefCountedShader_00bc9a00 {              // the shared shader object: an intrusive count at +0x10 and a virtual deleting destructor in slot 0
    virtual ~CRefCountedShader_00bc9a00();
    unsigned char _pad[0xc];
    long          m_refCount;                      // +0x10
};

class CShaderBase { public:                               // vtable 0x0129A030
    virtual void v00();
    ~CShaderBase() { }
};

class CVertexShader : public CShaderBase { public:               // vtable 0x0129B3D0: 8 bytes
    CRefCountedShader_00bc9a00* m_pShader;         // +0x04
    ~CVertexShader()
    {
        CRefCountedShader_00bc9a00* shader = m_pShader;   // a local: the decrement's flags feed the test directly (no re-read of the member)
        if (shader) {
            if (--shader->m_refCount == 0)
                delete shader;
        }
    }
};

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                      // 0x0099A300
};

struct __declspec(novtable) CEnginePrimitiveManagerOcclusionBase : CBaseClassNonCopyable {
    long                     m_type;               // +0x04
    bool                     m_enabled;            // +0x08
    CVertexShader            m_vertexShader;       // +0x0c
    CCountedPointer_00b38590 m_occlusionData;      // +0x14
    ~CEnginePrimitiveManagerOcclusionBase();
};

CEnginePrimitiveManagerOcclusionBase::~CEnginePrimitiveManagerOcclusionBase()
{
}