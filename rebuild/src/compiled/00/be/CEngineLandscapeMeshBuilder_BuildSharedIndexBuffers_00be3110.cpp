// CEngineLandscapeMeshBuilder::BuildSharedIndexBuffers() -- for every power-of-two patch size 2..16 x 2..16 build the index list into a temporary CArray, copy it into a new 16-bit triangle-strip index buffer and keep it in SharedIndexBuffers[x][y]
// Leaf: ?BuildSharedIndexBuffers@CEngineLandscapeMeshBuilder@@AAEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE3110, 386 bytes, thiscall, plain `ret`; 8-aligned frame (the C2DCoordI[289] local).
// Identity: Ego_r.pdb ?BuildSharedIndexBuffers@CEngineLandscapeMeshBuilder@@AAEXXZ (private); its one callee is BuildSharedIndexBufferGenerationTables 0x00BE2CD0
//   (Ego_r ?BuildSharedIndexBufferGenerationTables@CEngineLandscapeMeshBuilder@@QAEXPAVC2DCoordI@@PAG1JJAAJ@Z).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport vector storage), BuildSharedIndexBufferGenerationTables 0x00BE2CD0
//   (same-TU stand-in body below), operator new 0x00BFEA1A, CEngineResourceManager::GetIndexBufferUsage 0x00B3B060 /
//   GetIndexBufferMemPool 0x00B3B0A0, CIndexBuffer::CIndexBuffer 0x00AC2010 / Lock 0x00AC1FB0 / Unlock 0x00AC1F00; memcpy
//   intrinsic; g_pResourceManager 0x01436E98.
// Stand-in: BuildSharedIndexBufferGenerationTables is a same-TU noinline stand-in for the real retail callee (its writes through
//   the index pointer keep VC7.1 from dropping the vector's zero-fill stores); it is not part of this row.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A

#include "engine/CArray.h"

class C2DCoordI { public: long X, Y; };
enum EIndexBufferFormat { };
enum EIndexBufferPool { };
enum EPrimitiveType { };
class CIndexBuffer { public:
    CIndexBuffer(unsigned long size, unsigned long usage, EIndexBufferFormat format, EIndexBufferPool pool, EPrimitiveType type);   // 0x00AC2010
    unsigned char* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC1FB0
    void Unlock();                                                                        // 0x00AC1F00
    void* __vftable;
    unsigned char _pad04[0x10];
};
class CEngineResourceManager { public:
    unsigned long GetIndexBufferUsage();               // 0x00B3B060
    EIndexBufferPool GetIndexBufferMemPool();          // 0x00B3B0A0
};
extern CEngineResourceManager* g_pResourceManager;     // 0x01436E98

class CEngineLandscapeMeshBuilder { public:
    void*         __vftable;
    CIndexBuffer* SharedIndexBuffers[17][17];          // +0x04
    void BuildSharedIndexBufferGenerationTables(C2DCoordI* coords, unsigned short* indices, unsigned short* indices2, long sizeX, long sizeY, long& count);   // 0x00BE2CD0
private:
    void BuildSharedIndexBuffers();                     // Ego_r.pdb: private (AAE)
};

#pragma auto_inline(off)
__declspec(noinline) void CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables(C2DCoordI* coords, unsigned short* indices, unsigned short* indices2, long sizeX, long sizeY, long& count)
{
    indices[0] = 0;
    count = sizeX * sizeY;
}
#pragma auto_inline(on)

void CEngineLandscapeMeshBuilder::BuildSharedIndexBuffers()
{
    for (long x = 2; x <= 16; x *= 2) {
        for (long y = 2; y <= 16; y *= 2) {
            CArray<unsigned short> indices(x * y * 12);
            C2DCoordI coords[289];
            long count = 0;
            BuildSharedIndexBufferGenerationTables(coords, indices.GetAsCArray(), 0, x, y, count);
            CIndexBuffer* buffer = new CIndexBuffer(count * 2, g_pResourceManager->GetIndexBufferUsage(), (EIndexBufferFormat)0x65,
                                                    g_pResourceManager->GetIndexBufferMemPool(), (EPrimitiveType)5);
            unsigned char* data = buffer->Lock(0, 0, 0);
            memcpy(data, indices.GetAsCArray(), count * 2);
            buffer->Unlock();
            SharedIndexBuffers[x][y] = buffer;
        }
    }
}