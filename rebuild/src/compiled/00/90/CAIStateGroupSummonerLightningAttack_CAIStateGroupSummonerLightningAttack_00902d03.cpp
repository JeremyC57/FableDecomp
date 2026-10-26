// CAIStateGroup_SummonerLightningAttack::CAIStateGroup_SummonerLightningAttack() -- default constructor: base, target pointer, null def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00902D03, length 40 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase() @ 0x008FD255, CBaseIntelligentPointer::
// CBaseIntelligentPointer() @ 0x00A01B10 (the PTarget member at +0x14, engine header
// CAIStateGroup_SummonerLightningAttack.h: CIntelligentPointer<CThing>, whose own vtable 0x01238C6C is
// stored after the base call), this class's vtable 0x0128D2A0 (rtti-vtables.tsv).
// Shape: base constructor, this vtable, the member's inline constructor (base call + vtable), then
// PDef's inline null store as `and dword ptr [esi+1Ch],0` -- the size-optimised zero store, hence
// `#pragma optimize("s", on)`. NextFrameForPreemptiveUse (+0x20) is left uninitialised.
#pragma optimize("s", on)
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v01();                         // own vtable 0x01238C6C
    CIntelligentPointer() : CBaseIntelligentPointer() {}
};

template <class T>
class CDefPointer { public:
    T* Object;                                  // +0x00
    CDefPointer() : Object(0) {}
};

class CSummonerDef;

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // PBrain, ScriptAIPriority, Name, flags (+0x4..+0x13)
    CAIStateGroupBase();                        // 0x008FD255
};

struct CAIStateGroup_SummonerLightningAttack : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTarget;        // +0x14
    CDefPointer<const CSummonerDef> PDef;       // +0x1c
    long NextFrameForPreemptiveUse;                     // +0x20
    virtual void v01();                         // own vtable 0x0128D2A0
    CAIStateGroup_SummonerLightningAttack();
};

CAIStateGroup_SummonerLightningAttack::CAIStateGroup_SummonerLightningAttack()
    : CAIStateGroupBase(), PTarget(), PDef()
{
}