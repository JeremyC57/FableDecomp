// CVertexShader::CVertexShader(CArray<unsigned long>*, D3DVERTEXELEMENT9*, const void*, const void*) -- create D3D shader and declaration, then shared data
// Retail 0x00A63020, 167 bytes. Canonical engine/CVertexShader.h establishes {vptr,Data};
// it is a flattened data-only record, so the native virtual constructor uses this equivalent view.
// PDB CVertexShaderData fields: VertexDeclaration+4, VertexShader+8, FormatDecl+c, ReferenceCount+10.
// GFGetSystemManager009A4EC0 -> PDisplayManager+60 -> D3DDevice+58; CBaseClassNonCopyable ctor0099A2F0 (RTTI vtable0129A7C4, deleting destructor0099A470).
#include "engine/CVertexShader.h"
struct D3DVertexElement;
struct D3DVertexShader;
struct D3DVertexDeclaration;
struct D3DShaderDevice;
struct D3DShaderDeviceMethods {
    void* Slots0[86];
    long (__stdcall *CreateVertexDeclaration)(D3DShaderDevice*, const D3DVertexElement*, D3DVertexDeclaration**);
    void* Slots87[4];
    long (__stdcall *CreateVertexShader)(D3DShaderDevice*, const unsigned long*, D3DVertexShader**);
};
struct D3DShaderDevice { D3DShaderDeviceMethods* Methods; };
struct CShaderDisplayView { unsigned char BeforeDevice[0x58]; D3DShaderDevice* D3DDevice; };
struct CShaderSystemView { unsigned char BeforeDisplay[0x60]; CShaderDisplayView* PDisplayManager; };
CShaderSystemView* __fastcall GetShaderSystemManager(); // 0x009A4EC0
struct CShaderBytecodeArray {
    unsigned long* Start; unsigned long* Finish; unsigned long* End;
    const unsigned long* DataOrSelf() const {
        unsigned long count = (unsigned long)(Finish-Start);
        if (count > 0) return Start;
        return (const unsigned long*)this;
    }
};
struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable(); // vtable0129A7C4 slot0, deleting destructor0099A470
    CBaseClassNonCopyable(); // 0x0099A2F0
};
struct CVertexShaderData : CBaseClassNonCopyable {
    D3DVertexDeclaration* VertexDeclaration;
    D3DVertexShader* VertexShader;
    const D3DVertexElement* FormatDecl;
    unsigned long ReferenceCount;
    virtual ~CVertexShaderData(); // vtable0129DA50 slot0, deleting destructor00A630D0
    CVertexShaderData(D3DVertexDeclaration* declaration, D3DVertexShader* shader, const D3DVertexElement* format)
        : CBaseClassNonCopyable(), VertexDeclaration(declaration), VertexShader(shader), FormatDecl(format), ReferenceCount(1) {}
};
class CShaderBase { public: virtual ~CShaderBase() {} };
struct CVertexShaderConstructorView : CShaderBase {
    CVertexShaderData* Data;
    virtual ~CVertexShaderConstructorView();
    CVertexShaderConstructorView(const CShaderBytecodeArray*, const D3DVertexElement*, const void*, const void*);
};
CVertexShaderConstructorView::CVertexShaderConstructorView(const CShaderBytecodeArray* bytecode,
    const D3DVertexElement* format, const void*, const void*)
{
    D3DShaderDevice* device = GetShaderSystemManager()->PDisplayManager->D3DDevice;
    D3DVertexShader* shader;
    device->Methods->CreateVertexShader(device, bytecode->DataOrSelf(), &shader);
    D3DVertexDeclaration* declaration;
    device->Methods->CreateVertexDeclaration(device, format, &declaration);
    Data = new CVertexShaderData(declaration, shader, format);
}