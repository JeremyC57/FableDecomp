// NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::Alloc(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script ClassAllocFunc: new NScript::CArcheryCompetitionSignpost wrapped in a CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDEB40, 98 bytes, `__fastcall` returning a CCountedPointer by value
//   (hidden result in ecx, the parent script in edx, the master data and the thing on the stack, `ret 8`).
//   It is the ClassAllocFunc stored at +0x10 of the CEntityScriptBinding that 0x00CDE9A3 registers under
//   "ArcheryCompetitionSignpost" (AddEntityScriptBinding @ 0x00CB8230). The catalog name
//   "CFileInstaller_CFileInfo_CreateInstance" is an autoname; the creator's C++ name is not recovered.
// Callees: scalar operator new @ 0x00BFEA1A (0x1C bytes), CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90
//   (Me, +8), CCountedPointer<CEntityScriptBase>::CCountedPointer(CEntityScriptBase*) @ 0x00CDEDB0 (out of line in
//   this TU: count block {1, deleter 0x00CDEE00, object}).
// Data: CEntityScriptBase's vtable @ 0x012C3224, CArcheryCompetitionSignpost's vtable @ 0x012C3208.
// Layout = Ego_r.pdb CArcheryCompetitionSignpost (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
//   ParentClass +0x14 (CPersonalScript_GlobalThingsScript*), PMasterData +0x18 (CQ_SunnyvaleMasterData*).
// Shape: speed TU (no pragma). Game (= parent's +0x40) is stored before the base vtable store, so it lives in an
//   inlined root whose own vtable store is dead (the landed 0x00D69E30 factory's model); a null `new` passes 0 to
//   the counted-pointer ctor on a separate tail.
// Naming wave 35: identity reg 00cde9a3 "ArcheryCompetitionSignpost" in 00cde420; Alloc 00cdeb40; vtable 012c3208; PDB ?Alloc@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@SI...; refs=1 (registration sites); Anniversary RTTI agrees (high); replaces _global::CreateArcheryCompetitionSignpost: PDB has no such member (wave3/labels_alloc.tsv); PDB ?Alloc@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z in Ego_r.pdb / FableWin.pdb.
//   Was the free __fastcall `?CreateArcheryCompetitionSignpost@@YI?AV?$CCountedPointer@VCEntityScriptBase@@@@PAVCPersonalScript_GlobalThingsScript@@PAVCScriptDataBase@@ABVCScriptThing@@@Z`: now the PDB static member (the parent by reference, same pointer ABI); the row's outer
//   stand-in is NScript::CPersonalScript_GlobalThingsScript with the entity nested in it; same bytes.
class CGameScriptInterfaceBase;
class CScriptDataBase;

class CScriptThing {                                    // 12 bytes
public:
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CPersonalScript_GlobalThingsScript {
public:
    unsigned char _scriptBase[0x40];
    CGameScriptInterfaceBase* Game;                     // +0x40
    class CArcheryCompetitionSignpost;                  // the entity class below (naming wave 35)
};
}
using NScript::CPersonalScript_GlobalThingsScript;

class CEntityScriptRoot {
public:
    CEntityScriptRoot(CGameScriptInterfaceBase* game) : Game(game) {}
    virtual ~CEntityScriptRoot();
    CGameScriptInterfaceBase* Game;                     // +4
};

class CEntityScriptBase : public CEntityScriptRoot {
public:
    CEntityScriptBase(CGameScriptInterfaceBase* game, const CScriptThing& me) : CEntityScriptRoot(game), Me(me) {}
    virtual ~CEntityScriptBase();
    CScriptThing Me;                                    // +8
};

template <class T> class CCountedPointer {
public:
    CCountedPointer(T* object);                         // 0x00CDEDB0
    T* PObject;
    void* PCount;
};

namespace NScript {
class CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost : public CEntityScriptBase {
public:
    CArcheryCompetitionSignpost(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me)
        : CEntityScriptBase(parent.Game, me), ParentClass(&parent), PMasterData((CQ_SunnyvaleMasterData*)data) {}
    virtual ~CArcheryCompetitionSignpost();
    static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me);
    CPersonalScript_GlobalThingsScript* ParentClass;    // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
};
CCountedPointer<CEntityScriptBase> __fastcall CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::Alloc(CPersonalScript_GlobalThingsScript& parent,
                                                                               CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CArcheryCompetitionSignpost(parent, data, me));
}
}
