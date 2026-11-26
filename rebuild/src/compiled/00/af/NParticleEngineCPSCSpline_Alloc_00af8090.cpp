// NParticleEngine::CPSCSpline::Alloc() -- the component factory: a new CPSCSpline (operator new(0x40) 0x00BFEA1A, null-checked, then a tail jump into
//   the default constructor 0x00B1B350), returned as its CPSCBase
// Leaf: ?Alloc@CPSCSpline@NParticleEngine@@SIPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8090, oracle length 24 bytes.
// Retail bytes:  push 0x40 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCSpline::CPSCSpline ; N: xor eax,eax ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CPSCSpline@NParticleEngine@@SIPAVCPSCBase@2@XZ (public static __fastcall); retail references: the
//   registration table built by NParticleEngine::CPSCManager::RegisterPSCs 0x00AF8170 (whose landed row already binds this spelling here) and
//   the class's DatabaseClone 0x00B1B4B0. The constructor is ??0CPSCSpline@NParticleEngine@@QAE@XZ (Ego_r + FableWin), 0x00B1B350, whose landed row defines it.
// Naming (NF460 / NF462, queue nf453 / nf458): was the generated free function CPSCSpline_Alloc() returning a global-namespace CPSCSpline*, which also
//   called the constructor outside NParticleEngine. Same bytes.
// Model (NF462, ruling r460d): a local model deriving from the shared engine/CPSCSplineBase.h (containers-free view; CPSCSpline has no header; ctor, Alloc and the ten
//   CPSCBase pure-virtual overrides declared, as engine/NParticleEngine_CPSCOrbit.h does), so the class is concrete and derives from CPSCBase: no cast.
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSPLINEBASE_NO_CONTAINERS
#include "engine/CPSCSplineBase.h"

namespace NParticleEngine {
// Local model (NF462, ruling r460d): CPSCSpline has no shared header; it derives from the shared CPSCSplineBase (its ctor 0x00B1B350 calls
// CPSCSplineBase() 0x00B21530 first, then stores vtable 0x012A0934), on that header's containers-free view (r450a / r451a). Ego_r.pdb + FableWin.pdb name the ctor, Alloc and the ten CPSCBase overrides
// declared here; the class's own members are opaque. sizeof 0x40 (Alloc's operator new(0x40)).
class CPSCSpline : public CPSCSplineBase {
public:
    CPSCSpline();                                       // 0x00B1B350
    static CPSCBase* FABLE_FASTCALL Alloc();            // 0x00AF8090
    virtual CPSCBase* DatabaseClone() const;            // 0x00B1B4B0
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
    unsigned char _members[0x40 - 0x34];                // +0x34 own members (opaque)
};
}

namespace NParticleEngine {
CPSCBase* FABLE_FASTCALL CPSCSpline::Alloc()
{
    return new CPSCSpline;
}
}
