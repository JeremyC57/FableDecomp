// CTCDivineWrath::PerEffectInfo::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~PerEffectInfo runs Effect's destructor (CIntelligentPointer<CThing> at +0xC), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GPerEffectInfo@CTCDivineWrath@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078E370, 33 bytes (+ int3 padding), thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: CTCDivineWrath::PerEffectInfo::`scalar deleting destructor' -- proven; evidence: FableWin.pdb name set holds ??_GPerEffectInfo@CTCDivineWrath@@QAEPAXI@Z (PerCreatureInfo of the same class mangles as a struct); Ego_r.pdb PerEffectInfo is {C3DVector Position, CIntelligentPointer<CThing> Effect +0xC}; the body sits in the CTCDivineWrath module (next to a PerEffectInfo constructor 0x0078E3A0, before the CTCDivineWrath methods 0x0078F4D0.. of vtable 0x0126BE84); its byte twin 0x007939A0 is the CTCUnholyPower one.
//   Family G042 (8 deleting destructors that differ in the one member's offset and destructor; the propagated label
//   "CLandscapeBackgroundPatch::`vector deleting destructor'" is wrong for all eight).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer() (the implicit ~CIntelligentPointer<CThing>) 0x00A01C10, operator delete 0x00BFE9BC.
// Shape: `delete p` on the class emits the non-virtual ??_G as its own COMDAT (the helper below is not graded).
void __cdecl operator delete(void* p);                         // 0x00BFE9BC
class C3DVector {                                            // 12 bytes
public:
    float X, Y, Z;
};
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CTCDivineWrath {
public:
    struct PerEffectInfo {
        C3DVector Position;                                 // +0x00
        CIntelligentPointer<CThing> Effect;                 // +0x0c
    };
};
void DeleteOne(CTCDivineWrath::PerEffectInfo* p) { delete p; }