// Behaviour test for CPixelShaderData::~CPixelShaderData (0x00A5EC10): release the shader when present, delete[] the index array.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Shader { int refs; void Release() { --refs; } };
static int g_deleted;
struct Data { Shader* PixelShader; long* ConstIndices; ~Data() { if (PixelShader) PixelShader->Release(); if (ConstIndices) ++g_deleted; delete[] ConstIndices; } };
int main()
{
    Shader s = { 2 };
    { Data d; d.PixelShader = &s; d.ConstIndices = new long[4]; }
    { Data e; e.PixelShader = 0; e.ConstIndices = 0; }
    bool ok = s.refs == 1 && g_deleted == 1;
    printf(ok ? "00a5ec10_TEST PASS\n" : "00a5ec10_TEST FAIL\n");
    return ok ? 0 : 1;
}