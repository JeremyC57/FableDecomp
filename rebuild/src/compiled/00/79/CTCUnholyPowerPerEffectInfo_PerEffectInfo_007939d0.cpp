// CTCUnholyPower::PerEffectInfo::PerEffectInfo() -- default constructor of the per-effect record: the 3D Position (+0) is left unset, the intelligent Effect thing pointer (+0xc) is default-constructed
// Leaf: PerEffectInfo
// Retail: Fable.exe (TLC, VC7.1) @ 0x007939D0, 25 bytes, __fastcall, plain `ret`.
// Ego_r.pdb: struct CTCUnholyPower::PerEffectInfo (20 bytes) { C3DVector Position; CIntelligentPointer<CThing> Effect; }
//   (vector<CTCUnholyPower::PerEffectInfo> instantiations name the owner; byte twin: CTCDivineWrath::PerEffectInfo 0x0078E3A0).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10; vtable 0x01238C6C (CIntelligentPointer<CThing>,
//   as in the landed CTargetAndThing_0078e700 constructor 0x0078E700).
// Evidence: the two callers (0x00795F3D, 0x00796118, the CTCUnholyPower module) construct it in a stack local and then copy three dwords into
//   +0..+8 (the Position). The manifest label "PerEffectInfo" agrees with the PDB.
// Shape: the member pointer is formed once in edi (`lea edi,[esi+0xc]`), the base constructor, then its vtable store;
//   C3DVector's empty default constructor emits nothing. No engine header exists for CTCUnholyPower; minimal local model.
class CThing;

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    CIntelligentPointerThing() : CBaseIntelligentPointer() { }
};

class CTCUnholyPower {
public:
    struct PerEffectInfo {
        C3DVector Position;                     // +0x00
        CIntelligentPointerThing Effect;        // +0x0c
        PerEffectInfo();
    };
};

CTCUnholyPower::PerEffectInfo::PerEffectInfo()
{
}