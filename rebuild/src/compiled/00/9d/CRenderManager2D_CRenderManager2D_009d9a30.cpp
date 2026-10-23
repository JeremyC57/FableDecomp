// CRenderManager2D::CRenderManager2D(CDisplayManager& display, const CWideString& dataPath, unsigned long flags) -- construct the 2D render manager over CRenderManagerCore: empty quick-draw queues; in retail-bank mode first open the retail shader bank <dataPath>pc\shaders.big; if the SHADERS_BBBLIB bank exists open it read-only, create the vertex shader resource over it (2D declaration, format 0x1c4) and fetch VSHADER_BBBLIB_2D; remember flag bit 0 as AlternateRenderTargetClearEnabled
// Leaf: ??0CRenderManager2D
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D9A30, length 592 bytes (ret 0xc, no EH frame); PDB
// ??0CRenderManager2D@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z.
// Callees: CRenderManagerCore(CDisplayManager&, unsigned long) 0x00A0AFE0, CShaderDataBank() 0x00A62020, CVertexShader()
// 0x00A62FB0, CWideString(const unsigned short*) 0x0099B6B0 / ~CWideString 0x0099B510, operator+(const CWideString&, const
// CWideString&) 0x0099BE70 and (const CWideString&, const unsigned short*) 0x0099BF30 (__fastcall, hidden result),
// CBankFileManager::OpenRetailBank(const CWideString&, bool) 0x009A8840 and BankExists(const CCharString&) 0x009A80B0 on
// the manager singleton 0x013CA79C (retail-mode byte 0x013CA7B0), CCharString(const char*, long) 0x0099EBF0 / dtor
// 0x0099EAE0, the bank's virtual OpenReadOnly (slot 1), CShaderRenderManager::CreateResource 0x0098B860 on the static
// manager 0x013BC470, CCountedPointer<CShaderResource>::operator= 0x009DE7C0, operator delete 0x00BFE9BC,
// CShaderResource::OpenShaderBank 0x00A5D7C0 / SetVertexShaderDeclaration 0x00A5D4E0 / SetVertexFormat 0x00A5D4F0 /
// GetVertexShader 0x00A5D5F0. Data: vtables 0x0129B9EC (CRenderManager2D), 0x0129B3D0 (CVertexShader), 0x0129A030
// (CShaderBase); the 2D vertex declaration 0x0129D708.
// Layout: Ego_r.pdb CRenderManager2D: VertexShaderBank +0x3d20, VertexShaderResource +0x3e48, VertexShader2D +0x3e50,
// the six quick-draw vectors +0x3e58..+0x3e9c, AlternateRenderTargetClearEnabled +0x3ea0. The shader models follow the
// landed CEnginePrimitiveManager2DSprites ctor 0x00BAD040 and CEngineResourceManager::SetupVertexShaderResource 0x00B3B5D0.
// Shape: the L"pc\\" platform string is a named local in the retail-mode block (a temporary is built after the bool
// argument push); the bank name is a named CCharString (it lands in the dead display-argument slot); the resource calls
// go through the counted pointer's inline operator-> and the shader name is an implicitly converted literal (an
// explicit CCharString(...) loads Data into ecx at the call instead of edi before the pushes).
#ifndef _MSC_VER
#define __fastcall
#endif
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* m_pData;
    CWideString(const unsigned short* text);                   // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b);      // 0x0099BF30

struct CBankFileManager {
    unsigned char _pad[0x14];
    bool m_retailMode;                                  // +0x14
    void OpenRetailBank(const CWideString& path, bool flag);      // 0x009A8840
    bool BankExists(const CCharString& bankHandle) const;          // 0x009A80B0
};
extern CBankFileManager g_bankFileManager;              // 0x013CA79C

struct CShaderDataBank {
    virtual ~CShaderDataBank();
    virtual bool OpenReadOnly(const CCharString& name, unsigned long flags);   // slot 1
    unsigned char NativeBankState[0x124];
    CShaderDataBank();                                  // 0x00A62020
};

