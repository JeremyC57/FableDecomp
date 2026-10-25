// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::CSecretPassageSkeleton(CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me) -- the secret-passage skeleton's entity script: entity-script base over the quest's interface and Me, the parent, the four skeleton_object strings, and the master data last
// Leaf: ??0CSecretPassageSkeleton@CQ_OpeningGraveyardSecretPassageScript@NScript@@QAE@AAV12@PAUCScriptDataBase@@ABUCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC3DF0, 79 bytes (+ int3 padding), `ret 0Ch`; its only caller is the
//   factory 0x00DC3D70 (`push 34h`, CSecretPassageSkeleton::Alloc), bound as "SecretPassageSkeleton" at 0x00DC28DC.
//   PDB ctor shape ??0CX@CQ_Y@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z (the parent by reference).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, CCharString::CCharString() @ 0x0099E4B0 (x4).
// Layout = Ego_r.pdb CSecretPassageSkeleton (52 bytes, entity scripts keep their PDB offsets): CEntityScriptBase {vptr
//   0x012C3224, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, returned[4] +0x1c, skeleton_object[4] +0x20,
//   collected_all +0x30 (the bools are left uninitialised). Vtable 0x012DA494.
// Shape: the landed entity-script constructors (CDarkwoodTrader 0x00E01670, CBriarRose 0x00D9E930): Game (the parent's
//   interface at +0x40) is a plain initialiser of the base stored before the base vtable, ParentClass an initialiser, the
//   CCharString array (constructor only, no destructor in the TU) an inline counted loop, PMasterData a body assignment
//   stored last.
struct CGameScriptInterface;
struct CScriptDataBase;
struct CBaseClass {                           // vtable 0x01231710
    virtual void v00();
    CBaseClass();                             // 0x0099A2D0
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
struct CScriptThing : CBaseClass {            // vtable 0x01238C8C, 12 bytes (PImp Data +4, Info +8)
    CCountedPointerWords PImp;                // +0x04
    virtual void v00();
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
};
struct CEntityScriptBase {                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;               // +0x04 (a plain initialiser: stored before the vtable)
    CScriptThing          Me;                 // +0x08
    CEntityScriptBase(const CScriptThing& thing, CGameScriptInterface* game) : Game(game), Me(thing) { }
};
struct CCharString {
    void* PStringData;
    CCharString();                            // 0x0099E4B0
};
namespace NScript {
struct CQ_SunnyvaleMasterData;
class CQ_OpeningGraveyardSecretPassageScript {          // the owning quest script (retail 0x74 bytes): only Game is read
public:
    unsigned char         _pad00[0x40];                 // CScriptBase
    CGameScriptInterface* Game;                         // +0x40 (CLevelScriptBase<T>::Game)
    class CSecretPassageSkeleton;
};
class CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton : public CEntityScriptBase {   // vtable 0x012DA494
public:
    CQ_OpeningGraveyardSecretPassageScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*                 PMasterData;   // +0x18
    bool                                    returned[4];   // +0x1c
    CCharString                             skeleton_object[4];   // +0x20
    bool                                    collected_all; // +0x30
    virtual void v00();
    CSecretPassageSkeleton(CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me);
};
CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::CSecretPassageSkeleton(CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(me, parent.Game), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}