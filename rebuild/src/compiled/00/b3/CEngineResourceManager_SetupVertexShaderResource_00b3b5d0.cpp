// CEngineResourceManager::SetupVertexShaderResource opens a vertex bank, replaces its
// counted shader resource, then binds the bank, declaration and format. Retail
// 00B3B5D0, 242 bytes. The manager uses banks at +0x960 (stride 0x128) and counted
// resources at +0x29c0, unlike the included donor manager/bank layouts. Retail
// CShaderResource is 0x3c bytes: constructor 00A5D870 initializes the input at +0x18,
// and callees 00A5D7C0/00A5D4E0/00A5D4F0 establish bank/declaration/format at +4/+34/+38.
// The donor PDB CShaderResource layout in struct_layouts_egor.tsv is 0x48; no generated
// CShaderResource header is available. That donor layout places a manager pointer at
// +0x18, whereas retail constructor 00A5D870 calls the vertex-input ctor at +0x18.
// The native projection below therefore omits the donor-only manager field.
// Both native CBankFile and CShaderDataBank vtables place OpenReadOnly at slot 1; the
// shader-bank table shares the bank-file ABI prefix but has derived overrides later.
#define CEngineResourceManager CEngineResourceManager_PDB
#include "engine/CEngineResourceManager.h"
#undef CEngineResourceManager
#define CBankFile CBankFile_PDB
#include "engine/CBankFile.h"
#undef CBankFile
#define CShaderDataBank CShaderDataBank_PDB
#include "engine/CShaderDataBank.h"
#undef CShaderDataBank
#include "fable_string.h"
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif

// This retail class uses the native offsets confirmed by the target body. Its
// donor class definition above is included under an alias because that build's
// bank array starts at +0xa94 with a 0x1a8 stride.
enum EShaderBankNo { ShaderBankFixtureBase = 0 };
enum EVertexType { VertexTypeFixtureBase = 0 };
struct _D3DVERTEXELEMENT9;
struct CWideString;
struct CShaderRenderManager;

// This ABI view models only the verified native bank vtable prefix. Retail vtable
// data at 0x0129B54C (CBankFile) and 0x0129D644 (CShaderDataBank) confirms slot 0
// is the virtual destructor and slot 1 is 0x009D06F0 OpenReadOnly; slots 1-4 are
// shared by both tables. The complete 20-slot comparison is preserved in the lane
// evidence; unused slots are deliberately not reconstructed here. The donor headers
// above remain expanded in the fixture and disagree with retail bank stride (0x128).
struct CBankFileNativeInterface {
    virtual ~CBankFileNativeInterface();
    virtual bool OpenReadOnly(const CCharString& name, unsigned long flags); // 0x009D06F0
};
struct CShaderDataBank : CBankFileNativeInterface {
    unsigned char NativeBankState[0x124];
    CShaderDataBank(); // genuine PDB default constructor: Egor 0x00BDF0C0
};
class CShaderResource {
public:
    virtual ~CShaderResource();
    const CShaderDataBank* ShaderBank; // +0x04, written by retail OpenShaderBank
    unsigned char NativeShaders[0x10]; // +0x08, native CArray<CShaderBase*>
    unsigned char NativeVertexShaderInput[0x1c]; // +0x18, retail ctor calls its ctor here
    const _D3DVERTEXELEMENT9* VertexShaderDeclaration; // +0x34, retail setter
    EVertexType VertexFormat; // +0x38, retail setter
    void OpenShaderBank(const CShaderDataBank* bank);
    void SetVertexShaderDeclaration(const _D3DVERTEXELEMENT9* declaration);
    void SetVertexFormat(EVertexType format);
private:
    CShaderResource(); // genuine private default constructor: PDB 0x00BCF2F0; retail helper 0x00A5D870
    friend struct CShaderRenderManager;
};
typedef void (__fastcall *CCPPointerDeleteFunc)(void*);
struct CCPPointerInfoNative {
    unsigned long RefCount;
    CCPPointerDeleteFunc DeleteFunc;
    void* Data;
};
void __cdecl operator delete(void*) throw();
struct CShaderResourcePointer {
    CShaderResource* Data;
    CCPPointerInfoNative* Info;
    CShaderResourcePointer() : Data(0), Info(0) { }
    CShaderResourcePointer(const CShaderResourcePointer& other)
        : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CShaderResourcePointer() { Release(); }
    __forceinline void Release() {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
    }
    __forceinline CShaderResourcePointer& operator=(const CShaderResourcePointer& other) {
        CCPPointerInfoNative* info = other.Info;
        CShaderResource* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (Info) ++Info->RefCount;
        }
        return *this;
    }
};
struct CShaderRenderManager {
    CShaderResourcePointer CreateResource();
};
extern CShaderRenderManager* g_pShaderRenderManager_01436e14;
struct CEngineResourceManager {
    unsigned char NativeBeforeVertexBanks[0x960];
    CShaderDataBank VertexShaderBanks[27];
    unsigned char NativeBetweenBanksAndResources[0x29c0 - 0x960 - 27 * 0x128];
    CShaderResourcePointer VertexShaderResources[27];
    void SetupVertexShaderResource(EShaderBankNo bank, const CCharString& name,
        const _D3DVERTEXELEMENT9* declaration, EVertexType format);
};

void CEngineResourceManager::SetupVertexShaderResource(
    EShaderBankNo bank, const CCharString& name,
    const _D3DVERTEXELEMENT9* declaration, EVertexType format)
{
    CShaderDataBank* shaderBank = &VertexShaderBanks[bank];
    shaderBank->OpenReadOnly(name, 0x48);
    VertexShaderResources[bank] = g_pShaderRenderManager_01436e14->CreateResource();
    VertexShaderResources[bank].Data->OpenShaderBank(shaderBank);
    VertexShaderResources[bank].Data->SetVertexShaderDeclaration(declaration);
    VertexShaderResources[bank].Data->SetVertexFormat(format);
}

#ifdef _MSC_VER
typedef char CShaderDataBankNativeStride[(sizeof(CShaderDataBank) == 0x128) ? 1 : -1];
typedef char CShaderResourceNativeSize[(sizeof(CShaderResource) == 0x3c) ? 1 : -1];
typedef char CCPPointerInfoNativeSize[(sizeof(CCPPointerInfoNative) == 0x0c) ? 1 : -1];
#endif