// CAggregate_005bc216::~CAggregate_005bc216() -- implicit destructor: members at +0x8c, +0x80 destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC216, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x8c] ; call CMember_004ac8f0::~CMember_004ac8f0 ; lea ecx,[esi+0x80] ; call vector<CEngineTextureMorph>::~vector ; mov ecx,esi ; pop esi ; jmp CMember_00550a00::~CMember_00550a00
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x00550A00) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~CSymbolInfo>_>`) is propagated. One of 19 bodies of this shape.
// Name fix (agentNF54, nf44:5): the local CMember_005bc0d3 stand-in is gone: the member is the shared std::vector<CEngineTextureMorph>
//   (engine/CEngineTextureMorph.h), whose destructor stays the out-of-line COMDAT (FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE) and binds
//   FableWin.pdb ??1?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAE@XZ (retail 0x005BC0D3).
// Name fix (NF111, q4914): the +0 member is the shared engine/CEnginePrimitive2DAnimatingMesh.h class (sizeof 0x80; the local
//   CMember_00550a00 stand-in is gone): its out-of-line destructor ??1CEnginePrimitive2DAnimatingMesh@@QAE@XZ is 0x00550A00 (Ego_r.pdb /
//   FableWin.pdb). The aggregate is Ego_r.pdb CEnginePrimitive2DMorphedAnimatingMesh (that class as base +0, TextureMorphs +0x80,
//   HiddenMaterialNames +0x8C): this row's own name stays the pre-existing stand-in (NAME_FIXES.tsv: no shared header for that class).
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "engine/CEngineTextureMorph.h"
#include "engine/CEnginePrimitive2DAnimatingMesh.h"
struct CMember_004ac8f0 {
    unsigned char _body[0xc];
    ~CMember_004ac8f0();                            // 0x004AC8F0
};

struct CAggregate_005bc216 {
    CEnginePrimitive2DAnimatingMesh m_first;            // +0 (0x80 bytes; its destructor 0x00550A00 is the tail call)
    std::vector<CEngineTextureMorph> m_80;            // +0x80
    CMember_004ac8f0 m_8c;            // +0x8c
    ~CAggregate_005bc216();
};

CAggregate_005bc216::~CAggregate_005bc216()
{
}