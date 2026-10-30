// NScript::CV_SingingStonesScript::CSingingStone::Alloc(CV_SingingStonesScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "SingingStone" binding: new CSingingStone(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, the members' constructors, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CSingingStone@CV_SingingStonesScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED1420, 153 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CSingingStone@CV_SingingStonesScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits (and the PDB ctor ??0CSingingStone@CV_SingingStonesScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z takes the same three arguments).
//   CV_SingingStonesScript::Main stores this address as the factory of a CEntityScriptBinding<CV_SingingStonesScript> named "SingingStone"
//   (site 0x00ED1187, then CScriptBase::AddEntityScriptBinding 0x00CB8230); that Main 0x00ED10A0 is slot 2 of the script vtable 0x012F2B0C, which the "V_SingingStones" script's Alloc stores (CScriptInfoManager registration).
//   Family F014 (15 bodies, one per entity class): the manifest label "CTriangleList::CTriangleList" is propagated and
//   wrong; the landed twins of this shape (0x00D62360 etc.) are named by vtable address (Create_CVtable_...).
// Callees: scalar operator new 0x00BFEA1A (0x2C = sizeof(CSingingStone), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), CScriptThing::CScriptThing() 0x006E7B40 (MyParticleEmitter +0x20), and the
//   count block's deleter 0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CSingingStone 0x012F2C1C.
// Layout: Ego_r.pdb CV_SingingStonesScript::CSingingStone (44 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, MyStoneNumber +0x1C, MyParticleEmitter +0x20.
//   CEntityScriptBase: vptr, Game +0x04, Me +0x08 (Ego_r 20 bytes). The parent's Game is CLevelScriptBase's +0x40.
// Shape: the PDB ctor (parent, data, me) inlined: base and ParentClass in the init list, PMasterData assigned in the body
//   (its store follows the member constructor call); STLport-free counted pointer ctor: Data first, the null test on the
//   argument, the 12-byte {1, deleter, Data} block read back from Data, three exits (block, failed new, null object).
class CGameScriptInterfaceBase;
class CScriptDataBase;

void __fastcall Destroy_00cdee00(void* object);        // 0x00CDEE00 (catalog name; the CEntityScriptBase deleter)

class CCPPointerInfo {                                  // 12 bytes
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
    CCPPointerInfo(void* data, void (__fastcall* deleteFunc)(void*)) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};

template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(Data, Destroy_00cdee00);
        else
            Info = 0;
    }
};

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_SingingStonesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CSingingStone : public CEntityScriptBase {
    public:
        CV_SingingStonesScript* ParentClass;            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        unsigned long MyStoneNumber;                    // +0x1C
        CScriptThing MyParticleEmitter;                 // +0x20 CScriptThing (Ego_r)
        CSingingStone(CV_SingingStonesScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CSingingStone();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SingingStonesScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_SingingStonesScript::CSingingStone::Alloc(CV_SingingStonesScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CSingingStone(parent, data, me));
}
}