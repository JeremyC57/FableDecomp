// CEngineWeatherRenderer::BuildBuffers() -- (re)build the weather geometry: 128 random rain quads (512 vertices, one of five constant sets each) with their quad index list, 1024 random snowflake vertices, and four unit mist quads with their 24 indices
// Leaf: ?BuildBuffers@CEngineWeatherRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B51570, 1254 bytes, thiscall, no arguments; ends in a tail jump to CIndexBuffer::Unlock.
// Identity: Ego_r.pdb ?BuildBuffers@CEngineWeatherRenderer@@AAEXXZ (private); the members it fills are the Ego_r.pdb ones at their
//   retail offsets (RainVertexBuffer +0x0C, RainIndexBuffer +0x40, SnowVertexBuffer +0x54, MistVertexBuffer +0x88, MistIndexBuffer +0xBC,
//   Settings.RandomNumberGeneratorSeed +0x1EC); the vertex is Ego_r.pdb CTVertexWeather (X, Y, Z, U1, V1, ConstantIndex; stride 0x18).
// Callees: CVertexBufferWin32::Uninitialise 0x00A63200 / Initialise 0x00A63150, CVertexBufferBase::Lock 0x00AC20A0 / Unlock 0x00AC20B0,
//   CIndexBuffer::Uninitialise 0x00AC1EE0 / Initialise 0x00AC1F70 / GetSize 0x00AC1F10 / Lock 0x00AC1FB0 / Unlock 0x00AC1F00,
//   CEngineResourceManager::GetVertexBufferMemPool 0x00B3B090 / GetVertexBufferUsage 0x00B3B030 / GetIndexBufferMemPool 0x00B3B0A0 /
//   GetIndexBufferUsage 0x00B3B060 (through NEngineInternal::GPResourceManager 0x01436E98), GFFloatRandom 0x004978A0 (the rain loop's
//   three calls), GFROR13 0x00497890 and _CIfmod (the snow loop's five inline GFFloatRandom expansions).
// Shape: the rain loop runs over the vertex index in steps of four with the particle number kept beside it (VC7.1 turns the vertex
//   test into retail's 128 down-counter and the rain[v + k] addresses into one pointer biased by 0x30; the particle number is the
//   signed `i % 5`); its `for` frequency leaves GFFloatRandom out of line there, while the snow loop (after it) expands the inline body.
//   The mist indices are written through a small inline setter (descriptive name SetIndex): its parameter copies split VC7.1's
//   scheduling regions so the 24 stores keep source order, as retail's do (0x00B519BB..0x00B51A47).
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_VBPOOL
#define FABLE_RETAIL_GFFLOATRANDOM_INLINE
#define FABLE_RETAIL_CGENERICVERTEX_INLINE
#include "engine/EVertexType.h"
#include "engine/EPrimitiveType.h"
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CTVertexWeather.h"
#include "engine/GFFloatRandom.h"
enum EIndexBufferFormat { INDEX_BUFFER_FORMAT_16 = 0x65 };      // D3DFMT_INDEX16 (the enumerator name is descriptive)
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98 (established landed spelling)
// descriptive name: one 16-bit index stored into a locked index buffer (no out-of-line copy anywhere; expanded at every use)
inline void SetIndex(unsigned short* indices, long n, unsigned short value) { indices[n] = value; }

