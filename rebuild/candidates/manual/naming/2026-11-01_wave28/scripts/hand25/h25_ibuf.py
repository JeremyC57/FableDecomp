# h25_ibuf.py: naming wave 25, family ibuf -- the binders of ?Uninitialise@CIndexBuffer@@QAEXXZ 0x00AC1EE0 and
#  ?Uninitialise@CVertexBufferWin32@@QAEXXZ 0x00A63200 (c692 queue; Ego_r.pdb members; each class's dtor is `{ Uninitialise(); }`).
#  0x00AC2070 ??_GCIndexBuffer: Release -> Uninitialise.
#  0x00BC5420 ~CEnginePrimitiveManagerParticleLineGroup: the stand-ins CIndexBuffer_Retail (Release) / CVertexBufferWin32_Retail /
#    CMovableResource_Retail become the PDB classes (their vtables 0x0129EF2C / 0x0129BA20 / 0x0129B860 are already bound as
#    ??_7CIndexBuffer / ??_7CVertexBufferWin32 / ??_7CMovableResource by 0x00AC2010 / 0x00BB63A0 / 0x009D5110); the engine layout headers
#    are included under a layout alias (the 0x00BC3E10 precedent). The row's own class CEnginePrimitiveManagerParticleLineGroup_Retail is
#    the PDB class (Ego_r ??1CEnginePrimitiveManagerParticleLineGroup@@UAE@XZ), likewise aliased.
#  0x00B6DB80 CEngineWaterRenderer::CloseStaticMap (no Leaf line): the stand-ins CIndexBufferNative / CVertexBufferNative become the PDB
#    classes, and the row's class CWaterStaticMap the PDB CEngineWaterRenderer (Ego_r ?CloseStaticMap@CEngineWaterRenderer@@QAEXXZ; the
#    catalog names it); retail-layout model kept local as 0x00B6D480 does (the header's HaveBackgroundSea / LoadingSea offsets differ).
#  0x00B6D480 CEngineWaterRenderer::ReleaseSeaBuffers: CIndexBuffer_00b6d480 -> CIndexBuffer.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
A = '00ac2070'
he.edit(A, 'src', '''    void Release();                                // 0x00AC1EE0 (labelled "CVolume::ReleaseVolume")
    __forceinline virtual ~CIndexBuffer()
    {
        Release();
    }''', '''    void Uninitialise();                           // 0x00AC1EE0 (Ego_r.pdb ?Uninitialise@CIndexBuffer@@QAEXXZ; naming wave 25, was Release)
    __forceinline virtual ~CIndexBuffer()
    {
        Uninitialise();
    }''')
A = '00bc5420'
he.edit(A, 'src', '''Calls00ac1ee0 Release,00a63200 Uninitialise,0099a300 base dtor,00bfea14 free.
// Leaf: ??1CEnginePrimitiveManagerParticleLineGroup_Retail
''', '''Calls00ac1ee0 Uninitialise,00a63200 Uninitialise,0099a300 base dtor,00bfea14 free.
// Leaf: ??1CEnginePrimitiveManagerParticleLineGroup@@UAE@XZ
// Naming wave 25 (c692 queue): CIndexBuffer::Uninitialise 0x00AC1EE0 / CVertexBufferWin32::Uninitialise 0x00A63200 (Ego_r.pdb) on the PDB
//   classes CIndexBuffer / CVertexBufferWin32 / CMovableResource (were the stand-ins CIndexBuffer_Retail::Release, CVertexBufferWin32_Retail,
//   CMovableResource_Retail; their vtables are the ones 0x00AC2010 / 0x00BB63A0 / 0x009D5110 bind), and the row's own class is the PDB
//   CEnginePrimitiveManagerParticleLineGroup (was ..._Retail); the engine layout headers are included under a layout alias.
''')
he.edit(A, 'src', '''#include "engine/CEnginePrimitiveManagerParticleLineGroup.h"
#include "engine/CIndexBuffer.h"
#include "engine/CVertexBufferWin32.h"
''', '''#define CEnginePrimitiveManagerParticleLineGroup CEnginePrimitiveManagerParticleLineGroupLayout
#define CIndexBuffer CIndexBufferLayout
#define CVertexBufferWin32 CVertexBufferWin32Layout
#include "engine/CEnginePrimitiveManagerParticleLineGroup.h"
#include "engine/CIndexBuffer.h"
#include "engine/CVertexBufferWin32.h"
#undef CEnginePrimitiveManagerParticleLineGroup
#undef CIndexBuffer
#undef CVertexBufferWin32
''')
he.edit(A, 'src', '''struct CMovableResource_Retail { virtual ~CMovableResource_Retail(){} };
struct CIndexBuffer_Retail : CMovableResource_Retail { void* PD3DIndexBuffer;unsigned long ByteLength;unsigned long PrimitiveType;void* PAllocatedBuffer; void Release(); __forceinline ~CIndexBuffer_Retail(){Release();} };''',
'''class CMovableResource { public: virtual ~CMovableResource(){} };
class CIndexBuffer : public CMovableResource { public: void* PD3DIndexBuffer;unsigned long ByteLength;unsigned long PrimitiveType;void* PAllocatedBuffer; void Uninitialise(); __forceinline ~CIndexBuffer(){Uninitialise();} };''')
he.edit(A, 'src', '''struct CVertexBufferWin32_Retail : CBaseClassNonCopyable { void* PVertexBuffer;unsigned long Length;long Type;unsigned char Stride;CVertexCompressionParams_Retail CompressionParams; void Uninitialise(); __forceinline ~CVertexBufferWin32_Retail(){Uninitialise();} };''',
'''class CVertexBufferWin32 : public CBaseClassNonCopyable { public: void* PVertexBuffer;unsigned long Length;long Type;unsigned char Stride;CVertexCompressionParams_Retail CompressionParams; void Uninitialise(); __forceinline ~CVertexBufferWin32(){Uninitialise();} };''')
he.edit(A, 'src', '''struct __declspec(novtable) CEnginePrimitiveManagerParticleLineGroup_Retail : CBaseClassNonCopyable {''',
        '''class __declspec(novtable) CEnginePrimitiveManagerParticleLineGroup : public CBaseClassNonCopyable { public:''')
