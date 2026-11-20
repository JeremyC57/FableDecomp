// CEngineDynamic3DResource::CEngineDynamic3DResource() -- base CIndexBuffer(), this vtable, then 1 members zeroed
// Leaf: ??0CEngineDynamic3DResource@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF3290, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CIndexBuffer::CIndexBuffer ; mov dword ptr [esi],offset CEngineDynamic3DResource::`vftable' ; mov [esi+0x20],0 ; mov eax,esi ; pop esi ; ret
// Default constructor: base CIndexBuffer() @ 0x00AC1EC0, this class's vtable (0x012A81B8, named
// through rtti-vtables.tsv), then the 1 consecutive dword members at +0x20..+0x20
// are zeroed. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 10 bodies.
// agentNF182 (2026-11-20, q4554): the base is the shared engine/CIndexBuffer.h class (RTTI .?AV?$CEngineDynamic3DResource@VCIndexBuffer@@$02@@
//   : CIndexBuffer : CMovableResource, ghidra_out/rtti_bases.tsv); its out-of-line default ctor 0x00AC1EC0 binds ??0CIndexBuffer@@QAE@XZ
//   (Ego_r.pdb + FableWin.pdb). Was the local stand-in CBase_00ac1ec0. The derived class keeps its local (header-less) stand-in name.
#define FABLE_RETAIL_CINDEXBUFFER
#include "engine/CIndexBuffer.h"                // CIndexBuffer : CMovableResource, sizeof 0x14

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x20
    SZeroed() : m_00(0) { }           // inlined into every constructor that embeds it
};

struct CEngineDynamic3DResource : CIndexBuffer {
    unsigned char _pad[0x0c];
    SZeroed m_data;                        // +0x20
    CEngineDynamic3DResource();
};

CEngineDynamic3DResource::CEngineDynamic3DResource()
    : CIndexBuffer()
    , m_data()
{
}