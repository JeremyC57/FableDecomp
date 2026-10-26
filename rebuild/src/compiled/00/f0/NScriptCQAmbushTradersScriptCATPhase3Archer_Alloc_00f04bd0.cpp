// NScript::CQ_AmbushTradersScript::CAT_Phase3Archer::Alloc(CQ_AmbushTradersScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "AT_Phase3Archer" binding: new CAT_Phase3Archer(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CAT_Phase3Archer@CQ_AmbushTradersScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F04BD0, 145 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CAT_Phase3Archer@CQ_AmbushTradersScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits.
//   CQ_AmbushTradersScript::Main 0x00F024C0 (slot 2 of the script vtable 0x012C6CAC, which the "Q_AmbushTraders" script's Alloc stores at its CScriptInfoManager::RegisterScripts registration) stores this address as the factory of the CEntityScriptBinding<CQ_AmbushTradersScript> record named "AT_Phase3Archer" (site 0x00F02770).
//   Was landed as _global::Create_CVtable_012f7e4c (named by the entity vtable address); same shape as the family-pass F014 Alloc rows.
// Callees: scalar operator new 0x00BFEA1A (0x1C = sizeof(CAT_Phase3Archer), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), and the count block's deleter
//   0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CAT_Phase3Archer 0x012F7E4C.
// Layout: Ego_r.pdb CQ_AmbushTradersScript::CAT_Phase3Archer (28 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18.
//   CEntityScriptBase: vptr, Game +0x04, Me +0x08 (Ego_r 20 bytes). The parent's Game is CLevelScriptBase's +0x40.
// Shape: the PDB ctor (parent, data, me) inlined: base and ParentClass in the init list, PMasterData assigned in the body;
//   STLport-free counted pointer ctor: Data first, the null test on the argument, the 12-byte {1, deleter, Data} block
//   read back from Data, three exits (block, failed new, null object).
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
class CQ_AmbushTradersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CAT_Phase3Archer : public CEntityScriptBase {
    public:
        CQ_AmbushTradersScript* ParentClass;            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CAT_Phase3Archer(CQ_AmbushTradersScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CAT_Phase3Archer();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_AmbushTradersScript& parent, CScriptDataBase* data, const CScriptThing& me);
    };
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_AmbushTradersScript::CAT_Phase3Archer::Alloc(CQ_AmbushTradersScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CAT_Phase3Archer(parent, data, me));
}
}
