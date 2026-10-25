// CCreatureAction_SpiritAttack::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CCreatureAction_SpiritAttack inlined (destroying PParticleEmitter, PTarget, then ~CCreatureActionBase), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCCreatureAction_SpiritAttack@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00855100, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity: CCreatureAction_SpiritAttack -- proven; evidence: vtable 0x01275D74 slot 0 (RTTI .?AVCCreatureAction_SpiritAttack@@ : CCreatureActionBase); the two intelligent pointers are Ego_r.pdb PParticleEmitter (+0xB0) and PTarget (+0xA8) (PDB +188/+180 less the base's 12 container bytes), and 0x00693EF0 stores the CCreatureActionBase vtable.
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (twice), CCreatureActionBase::~CCreatureActionBase 0x00693EF0, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors, and every destructor between this class
//   and CCreatureActionBase is implicit (CCreatureActionBase's own one stores its vtable); member destructors run in reverse declaration order.
//   The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that
//   construction, nothing more.
// Model: local -- engine/CCreatureAction_SpiritAttack.h exists but is data-only (vptr flattened into a word), which cannot carry the virtual destructor chain.
class CThing;
class CBaseClass {                                // RTTI root, vtable 0x01231710
public:
    virtual ~CBaseClass();                        // 0x0099A2E0 (FableWin.pdb ??1CBaseClass@@UAE@XZ)
};
class CBaseClassNonCopyable : public CBaseClass { // vtable 0x0129A7C4
protected:
    CBaseClassNonCopyable();                      // 0x0099A2F0 (FableWin.pdb ??0CBaseClassNonCopyable@@IAE@XZ)
    virtual ~CBaseClassNonCopyable();             // 0x0099A300 (FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
#pragma pack(push, 1)
class CBaseIntelligentPointer {                   // 8 bytes, 1-aligned in retail (Coin::Thing +0x25, ShowTrophy::PEmitter +0xB5)
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();           // 0x00A01C10
    void* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)
class CBaseObject : public CBaseClass { };
class CCreatureActionBase : public CBaseObject {  // retail 0xA8 bytes (Ego_r.pdb 180: two vectors and a map 4 bytes larger each)
public:
    CCreatureActionBase();
    virtual ~CCreatureActionBase();               // 0x00693EF0
    unsigned char _base[0xA4];                    // PCreature .. DependentThings
};
class CCreatureAction_SpiritAttack : public CCreatureActionBase {
public:
    CIntelligentPointer<CThing> PTarget;          // +0xa8
    CIntelligentPointer<CThing> PParticleEmitter; // +0xb0
};
void* MakeOne()
{
    return new CCreatureAction_SpiritAttack;
}