struct CVertexShaderData {
    virtual ~CVertexShaderData();
    void* VertexDeclaration;
    void* VertexShader;
    const void* FormatDecl;
    long ReferenceCount;                                // +0x10
    long RemoveReference() { return --ReferenceCount; }
    void ReleaseReference() { if (--ReferenceCount == 0) delete this; }
};
struct CShaderBase {                                    // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
struct CVertexShader : CShaderBase {                    // vtable 0x0129B3D0
    CVertexShaderData* Data;
    CVertexShader();                                    // 0x00A62FB0
    CVertexShader& operator=(const CVertexShader& other)
    {
        if (Data) Data->ReleaseReference();
        Data = other.Data;
        ++Data->ReferenceCount;
        return *this;
    }
    virtual ~CVertexShader() { if (Data) { CVertexShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
};

struct _D3DVERTEXELEMENT9;
extern const _D3DVERTEXELEMENT9 g_Declaration2D[];     // 0x0129D708
enum EVertexType { VERTEX_TYPE_2D = 0x1c4 };
struct CShaderResource {
    void OpenShaderBank(const CShaderDataBank* bank);                       // 0x00A5D7C0
    void SetVertexShaderDeclaration(const _D3DVERTEXELEMENT9* declaration); // 0x00A5D4E0
    void SetVertexFormat(EVertexType format);                               // 0x00A5D4F0
    CVertexShader GetVertexShader(const CCharString& name);                 // 0x00A5D5F0
};
typedef void (__fastcall *CCPPointerDeleteFunc)(void*);
struct CCPPointerInfo {
    unsigned long RefCount;
    CCPPointerDeleteFunc DeleteFunc;
    void* Data;
};
void operator delete(void*);
struct CShaderResourcePointer {                         // CCountedPointer<CShaderResource>
    CShaderResource* Data;
    CCPPointerInfo* Info;
    CShaderResourcePointer() : Data(0), Info(0) { }
    ~CShaderResourcePointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
    }
    CShaderResourcePointer& operator=(const CShaderResourcePointer& other);   // 0x009DE7C0
    CShaderResource* operator->() const { return Data; }
};
struct CShaderRenderManager {
    CShaderResourcePointer CreateResource();            // 0x0098B860
};
extern CShaderRenderManager g_ShaderRenderManager;      // 0x013BC470

struct CTVertexRHWColSpecTex1;
struct CQuickDrawTriInfo;
template<class T> struct CVector {                      // STLport vector (12 bytes)
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CDisplayManager;
struct CRenderManagerCore {
    virtual ~CRenderManagerCore();
    unsigned char _pad0004[0x3d1c];
    CRenderManagerCore(CDisplayManager& display, unsigned long flags);   // 0x00A0AFE0
};
struct CRenderManager2D : CRenderManagerCore {          // vtable 0x0129B9EC
    CShaderDataBank                  VertexShaderBank;          // +0x3d20
    CShaderResourcePointer           VertexShaderResource;      // +0x3e48
    CVertexShader                    VertexShader2D;            // +0x3e50
    CVector<CTVertexRHWColSpecTex1>  QuickDrawVertices;         // +0x3e58
    CVector<CQuickDrawTriInfo>       QuickDrawTriInfo;          // +0x3e64
    CVector<CTVertexRHWColSpecTex1>  QuickDrawAlphaVertices;    // +0x3e70
    CVector<CQuickDrawTriInfo>       QuickDrawAlphaTriInfo;     // +0x3e7c
    CVector<CTVertexRHWColSpecTex1>  QuickDraw2DVertices;       // +0x3e88
    CVector<CQuickDrawTriInfo>       QuickDraw2DTriInfo;        // +0x3e94
    bool                             AlternateRenderTargetClearEnabled;   // +0x3ea0
    CRenderManager2D(CDisplayManager& display, const CWideString& dataPath, unsigned long flags);
    virtual ~CRenderManager2D();
};

CRenderManager2D::CRenderManager2D(CDisplayManager& display, const CWideString& dataPath, unsigned long flags)
    : CRenderManagerCore(display, flags)
{
    if (g_bankFileManager.m_retailMode) {
        CWideString platform(L"pc\\");
        g_bankFileManager.OpenRetailBank(dataPath + platform + L"shaders.big", false);
    }
    CCharString bankName("SHADERS_BBBLIB");
    if (g_bankFileManager.BankExists(bankName)) {
        VertexShaderBank.OpenReadOnly(bankName, 0x48);
        VertexShaderResource = g_ShaderRenderManager.CreateResource();
        VertexShaderResource->OpenShaderBank(&VertexShaderBank);
        VertexShaderResource->SetVertexShaderDeclaration(g_Declaration2D);
        VertexShaderResource->SetVertexFormat(VERTEX_TYPE_2D);
        VertexShader2D = VertexShaderResource->GetVertexShader("VSHADER_BBBLIB_2D");
    }
    AlternateRenderTargetClearEnabled = (flags & 1) != 0;
}