he.edit(A, 'src', ''' CVertexBufferWin32_Retail VertexBuffer;
 CIndexBuffer_Retail IndexBuffer;''', ''' CVertexBufferWin32 VertexBuffer;
 CIndexBuffer IndexBuffer;''')
he.edit(A, 'src', ''' ~CEnginePrimitiveManagerParticleLineGroup_Retail();
};
CEnginePrimitiveManagerParticleLineGroup_Retail::~CEnginePrimitiveManagerParticleLineGroup_Retail(){}''', ''' ~CEnginePrimitiveManagerParticleLineGroup();
};
CEnginePrimitiveManagerParticleLineGroup::~CEnginePrimitiveManagerParticleLineGroup(){}''')
he.edit(A, 'src', '''FABLE_STATIC_ASSERT(sizeof(CIndexBuffer_Retail)==sizeof(CIndexBuffer));
FABLE_STATIC_ASSERT(sizeof(CVertexBufferWin32_Retail)==sizeof(CVertexBufferWin32));''', '''FABLE_STATIC_ASSERT(sizeof(CIndexBuffer)==sizeof(CIndexBufferLayout));
FABLE_STATIC_ASSERT(sizeof(CVertexBufferWin32)==sizeof(CVertexBufferWin32Layout));''')
A = '00b6db80'
he.edit(A, 'src', '''// CEngineWaterRenderer::CloseStaticMap -- release sea geometry, discard async data and release loading pointers.
''', '''// CEngineWaterRenderer::CloseStaticMap -- release sea geometry, discard async data and release loading pointers.
// Leaf: ?CloseStaticMap@CEngineWaterRenderer@@QAEXXZ
// Naming wave 25 (c692 queue): CIndexBuffer::Uninitialise 0x00AC1EE0 / CVertexBufferWin32::Uninitialise 0x00A63200 (Ego_r.pdb) on the PDB
//   classes (were the stand-ins CIndexBufferNative / CVertexBufferNative); the row's class is the PDB CEngineWaterRenderer (Ego_r
//   ?CloseStaticMap@CEngineWaterRenderer@@QAEXXZ; was a local CWaterStaticMap; the retail layout stays local, as 0x00B6D480 models it);
//   the engine layout headers are included under a layout alias. The Leaf line is new (the row had none).
''')
he.edit(A, 'src', '''#include "engine/CEngineWaterRenderer.h"
#include "engine/CIndexBuffer.h"
#include "engine/CVertexBufferWin32.h"
#else
#define __fastcall
class CIndexBuffer { public:unsigned char State[0x14];};
class CVertexBufferWin32 { public:unsigned char State[0x34];};
#define __forceinline inline
#endif''', '''#define CEngineWaterRenderer CEngineWaterRendererLayout
#define CIndexBuffer CIndexBufferLayout
#define CVertexBufferWin32 CVertexBufferWin32Layout
#include "engine/CEngineWaterRenderer.h"
#include "engine/CIndexBuffer.h"
#include "engine/CVertexBufferWin32.h"
#undef CEngineWaterRenderer
#undef CIndexBuffer
#undef CVertexBufferWin32
#else
#define __fastcall
#define __forceinline inline
#endif''')
he.edit(A, 'src', '''struct CVertexBufferNative:CVertexBufferWin32 {void Uninitialise();}; //00a63200
struct CIndexBufferNative:CIndexBuffer {void Uninitialise();}; //00ac1ee0
struct CWaterStaticMap {
 unsigned char Foreground[0x68];CIndexBufferNative SeaBackgroundIndexBuffer;CVertexBufferNative SeaBackgroundVertexBuffer;''',
'''class CVertexBufferWin32 { public:unsigned char State[0x34];void Uninitialise();}; //00a63200
class CIndexBuffer { public:unsigned char State[0x14];void Uninitialise();}; //00ac1ee0
class CEngineWaterRenderer { public:
 unsigned char Foreground[0x68];CIndexBuffer SeaBackgroundIndexBuffer;CVertexBufferWin32 SeaBackgroundVertexBuffer;''')
he.edit(A, 'src', '''void CWaterStaticMap::CloseStaticMap(){''', '''void CEngineWaterRenderer::CloseStaticMap(){''')
A = '00b6d480'
he.edit(A, 'src', '''struct CIndexBuffer_00b6d480 {
    unsigned char _body[0x14];
    void Uninitialise();                           // 0x00AC1EE0
};''', '''class CIndexBuffer { public:                       // naming wave 25: the PDB class (was a CIndexBuffer_00b6d480 stand-in)
    unsigned char _body[0x14];
    void Uninitialise();                           // 0x00AC1EE0 (Ego_r.pdb ?Uninitialise@CIndexBuffer@@QAEXXZ)
};''')
he.edit(A, 'src', '''    CIndexBuffer_00b6d480       SeaBackgroundIndexBuffer;     // +0x68''', '''    CIndexBuffer                SeaBackgroundIndexBuffer;     // +0x68''')
