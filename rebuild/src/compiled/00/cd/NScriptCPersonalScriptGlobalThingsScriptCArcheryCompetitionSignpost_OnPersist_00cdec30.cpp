// NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::OnPersist(CPersistContext&) -- empty override (the entity persists nothing of its own)
// Leaf: ?OnPersist@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDEC30, 3 bytes: ret 4.
// Identity (naming wave 35, family ent): Anniversary wave 2026-11-04 (high): vtable 0x012C3208 slot 4; Anniversary RTTI NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost (string-literal match, validated 257/258); PDB name set has the OnPersist member of NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost; slot layout from named TLC siblings; function referenced by this vtable only.
//   ?OnPersist@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXAAVCPersistContext@@@Z in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00CDEB40 (rebuild/src/compiled/00/cd/NScriptCPersonalScriptGlobalThingsScriptCArcheryCompetitionSignpost_Alloc_00cdeb40.cpp); the PDB member declaration added; the Alloc body (if any) is not part of this graded body.
class CGameScriptInterfaceBase;
class CScriptDataBase;
class CPersistContext;                                  // the OnPersist parameter (declaration only)

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
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
CCountedPointer<CEntityScriptBase> __fastcall CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::Alloc(CPersonalScript_GlobalThingsScript& parent,
                                                                               CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CArcheryCompetitionSignpost(parent, data, me));
}
}

void NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::OnPersist(CPersistContext&)
{
}
