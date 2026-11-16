// Leaf: ?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@ABVCCharString@@@Z
// CShaderResource::GetVertexShader(const CCharString& name) const -- look the shader up in the bank by symbol and return the
// vertex shader for that index (GetVertexShader(unsigned long) 0x00A5D510, hidden CVertexShader result).
// NF35 (2026-11-16): was the July stand-in free function `GetPixelShader(CShaderResource*, void*, CPixelShader*, void*)` (catalog
//   CShaderResource::GetVertexShader); renamed to Ego_r.pdb + FableWin.pdb ?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@ABVCCharString@@@Z
//   (engine/CShaderResource.h names 0x00A5D5F0; 14 landed callers bind it) and moved onto engine/CShaderResource.h +
//   engine/CShaderDataBank.h (CBankFile::FindIndexBySymbol 0x009CCDC0, FABLE_RETAIL_CBANKFILE_FINDINDEX).
// Retail: Fable.exe @ 0x00A5D5F0, 37 bytes.
#include "fable_compiler.h"
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CSHADERDATABANK_CTOR
#define FABLE_RETAIL_CBANKFILE_FINDINDEX
#include "engine/CShaderResource.h"
#include "engine/CShaderDataBank.h"

CVertexShader CShaderResource::GetVertexShader(const CCharString& name) const
{
    return GetVertexShader(ShaderBank->FindIndexBySymbol(name));
}
