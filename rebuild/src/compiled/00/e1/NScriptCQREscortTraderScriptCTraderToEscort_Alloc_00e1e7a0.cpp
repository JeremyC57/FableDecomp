// NScript::CQR_EscortTraderScript::CTraderToEscort::Alloc(CQR_EscortTraderScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "TraderToEscort" binding: new CTraderToEscort(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CTraderToEscort@CQR_EscortTraderScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1E7A0, 145 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CTraderToEscort@CQR_EscortTraderScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits.
//   CQR_EscortTraderScript::Main 0x00E1E110 (slot 2 of the script vtable 0x012E12E0, which the "QR_EscortTrader" script's Alloc stores at its CScriptInfoManager::RegisterScripts registration) stores this address as the factory of the CEntityScriptBinding<CQR_EscortTraderScript> record named "TraderToEscort" (site 0x00E1E22B).
//   Was landed as _global::Create_CVtable_012e151c (named by the entity vtable address); same shape as the family-pass F014 Alloc rows.
// Callees: scalar operator new 0x00BFEA1A (0x2C = sizeof(CTraderToEscort), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), and the count block's deleter
//   0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CTraderToEscort 0x012E151C.
// Layout: Ego_r.pdb CQR_EscortTraderScript::CTraderToEscort (44 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, lastRandomSpeechFollowIdx1 +0x1C, lastRandomSpeechFollowIdx2 +0x20, lastRandomSpeechWaitIdx1 +0x24, lastRandomSpeechWaitIdx2 +0x28.
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
class CQR_EscortTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CTraderToEscort : public CEntityScriptBase {
    public:
        CQR_EscortTraderScript* ParentClass;            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long lastRandomSpeechFollowIdx1;                // +0x1C
        long lastRandomSpeechFollowIdx2;                // +0x20
        long lastRandomSpeechWaitIdx1;                  // +0x24
        long lastRandomSpeechWaitIdx2;                  // +0x28
        CTraderToEscort(CQR_EscortTraderScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CTraderToEscort();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQR_EscortTraderScript& parent, CScriptDataBase* data, const CScriptThing& me);
    };
};

CCountedPointer<CEntityScriptBase> __fastcall CQR_EscortTraderScript::CTraderToEscort::Alloc(CQR_EscortTraderScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CTraderToEscort(parent, data, me));
}
}
