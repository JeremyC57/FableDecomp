// CTCDivineWrath::PerEffectInfo::PerEffectInfo() -- default constructor of the per-effect record: the 3D Position (+0) is left unset, the intelligent Effect thing pointer (+0xc) is default-constructed
// Leaf: PerEffectInfo
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078E3A0, 25 bytes, __fastcall, plain `ret`.
// Ego_r.pdb: struct CTCDivineWrath::PerEffectInfo (20 bytes) { C3DVector Position; CIntelligentPointer<CThing> Effect; }
//   (vector<CTCDivineWrath::PerEffectInfo> instantiations name the owner; byte twin: CTCUnholyPower::PerEffectInfo 0x007939D0).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10; vtable 0x01238C6C (CIntelligentPointer<CThing>,
//   as in the landed CTargetAndThing_0078e700 constructor 0x0078E700).
// Evidence: the two callers (0x0079099D, 0x00790B78, the CTCDivineWrath module) construct it in a stack local and then copy three dwords into
//   +0..+8 (the Position). The manifest label "PerEffectInfo" agrees with the PDB.
// Shape: the member pointer is formed once in edi (`lea edi,[esi+0xc]`), the base constructor, then its vtable store;
//   C3DVector's empty default constructor emits nothing. No engine header exists for CTCDivineWrath; minimal local model.
class CThing;

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCDivineWrath {
public:
    struct PerEffectInfo {
        C3DVector Position;                     // +0x00
        CIntelligentPointer<CThing> Effect;        // +0x0c
        PerEffectInfo();
    };
};

CTCDivineWrath::PerEffectInfo::PerEffectInfo()
{
}