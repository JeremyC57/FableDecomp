// NScript::CQ_TraderEscortScript::CRockTrollTrigger::Alloc(CQ_TraderEscortScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "RockTrollTrigger" binding: new CRockTrollTrigger(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CRockTrollTrigger@CQ_TraderEscortScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E03970, 145 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CRockTrollTrigger@CQ_TraderEscortScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits.
//   CQ_TraderEscortScript::Main 0x00E00DB0 (slot 2 of the script vtable 0x012DF16C, which the "Q_TraderEscort" script's Alloc stores at its CScriptInfoManager::RegisterScripts registration) stores this address as the factory of the CEntityScriptBinding<CQ_TraderEscortScript> record named "RockTrollTrigger" (site 0x00E0105C).
//   Was landed as _global::Create_CVtable_012df634 (named by the entity vtable address); same shape as the family-pass F014 Alloc rows.
// Callees: scalar operator new 0x00BFEA1A (0x20 = sizeof(CRockTrollTrigger), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), and the count block's deleter
//   0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CRockTrollTrigger 0x012DF634.
// Layout: Ego_r.pdb CQ_TraderEscortScript::CRockTrollTrigger (32 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, RockTrollSpawned +0x1C.
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
class CQ_TraderEscortScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CRockTrollTrigger : public CEntityScriptBase {
    public:
        CQ_TraderEscortScript* ParentClass;             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool RockTrollSpawned;                          // +0x1C
        CRockTrollTrigger(CQ_TraderEscortScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CRockTrollTrigger();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_TraderEscortScript& parent, CScriptDataBase* data, const CScriptThing& me);
    };
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_TraderEscortScript::CRockTrollTrigger::Alloc(CQ_TraderEscortScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CRockTrollTrigger(parent, data, me));
}
}
