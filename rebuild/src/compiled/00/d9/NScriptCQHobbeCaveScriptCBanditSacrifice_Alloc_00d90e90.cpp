// NScript::CQ_HobbeCaveScript::CBanditSacrifice::Alloc(CQ_HobbeCaveScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "BanditSacrifice" binding: new CBanditSacrifice(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, the members' constructors, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CBanditSacrifice@CQ_HobbeCaveScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D90E90, 153 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CBanditSacrifice@CQ_HobbeCaveScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits (and the PDB ctor ??0CBanditSacrifice@CQ_HobbeCaveScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z takes the same three arguments).
//   CQ_HobbeCaveScript::Main stores this address as the factory of a CEntityScriptBinding<CQ_HobbeCaveScript> named "BanditSacrifice"
//   (site 0x00D8E2E8, then CScriptBase::AddEntityScriptBinding 0x00CB8230); that Main 0x00D8DD10 is slot 2 of the script vtable 0x012D5540, which the "Q_HobbeCave" script's Alloc stores (CScriptInfoManager registration).
//   Family F014 (15 bodies, one per entity class): the manifest label "CTriangleList::CTriangleList" is propagated and
//   wrong; the landed twins of this shape (0x00D62360 etc.) are named by vtable address (Create_CVtable_...).
// Callees: scalar operator new 0x00BFEA1A (0x3C = sizeof(CBanditSacrifice), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), CScriptThing::CScriptThing() 0x006E7B40 (ImprisonedFX +0x28), and the
//   count block's deleter 0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CBanditSacrifice 0x012D5A94.
// Layout: Ego_r.pdb CQ_HobbeCaveScript::CBanditSacrifice (60 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, CurrentAIState +0x1C, PreviousAIState +0x20, BanditHealthID +0x24, ImprisonedFX +0x28, MadeMainChamberHobbesComment +0x34, FollowingCommentsSaid +0x38.
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

class CScriptThing {                                    // 12 bytes (Ego_r): vptr, PImp
public:
    virtual ~CScriptThing();
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    CScriptThing();                                     // 0x006E7B40 (base ctor 0x0099A2D0, vtable 0x01238C8C, PImp nulled)
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
};

class CEntityScriptBase {                               // 20 bytes (Ego_r)
public:
    CEntityScriptBase(CGameScriptInterfaceBase& game, const CScriptThing& me) : Game(&game), Me(me) { }
    virtual ~CEntityScriptBase();
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
};

class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData* MasterScriptData;                            // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HobbeCaveScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CBanditSacrifice : public CEntityScriptBase {
    public:
        CQ_HobbeCaveScript* ParentClass;                // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long CurrentAIState;                            // +0x1C
        long PreviousAIState;                           // +0x20
        long BanditHealthID;                            // +0x24
        CScriptThing ImprisonedFX;                      // +0x28 CScriptThing (Ego_r)
        bool MadeMainChamberHobbesComment;              // +0x34
        long FollowingCommentsSaid;                     // +0x38
        CBanditSacrifice(CQ_HobbeCaveScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CBanditSacrifice();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HobbeCaveScript& parent, CScriptDataBase* data, const CScriptThing& me);
    };
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_HobbeCaveScript::CBanditSacrifice::Alloc(CQ_HobbeCaveScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CBanditSacrifice(parent, data, me));
}
}