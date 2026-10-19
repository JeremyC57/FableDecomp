#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define FABLE_ENGINE_CVERTEXSHADER_H
struct CVertexShaderData;
struct CVertexShader { void* __vftable; CVertexShaderData* Data; };
#endif
static int bad, stage, systemCalls, shaderCalls, declarationCalls, baseCalls, allocs, failAllocation, failCom;
void* operator new(size_t count)
{
    ++allocs;
    if (stage != 2) bad=1;
    stage=3;
    if (failAllocation) return 0;
    return malloc(count);
}
void operator delete(void* value) throw() { free(value); }
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
struct CShaderBase { virtual ~CShaderBase() {} };
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

struct D3DVertexElement { int value; };
struct D3DVertexShader { int value; };
struct D3DVertexDeclaration { int value; };
static D3DVertexShader shaderObject;
static D3DVertexDeclaration declarationObject;
static D3DShaderDevice device;
static D3DShaderDeviceMethods methods;
static CShaderDisplayView display;
static CShaderSystemView systemManager;
static const unsigned long* expectedCode;
static const D3DVertexElement* expectedFormat;
static long __stdcall createShader(D3DShaderDevice* self, const unsigned long* code, D3DVertexShader** output)
{
    if (self!=&device || code!=expectedCode || stage!=0 || systemCalls!=1) bad=1;
    ++shaderCalls; stage=1;
    *output=failCom ? 0 : &shaderObject;
    return failCom ? -1 : 0;
}
static long __stdcall createDeclaration(D3DShaderDevice* self, const D3DVertexElement* format, D3DVertexDeclaration** output)
{
    if (self!=&device || format!=expectedFormat || stage!=1 || shaderCalls!=1) bad=1;
    ++declarationCalls;stage=2;
    *output=failCom ? 0 : &declarationObject;
    return failCom ? -1 : 0;
}
CShaderSystemView* __fastcall GetShaderSystemManager()
{
    ++systemCalls;
    return &systemManager;
}
CBaseClassNonCopyable::CBaseClassNonCopyable()
{
    if (stage!=3) bad=1;
    ++baseCalls;
}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CVertexShaderData::~CVertexShaderData() {}
CVertexShaderConstructorView::~CVertexShaderConstructorView() {}
static int run(int populated, int allocationFailure, int comFailure)
{
    bad=stage=systemCalls=shaderCalls=declarationCalls=baseCalls=allocs=0;
    failAllocation=allocationFailure;
    failCom=comFailure;
    unsigned long words[3]={1,2,3};
    CShaderBytecodeArray bytecode;
    bytecode.Start=words;
    bytecode.Finish=words+(populated ? 3 : 0);
    bytecode.End=words+3;
    D3DVertexElement format;
    expectedCode=populated ? words : (const unsigned long*)&bytecode;
    expectedFormat=&format;
    CVertexShaderConstructorView shader(&bytecode,&format,(void*)17,(void*)23);
    if (bad || stage!=3 || systemCalls!=1 || shaderCalls!=1 || declarationCalls!=1 || allocs!=1) return 0;
    if (allocationFailure) return !shader.Data && !baseCalls;
    if (!shader.Data || baseCalls!=1 || shader.Data->ReferenceCount!=1 ||
        shader.Data->FormatDecl!=&format ||
        shader.Data->VertexShader!=(comFailure ? 0 : &shaderObject) ||
        shader.Data->VertexDeclaration!=(comFailure ? 0 : &declarationObject)) return 0;
    delete shader.Data;
    return !bad;
}
int main()
{
    methods.CreateVertexShader=createShader;
    methods.CreateVertexDeclaration=createDeclaration;
    device.Methods=&methods;
    display.D3DDevice=&device;
    systemManager.PDisplayManager=&display;
    if (!run(1,0,0) || !run(0,0,0) || !run(1,1,0) || !run(1,0,1)) {
        printf("00a63020_TEST FAIL\n");return 1;
    }
    printf("00a63020_TEST PASS\n");return 0;
}