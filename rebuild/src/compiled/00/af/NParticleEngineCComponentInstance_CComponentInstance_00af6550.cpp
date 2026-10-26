// NParticleEngine::CComponentInstance::CComponentInstance() -- CBase(), this class's vtable, an empty CCountedPointer and an `active` flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF6550, oracle length 30 bytes.
// Callees: CBase::CBase @ 0x0099A2D0 (landed). Vtable 0x0129F478 = NParticleEngine::CComponentInstance
// (rebuild/manifest/rtti-vtables.tsv); the manifest label "C3DMeshObject::C3DMeshObject" is a propagated
// name and is rejected (the C3DMeshObject vtable is 0x01239860, see 0x00A36E70).
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase::CBase ; xor eax,eax ; mov [esi],offset vftable ;
//                mov [esi+4],eax ; mov [esi+8],eax ; mov byte ptr [esi+0Ch],1 ; mov eax,esi ; pop esi ; ret
// Shape: the vtable store sits BEFORE the two zero stores at +4/+8. Two plain pointer members
// initialised in the mem-init list schedule after the vtable store (tried: DIFFER), so the pair is a
// sub-object with its own inline zeroing constructor -- the engine's CCountedPointer {object, count
// block}. Member names are descriptive (no PDB layout for this class in struct_layouts_egor.tsv).
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                               // 0x0099A2D0
}; } using N_0099a2d0::CBase;

struct CCountedPointer {
    void* m_pObject;                       // +0
    void* m_pCount;                        // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

namespace NParticleEngine {

class CComponentInstance : public CBase { public:
    virtual void v00();                    // this class has its own vtable (0x0129F478)
    CCountedPointer m_pDefinition;         // +0x4 .. +0x8
    bool            m_active;              // +0xC
    CComponentInstance();
};

CComponentInstance::CComponentInstance()
    : CBase(), m_pDefinition(), m_active(true)
{
}

}