void CEngineWeatherRenderer::BuildBuffers()
{
    RainVertexBuffer.Uninitialise();
    RainVertexBuffer.Initialise(VERTEX_NULL, 512, sizeof(CTVertexWeather), NEngineInternal::GPResourceManager->GetVertexBufferUsage(), NEngineInternal::GPResourceManager->GetVertexBufferMemPool());
    CTVertexWeather* rain = (CTVertexWeather*)RainVertexBuffer.Lock(0, 0, RainVertexBuffer.GetSize());
    long i = 0;
    for (long v = 0; v < 512; v += 4, ++i)
    {
        rain[v + 0].X = rain[v + 1].X = rain[v + 2].X = rain[v + 3].X = GFFloatRandom(1.0f, &Settings.RandomNumberGeneratorSeed) - 0.5f;
        rain[v + 0].Y = rain[v + 1].Y = rain[v + 2].Y = rain[v + 3].Y = GFFloatRandom(1.0f, &Settings.RandomNumberGeneratorSeed) - 0.5f;
        rain[v + 0].Z = rain[v + 1].Z = rain[v + 2].Z = rain[v + 3].Z = GFFloatRandom(1.0f, &Settings.RandomNumberGeneratorSeed) - 0.5f;
        rain[v + 0].ConstantIndex = rain[v + 1].ConstantIndex = rain[v + 2].ConstantIndex = rain[v + 3].ConstantIndex = (unsigned short)(i % 5);
        rain[v + 0].U1 = 0.0f; rain[v + 0].V1 = 0.0f;
        rain[v + 1].U1 = 0.0f; rain[v + 1].V1 = 1.0f;
        rain[v + 2].U1 = 1.0f; rain[v + 2].V1 = 1.0f;
        rain[v + 3].U1 = 1.0f; rain[v + 3].V1 = 0.0f;
    }
    RainVertexBuffer.Unlock();

    RainIndexBuffer.Uninitialise();
    RainIndexBuffer.Initialise(0x600, NEngineInternal::GPResourceManager->GetIndexBufferUsage(), INDEX_BUFFER_FORMAT_16, NEngineInternal::GPResourceManager->GetIndexBufferMemPool(), PT_TRIANGLE_LIST);
    unsigned short* index = (unsigned short*)RainIndexBuffer.Lock(0, 0, RainIndexBuffer.GetSize());
    for (long v = 0; v < 512; v += 4)
    {
        *index++ = (unsigned short)v;
        *index++ = (unsigned short)(v + 1);
        *index++ = (unsigned short)(v + 2);
        *index++ = (unsigned short)v;
        *index++ = (unsigned short)(v + 2);
        *index++ = (unsigned short)(v + 3);
    }
    RainIndexBuffer.Unlock();

    SnowVertexBuffer.Uninitialise();
    SnowVertexBuffer.Initialise(VERTEX_NULL, 1024, sizeof(CTVertexWeather), NEngineInternal::GPResourceManager->GetVertexBufferUsage(), NEngineInternal::GPResourceManager->GetVertexBufferMemPool());
    CTVertexWeather* snow = (CTVertexWeather*)SnowVertexBuffer.Lock(0, 0, SnowVertexBuffer.GetSize());
    for (long j = 0; j < 1024; ++j)
    {
        snow[j].X = GFFloatRandom(2.0f, &Settings.RandomNumberGeneratorSeed) - 1.0f;
        snow[j].Y = GFFloatRandom(2.0f, &Settings.RandomNumberGeneratorSeed) - 1.0f;
        snow[j].Z = GFFloatRandom(2.0f, &Settings.RandomNumberGeneratorSeed) - 1.0f;
        snow[j].ConstantIndex = (unsigned short)(j % 5);
        float u = GFFloatRandom(1.0f, &Settings.RandomNumberGeneratorSeed) - 0.5f;
        float v = GFFloatRandom(1.0f, &Settings.RandomNumberGeneratorSeed) - 0.5f;
        snow[j].SetU1V1(u, v);
    }
    SnowVertexBuffer.Unlock();

    MistVertexBuffer.Uninitialise();
    MistVertexBuffer.Initialise(VERTEX_NULL, 16, sizeof(CTVertexWeather), NEngineInternal::GPResourceManager->GetVertexBufferUsage(), NEngineInternal::GPResourceManager->GetVertexBufferMemPool());
    CTVertexWeather* mist = (CTVertexWeather*)MistVertexBuffer.Lock(0, 0, MistVertexBuffer.GetSize());
    for (long k = 0; k < 4; ++k)
    {
        mist->X = -1.0f; mist->Y = 1.0f; mist->Z = 1.0f; mist->U1 = 1.0f; mist->V1 = 0.0f; ++mist;
        mist->X = 1.0f; mist->Y = 1.0f; mist->Z = 1.0f; mist->U1 = 0.0f; mist->V1 = 0.0f; ++mist;
        mist->X = 1.0f; mist->Y = -1.0f; mist->Z = 1.0f; mist->U1 = 0.0f; mist->V1 = 1.0f; ++mist;
        mist->X = -1.0f; mist->Y = -1.0f; mist->Z = 1.0f; mist->U1 = 1.0f; mist->V1 = 1.0f; ++mist;
    }
    MistVertexBuffer.Unlock();

    MistIndexBuffer.Uninitialise();
    MistIndexBuffer.Initialise(0x30, NEngineInternal::GPResourceManager->GetIndexBufferUsage(), INDEX_BUFFER_FORMAT_16, NEngineInternal::GPResourceManager->GetIndexBufferMemPool(), PT_TRIANGLE_LIST);
    unsigned short* mistIndex = (unsigned short*)MistIndexBuffer.Lock(0, 0, MistIndexBuffer.GetSize());
    SetIndex(mistIndex, 0, 0);
    SetIndex(mistIndex, 1, 1);
    SetIndex(mistIndex, 2, 2);
    SetIndex(mistIndex, 3, 0);
    SetIndex(mistIndex, 4, 2);
    SetIndex(mistIndex, 5, 3);
    SetIndex(mistIndex, 6, 4);
    SetIndex(mistIndex, 7, 5);
    SetIndex(mistIndex, 8, 6);
    SetIndex(mistIndex, 9, 4);
    SetIndex(mistIndex, 10, 6);
    SetIndex(mistIndex, 11, 7);
    SetIndex(mistIndex, 12, 8);
    SetIndex(mistIndex, 13, 9);
    SetIndex(mistIndex, 14, 10);
    SetIndex(mistIndex, 15, 8);
    SetIndex(mistIndex, 16, 10);
    SetIndex(mistIndex, 17, 11);
    SetIndex(mistIndex, 18, 12);
    SetIndex(mistIndex, 19, 13);
    SetIndex(mistIndex, 20, 14);
    SetIndex(mistIndex, 21, 12);
    SetIndex(mistIndex, 22, 14);
    SetIndex(mistIndex, 23, 15);
    MistIndexBuffer.Unlock();
}
