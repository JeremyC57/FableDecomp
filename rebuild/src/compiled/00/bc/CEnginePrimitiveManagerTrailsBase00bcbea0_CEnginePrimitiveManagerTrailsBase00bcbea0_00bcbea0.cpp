// CEnginePrimitiveManagerTrailsBase_00bcbea0::~CEnginePrimitiveManagerTrailsBase_00bcbea0() -- destroy the dynamic 3D resource manager at +0x14 (its vtable, then its member at +0x24 out of line), release the vertex shader at +0xc, then ~CBase (copy a of two)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCBEA0, length 58 bytes.
// Callees: the resource manager's member destructor @ 0x00BF3160 (the manifest label "CActiveFile::OnReadFinished" is propagated), the shared shader's virtual
// deleting destructor (slot 0, flag 1), CBase::~CBase @ 0x0099A300 (a tail jump). Vtables 0x012A69DC = CEngineDynamic3DResourceManager, 0x0129B3D0 =
// CVertexShader, 0x0129A030 = CShaderBase. Shape: members in reverse order; no vtable store of this class's own (abstract: `__declspec(novtable)`). The
// chunk label "CEnginePrimitiveManagerSpriteTrails `scalar deleting destructor'" is wrong (no flag test): this is the inline destructor of the trails
// managers' shared base, emitted in the sprite-trail and weapon-trail translation units; named by address.
struct CRefCountedShader_00bc9a00 {              // the shared shader object: an intrusive count at +0x10 and a virtual deleting destructor in slot 0
    virtual ~CRefCountedShader_00bc9a00();
    unsigned char _pad[0xc];
    long          m_refCount;                      // +0x10
};

struct CShaderBase {                               // vtable 0x0129A030
    virtual void v00();
    ~CShaderBase() { }
};

struct CVertexShader : CShaderBase {               // vtable 0x0129B3D0: 8 bytes
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

namespace N_0099a300 { struct CBase {
    virtual void v00();
    ~CBase();                                      // 0x0099A300
}; } using N_0099a300::CBase;

struct CResourceOwner_00bf3160 {                   // the resource manager's member at +0x10 of it
    unsigned char _body[4];
    ~CResourceOwner_00bf3160();                    // 0x00BF3160
};

struct CEngineDynamic3DResourceManager {           // vtable 0x012A69DC: 0x14 bytes
    virtual void v00();
    unsigned char           _pad[0xc];             // +0x04 .. +0x10
    CResourceOwner_00bf3160 m_owner;               // +0x10
    ~CEngineDynamic3DResourceManager() { }
};

struct __declspec(novtable) CEnginePrimitiveManagerTrailsBase_00bcbea0 : CBase {
    long                           m_type;         // +0x04
    bool                           m_enabled;      // +0x08
    CVertexShader                  m_vertexShader; // +0x0c
    CEngineDynamic3DResourceManager m_resources;   // +0x14
    ~CEnginePrimitiveManagerTrailsBase_00bcbea0();
};

CEnginePrimitiveManagerTrailsBase_00bcbea0::~CEnginePrimitiveManagerTrailsBase_00bcbea0()
{
}