// CCreatureAction_ShowToSheatheTrophy::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CCreatureAction_ShowToSheatheTrophy inlined (destroying the inherited CCreatureAction_ShowTrophy::PEmitter and PTrophyBeingShown, then ~CCreatureActionBase), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCCreatureAction_ShowToSheatheTrophy@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DF8C0, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity: CCreatureAction_ShowToSheatheTrophy -- proven; evidence: vtable 0x0125F9F4 slot 0 (RTTI .?AVCCreatureAction_ShowToSheatheTrophy@@ : CCreatureAction_ShowTrophy : CCreatureActionBase); Ego_r.pdb gives ShowToSheatheTrophy no members of its own, so the two intelligent pointers destroyed are CCreatureAction_ShowTrophy::PEmitter (+0xB5, packed after the PostedDeed bool) and PTrophyBeingShown (+0xA8) through its inlined implicit destructor; 0x00693EF0 stores the CCreatureActionBase vtable 0x01259D24.
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (twice: the implicit ~CIntelligentPointer<CThing>), CCreatureActionBase::~CCreatureActionBase 0x00693EF0, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors, and every destructor between this class
//   and CCreatureActionBase is implicit (CCreatureActionBase's own one stores its vtable); member destructors run in reverse declaration order.
//   The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that
//   construction. KillTrophy / KillOne's qualified explicit destructor calls make VC7.1 define both implicit destructors
//   before it generates the vtable's deleting destructor, which then inlines the two-level chain (without them it calls
//   the implicit ~CCreatureAction_ShowTrophy out of line: 30 bytes against 48). Same fix as G032 (0x009C6F40).
// Model: local -- no engine header for this class; the bases are sized to the retail layout.
class CThing;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { };
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
class CCreatureAction_ShowTrophy : public CCreatureActionBase {
public:
    CIntelligentPointer<CThing> PTrophyBeingShown;  // +0xa8
    unsigned long ShowingTrophyDeedHandle;          // +0xb0
    bool PostedDeed;                                // +0xb4
    CIntelligentPointer<CThing> PEmitter;           // +0xb5
};
class CCreatureAction_ShowToSheatheTrophy : public CCreatureAction_ShowTrophy {
};
void* MakeOne()
{
    return new CCreatureAction_ShowToSheatheTrophy;
}
void KillTrophy(CCreatureAction_ShowTrophy* p) { p->CCreatureAction_ShowTrophy::~CCreatureAction_ShowTrophy(); }
void KillOne(CCreatureAction_ShowToSheatheTrophy* p) { p->CCreatureAction_ShowToSheatheTrophy::~CCreatureAction_ShowToSheatheTrophy(); }