// CTCBase::GetDefinitionManager() const -- the game definition manager, read through the parent thing
// Leaf: ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006869D0, 8 bytes: mov ecx,[ecx+4] (ParentThing) ; jmp 0x004C7990
//   (CThing::PeekDefinitionManager, which returns g_pThingManager->DefinitionManager).
// Identity (sweep3; was the old-lane forwarder "CTCHeroStats::GetDisplayEngine", earlier labels CTCBase::GetDefinitions /
//   GetGameData): Ego_r.pdb and FableWin.pdb ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ (protected
//   const); Ego_r order HandleMessage 0x006B1DA0, StatGetDriverType 0x006B1DB0, GetDefinitionManager 0x006B1DC0,
//   EndSerialising 0x006B1DD0 = retail HandleMessage 0x006869B0, 0x006869C0 (xor eax,eax), 0x006869D0, EndSerialising
//   0x006869E0; no PDB has a CTCHeroStats::GetDisplayEngine (only CMainGameComponent / NDisplayView::CViewBase); all 389
//   call sites are CTCBase-derived component methods that go on to call GetDef / GetPDefFromGlobalIndex on the result.
// Layout: engine/CTCBase.h (ParentThing +0x04); the generated header has no member functions, so the class is
//   restated here with the same layout.
class CGameDefinitionManager;

class CThing {
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x004C7990
};

class CTCBase {
public:
    virtual ~CTCBase();
protected:
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    const CGameDefinitionManager& GetDefinitionManager() const;
};

const CGameDefinitionManager& CTCBase::GetDefinitionManager() const
{
    return ParentThing->PeekDefinitionManager();
}
