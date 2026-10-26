// NScript::CQ_NewOakValeIntroScript::CNOVI_Bully::Alloc(CQ_NewOakValeIntroScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "NOVI_Bully" binding: new CNOVI_Bully(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CNOVI_Bully@CQ_NewOakValeIntroScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DAEC60, 145 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CNOVI_Bully@CQ_NewOakValeIntroScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits.
//   CQ_NewOakValeIntroScript::Main 0x00DABAC0 (slot 2 of the script vtable 0x012D7A28, which the "Q_NewOakValeIntro" script's Alloc stores at its CScriptInfoManager::RegisterScripts registration) stores this address as the factory of the CEntityScriptBinding<CQ_NewOakValeIntroScript> record named "NOVI_Bully" (site 0x00DABC9E).
//   Was landed as _global::Create_CVtable_012d879c (named by the entity vtable address); same shape as the family-pass F014 Alloc rows.
// Callees: scalar operator new 0x00BFEA1A (0x2C = sizeof(CNOVI_Bully), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), and the count block's deleter
//   0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CNOVI_Bully 0x012D879C.
// Layout: Ego_r.pdb CQ_NewOakValeIntroScript::CNOVI_Bully (44 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, InitialHealth +0x1C, HitsTaken +0x20, DoneIntro +0x24, SaidPieceAboutAttackingVictim +0x25, SpokenOnFirstProximity +0x26, IntimidateSpeechLoop +0x28.
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
class CQ_NewOakValeIntroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CNOVI_Bully : public CEntityScriptBase {
    public:
        CQ_NewOakValeIntroScript* ParentClass;          // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long InitialHealth;                             // +0x1C
        long HitsTaken;                                 // +0x20
        bool DoneIntro;                                 // +0x24
        bool SaidPieceAboutAttackingVictim;             // +0x25
        bool SpokenOnFirstProximity;                    // +0x26
        long IntimidateSpeechLoop;                      // +0x28
        CNOVI_Bully(CQ_NewOakValeIntroScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CNOVI_Bully();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_NewOakValeIntroScript& parent, CScriptDataBase* data, const CScriptThing& me);
    };
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_NewOakValeIntroScript::CNOVI_Bully::Alloc(CQ_NewOakValeIntroScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CNOVI_Bully(parent, data, me));
}
}
