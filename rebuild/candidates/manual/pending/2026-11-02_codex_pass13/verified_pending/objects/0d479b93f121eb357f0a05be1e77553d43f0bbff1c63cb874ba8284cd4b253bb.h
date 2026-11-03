#ifndef FABLE_ENGINE_CPIXELSHADER_RENDER2D_H
#define FABLE_ENGINE_CPIXELSHADER_RENDER2D_H
#include "engine/CShaderBase.h"
#include "engine/CArray.h"
class CPixelShaderData;class CCharString;struct IDirect3DPixelShader9;
class CPixelShader : public CShaderBase {
private: CPixelShaderData* Data;
private: CPixelShader(IDirect3DPixelShader9 *,const CArray<long> &,const CCharString &);
public: CPixelShader(const CPixelShader &);
public: CPixelShader();
public: virtual ~CPixelShader();
public: void operator=(const CPixelShader &);
public: bool operator==(const CPixelShader &) const;
public: bool operator!=(const CPixelShader &) const;
public: bool IsInitialised() const;
public: void Uninitialise();
public: IDirect3DPixelShader9 * GetD3DPixelShader() const;
private: virtual unsigned long CountReferences();
private: unsigned long GetConstCount() const;
private: const long * PeekConstIndices() const;
};
#endif
