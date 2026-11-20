// Leaf: ?GetPixelShader@CShaderResource@@QBE?AVCPixelShader@@ABVCCharString@@@Z
// CShaderResource::GetPixelShader(const CCharString& name) const -- look the shader up in the bank by symbol and return the
// pixel shader for that index (GetPixelShader(unsigned long) 0x00A5D620, hidden CPixelShader result).
// NF161 (2026-11-20, q4550): was the July stand-in `CCharString* CShaderResource::GetPixelShader(CCharString*, CPixelShader*)` on a row-local
//   CShaderResource (own leaf ?GetPixelShader@CShaderResource@@QAEPAVCCharString@@PAV2@PAVCPixelShader@@@Z, callees ResolveHandle / Fetch
//   stand-ins); renamed to Ego_r.pdb 0x00BCF170 + FableWin.pdb 0x032EB400 ?GetPixelShader@CShaderResource@@QBE?AVCPixelShader@@ABVCCharString@@@Z
//   (engine/CShaderResource.h names 0x00A5D720; all 23 landed referencers already bind it) and moved onto engine/CShaderResource.h +
//   engine/CShaderDataBank.h, the form of the landed twin GetVertexShader 0x00A5D5F0 (same body, 0x00A5D510 in place of 0x00A5D620):
//   CBankFile::FindIndexBySymbol 0x009CCDC0 (FABLE_RETAIL_CBANKFILE_FINDINDEX) and the header's const GetPixelShader(unsigned long)
//   (?GetPixelShader@CShaderResource@@QBE?AVCPixelShader@@K@Z, Ego_r.pdb / FableWin.pdb).
// Retail: Fable.exe @ 0x00A5D720, 37 bytes.
#include "fable_compiler.h"
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CSHADERDATABANK_CTOR
#define FABLE_RETAIL_CBANKFILE_FINDINDEX
#include "engine/CShaderResource.h"
#include "engine/CShaderDataBank.h"

CPixelShader CShaderResource::GetPixelShader(const CCharString& name) const
{
    return GetPixelShader(ShaderBank->FindIndexBySymbol(name));
}
