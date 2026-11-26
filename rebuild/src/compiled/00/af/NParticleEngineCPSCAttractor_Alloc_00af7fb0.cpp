// NParticleEngine::CPSCAttractor::Alloc() -- the component factory: a new CPSCAttractor (operator new(0x30) 0x00BFEA1A, null-checked, then a tail jump into
//   the default constructor 0x00B18020), returned as its CPSCBase
// Leaf: ?Alloc@CPSCAttractor@NParticleEngine@@SIPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF7FB0, oracle length 24 bytes.
// Retail bytes:  push 0x30 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCAttractor::CPSCAttractor ; N: xor eax,eax ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CPSCAttractor@NParticleEngine@@SIPAVCPSCBase@2@XZ (public static __fastcall); retail references: the
//   registration table built by NParticleEngine::CPSCManager::RegisterPSCs 0x00AF8170 (whose landed row already binds this spelling here) and
//   the class's DatabaseClone 0x00B18740. The constructor is ??0CPSCAttractor@NParticleEngine@@QAE@XZ (Ego_r + FableWin), 0x00B18020, whose landed row defines it.
// Naming (NF460 / NF462, queue nf453 / nf458): was the generated free function CPSCAttractor_Alloc() returning a global-namespace CPSCAttractor*, which also
//   called the constructor outside NParticleEngine. Same bytes.
// Model (NF462, ruling r460d): a local model deriving from the shared engine/CPSCBase.h (CPSCAttractor has no header; ctor, Alloc and the ten
//   CPSCBase pure-virtual overrides declared, as engine/NParticleEngine_CPSCOrbit.h does), so the class is concrete and derives from CPSCBase: no cast.
#include "engine/CPSCBase.h"

namespace NParticleEngine {
// Local model (NF462, ruling r460d): CPSCAttractor has no shared header; it derives from the shared CPSCBase (RTTI: its ctor 0x00B18020 calls
// CPSCBase() 0x00AF2620 first, then stores vtable 0x012A04AC). Ego_r.pdb + FableWin.pdb name the ctor, Alloc and the ten CPSCBase overrides
// declared here; the class's own members (AttractorEnabled.. in Ego_r) are opaque. sizeof 0x30 (Alloc's operator new(0x30)).
class CPSCAttractor : public CPSCBase {
public:
    CPSCAttractor();                                    // 0x00B18020
    static CPSCBase* FABLE_FASTCALL Alloc();            // 0x00AF7FB0
    virtual CPSCBase* DatabaseClone() const;            // 0x00B18740
    virtual CCharString GetClassTypeName() const;
    virtual EPSCType GetComponentType() const;
    virtual EPSCSubType GetComponentSubType() const;
    virtual CCharString GetGUIName() const;
    virtual void InterpolateTo(const CPSCBase&,const float);
    virtual void ReadBinary(CDataInputStream&);
    virtual void WriteBinary(CDataOutputStream&) const;
    virtual void DatabaseCopy(const CPSCBase&);
    virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float);
private:
    unsigned char _members[0x30 - 0xc];                 // +0x0c own members (opaque)
};
}

namespace NParticleEngine {
CPSCBase* FABLE_FASTCALL CPSCAttractor::Alloc()
{
    return new CPSCAttractor;
}
}
