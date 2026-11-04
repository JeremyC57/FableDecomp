// NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDED90, 30 bytes, thiscall, `ret 4`; calls 00f35b40=CEntityScriptBase::~CEntityScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): Anniversary wave 2026-11-04 (high): vtable 0x012C3208 slot 0; Anniversary RTTI NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost (string-literal match, validated 257/258); PDB name set has the sdd member of NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost; slot layout from named TLC siblings; function referenced by this vtable only.
//   ??_GCArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv); PDB: FableWin.pdb ??_GCArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012c3208, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 esb): the class model and
//   the PDB Alloc ?Alloc@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z of the landed row 0x00CDEB40 (rebuild/src/compiled/00/cd/NScriptCPersonalScriptGlobalThingsScriptCArcheryCompetitionSignpost_Alloc_00cdeb40.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The Alloc body is not part of this graded body.
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
