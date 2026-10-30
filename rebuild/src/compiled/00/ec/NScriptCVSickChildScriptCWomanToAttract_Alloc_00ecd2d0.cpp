// NScript::CV_SickChildScript::CWomanToAttract::Alloc(CV_SickChildScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "WomanToAttract" binding: new CWomanToAttract(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, the members' constructors, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CWomanToAttract@CV_SickChildScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ECD2D0, 153 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CWomanToAttract@CV_SickChildScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits (and the PDB ctor ??0CWomanToAttract@CV_SickChildScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z takes the same three arguments).
//   CV_SickChildScript::Main stores this address as the factory of a CEntityScriptBinding<CV_SickChildScript> named "WomanToAttract"
//   (site 0x00EC58BE, then CScriptBase::AddEntityScriptBinding 0x00CB8230); that Main 0x00EC54E0 is slot 2 of the script vtable 0x012F17DC, which the "V_SickChild" script's Alloc stores (CScriptInfoManager registration).
//   Family F014 (15 bodies, one per entity class): the manifest label "CTriangleList::CTriangleList" is propagated and
//   wrong; the landed twins of this shape (0x00D62360 etc.) are named by vtable address (Create_CVtable_...).
// Callees: scalar operator new 0x00BFEA1A (0x34 = sizeof(CWomanToAttract), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase() 0x007E72A0 (seh_me +0x24), and the
//   count block's deleter 0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CWomanToAttract 0x012F2404.
// Layout: Ego_r.pdb CV_SickChildScript::CWomanToAttract (52 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, GivenObject +0x1C, TimesMadeLaugh +0x20, seh_me +0x24.
//   CEntityScriptBase: vptr, Game +0x04, Me +0x08 (Ego_r 20 bytes). The parent's Game is CLevelScriptBase's +0x40.
// Shape: the PDB ctor (parent, data, me) inlined: base and ParentClass in the init list, PMasterData assigned in the body
//   (its store follows the member constructor call); STLport-free counted pointer ctor: Data first, the null test on the
//   argument, the 12-byte {1, deleter, Data} block read back from Data, three exits (block, failed new, null object).
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CScriptGameResourceObjectScriptedThingBase {      // 16 bytes (Ego_r): vptr, +4, PImp +8
public:
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    unsigned char _base04[4];
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
};

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_SickChildScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CWomanToAttract : public CEntityScriptBase {
    public:
        CV_SickChildScript* ParentClass;                // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool GivenObject;                               // +0x1C
        long TimesMadeLaugh;                            // +0x20
        CScriptGameResourceObjectScriptedThingBase seh_me;// +0x24
        CWomanToAttract(CV_SickChildScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CWomanToAttract();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SickChildScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_SickChildScript::CWomanToAttract::Alloc(CV_SickChildScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CWomanToAttract(parent, data, me));
}
}