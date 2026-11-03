W='${S}/w22/chunk734/hdr/'
import sys
p=W+'engine/CWorld.h'; s=open(p).read()
cut=s.index('#else // flattened Ego_r layout')
ret,rest=s[:cut],s[cut:]
pairs=[
('''class CProceduralTexture;
''','''class CProceduralTexture;
// Chunk 734: the enum-typed members' real types (Ego_r.pdb CWorld, ghidra_out/fable_types.h); MS forward enums (4 bytes), the
// enumerator names are not in the PDB name sets, so the rows compare and store the values retail uses.
enum EPlayerMode;
enum ESaveGameLoadStatus;
enum ERegionLoadStatus;
enum EPlayerSpawnStatus;
'''),
('''    const CPlayer* PeekMyPlayer() const;                         // 0x0049D9D0
protected:
    const CGameEventDispatch* GetEventDispatchTableEntry(long type) const;   // 0x0049D8E0 ?GetEventDispatchTableEntry@CWorld@@IBEPBVCGameEventDispatch@@J@Z
public:
''','''    const CPlayer* PeekMyPlayer() const;                         // 0x0049D9D0
    // Chunk 734 (row 0x004A5A40 UpdateFrame): Ego_r.pdb / FableWin.pdb spellings of the CWorld members it calls.
    void ResetToFrontEnd();                                      // 0x0049DCC0 ?ResetToFrontEnd@CWorld@@QAEXXZ
    void OpenHeroInformationScreen(EPlayerMode mode);            // 0x0049F3F0 ?OpenHeroInformationScreen@CWorld@@QAEXW4EPlayerMode@@@Z
protected:
    const CGameEventDispatch* GetEventDispatchTableEntry(long type) const;   // 0x0049D8E0 ?GetEventDispatchTableEntry@CWorld@@IBEPBVCGameEventDispatch@@J@Z
    // Chunk 734: protected members (IAE in both PDBs). SetAsPaused is out of line at 0x0049D8F0 (`cmp al,[ecx+0A4h]; je; mov`) and
    // expanded in UpdateFrame (0x004A5E64: `cmp byte ptr [esi+0A4h],1 ; je ; mov byte ptr [esi+0A4h],1`), so it is defined inline.
    void SetAsPaused(bool paused) { if (Paused != paused) Paused = paused; }   // ?SetAsPaused@CWorld@@IAEX_N@Z
    void UpdateFrame();                                          // 0x004A5A40 ?UpdateFrame@CWorld@@IAEXXZ
    void UpdateSaveGameLoad();                                   // 0x004A2F10 ?UpdateSaveGameLoad@CWorld@@IAEXXZ
    void UpdateRegionLoad();                                     // 0x004A3740 ?UpdateRegionLoad@CWorld@@IAEXXZ
    void PrepareAtmosBanksForLoadedRegion();                     // 0x0049E5E0 ?PrepareAtmosBanksForLoadedRegion@CWorld@@IAEXXZ
    void UpdateEnvironmentCameraPos(const C3DVector& pos);       // 0x0049E370 ?UpdateEnvironmentCameraPos@CWorld@@IAEXABVC3DVector@@@Z
private:
    // The world frame counter behind the static GetFrame / SetFrame (0x013B89BC, read by 806 call sites). No PDB data symbol:
    // descriptive name, as the landed GetFrame 0x0049D870.
    static long Frame;                                           // 0x013B89BC
public:
'''),
('unsigned char               HeroInformationScreenModeAfterTutorial[0x4]; // +0xe4 EPlayerMode','EPlayerMode                 HeroInformationScreenModeAfterTutorial; // +0xe4 (chunk 734: typed)'),
('unsigned char               SaveGameLoadStatus[0x4];     // +0xf8 ESaveGameLoadStatus','ESaveGameLoadStatus         SaveGameLoadStatus;          // +0xf8 (chunk 734: typed)'),
('unsigned char               RegionLoadStatus[0x4];       // +0x104 ERegionLoadStatus','ERegionLoadStatus           RegionLoadStatus;            // +0x104 (chunk 734: typed)'),
('unsigned char               PlayerSpawnStatus[0x4];      // +0x178 EPlayerSpawnStatus','EPlayerSpawnStatus          PlayerSpawnStatus;           // +0x178 (chunk 734: typed)'),
]
for old,new in pairs:
    assert ret.count(old)==1,(old[:60],ret.count(old))
    ret=ret.replace(old,new)
open(p,'w').write(ret+rest)
