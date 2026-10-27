// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Init() -- reset the script's state: already_talked_to, Helmfound, Swordfound, Shieldfound, Armourfound = 0/false
// Leaf: ?Init@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC4B50, 18 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 of the entity-script vtable 0x012DA658 (stored by the landed CSecretPassageGraveyardKeeper::Alloc); FableWin.pdb
//   ?Init@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ; the members it clears are Ego_r.pdb's CSecretPassageGraveyardKeeper fields at their PDB offsets.
// Shape (family K066, representative 0x00CF06D0): one zero register (al, or eax when a long is cleared) and one store per
//   member in source order; every state member is cleared.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CScriptThing {                                    // 12 bytes (Ego_r): vptr, PImp (a CCountedPointer)
public:
    virtual ~CScriptThing();
    void* PImpData;                                     // +0x04
    void* PImpInfo;                                     // +0x08
};
class CScriptBase;
class CEntityScriptBase {                               // 20 bytes (Ego_r)
public:
    virtual ~CEntityScriptBase();
    virtual CScriptBase* GetParentScript() const;       // slot 1
private:
    virtual void Init();                                // slot 2
public:
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
};
class CScriptBase {
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
};
class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    CScriptDataBase* MasterScriptData;                  // +0x44
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OpeningGraveyardSecretPassageScript : public CLevelScriptBase {
public:
    class CSecretPassageGraveyardKeeper : public CEntityScriptBase {
    private:
        virtual void Init();
    public:
        CQ_OpeningGraveyardSecretPassageScript* ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool  already_talked_to;                       // +0x1C
        bool  Helmfound;                               // +0x1D
        bool  Swordfound;                              // +0x1E
        bool  Shieldfound;                             // +0x1F
        bool  Armourfound;                             // +0x20
    };
};

void CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Init()
{
    already_talked_to = false;
    Helmfound = false;
    Swordfound = false;
    Shieldfound = false;
    Armourfound = false;
}
}