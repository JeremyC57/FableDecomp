# t3notes_1_60.py: NF298's per-entry notes for the T3 readings of ENTRIES 1-60 (passed to t3read.py)
import subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF298'
N={
'00994b40':'Binders 0x00A99840 / 0x00A9BA70: CMemoryDataOutputStreamMethods derives the GENERATED flat CMemoryDataOutputStream branch (FABLE_RETAIL_MEMORY_OUTPUT_STREAM); GetLength / PeekData are declared only in the FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM branch. Recipe for a later pass: an add-only flat opt-in with both declarations (both PDB name sets) -- the flat branch\'s FABLE_STATIC_ASSERT is unpinned, so pin_asserts first. Both binders\' own code lives in the stand-in namespaces X_00a99840 / X_00a9ba70',
'00995e70':'Same binders and recipe as entry 8 (GetLength)',
'00996af0':'The ctor: the binders build a local CMemoryDataOutputStreamMethods (its own ctor / non-virtual dtor stand-ins); binding the PDB ctor means retyping the local onto the retail polymorphic class (virtual dtor 0x00996B30): port',
'00487cf0':'Binder 0x0048A070 is itself CPlayer_Methods (a stand-in deriving engine/CPlayer.h): self port. Stand-in takes CThing*, the PDB CThingPlayerCreature*',
'00489d40':'Binder 0x0048A070 (self CPlayer_Methods): CreateCharacter is private (AAE) in both PDBs, unreachable from the derived stand-in: self port',
'0043a2a0':'Identity doubt: the binder calls CGameUI_Retail::ShowOnScreenMessage(const CWideString&, long, long, long); the PDB name for 0x0043A2A0 is CPlayerGui::AddScriptScreenText(CWideString, C2DVector, CRGBColour, CCharString): research before a port',
'0051a110':'Binder 0x006191D0 calls the destructor as a free fastcall on (char*)region + 4: binding ~CRegion needs the CRegion type at that offset (a new model or engine/CRegion.h)',
'00a04630':'Binder 0x00A07900\'s own function is CRenderStateManager_Methods::Init (stand-in deriving the header class); SetupRenderStates is private (AAE) in both PDBs: self port',
'00a059e0':'Binder 0x00A07900 defines a same-TU stand-in BODY for SetDefaultRenderStates (`__declspec(noinline) void CRenderStateManager_Methods::SetDefaultRenderStates()`): respelling it would define the owner 0x00A059E0\'s symbol with a made-up body (D3): T3',
'004ae0e0':'Three stand-ins (two local member dtors, one free InnerDtor) and an empty self leaf; identity not checked',
'006c0330':'Binder 0x006C04B0 is itself the free stand-in CTCBuyableHouse_IncrementDressLevel; its Methods SetDressLevel(int) stands for SetupDressingAtLevel(long) (J): self port plus a parameter type',
'006c1150':'Binder 0x00895ED0 uses the GENERATED flat CTCBuyableHouse branch; the PDB declarations exist only under FABLE_RETAIL_CTCBUYABLEHOUSE_RECEIVEDCONFIRMATION in the retail branch (and the flat branch is marked do-not-edit). The row also carries the two-view ScriptMethods model (nf105 debt)',
'006c17b0':'Same binder and reason as entry 37 (SetRented)',
'006a2520':'CarryingMethods derives CTCBase, not CTCCarrying: a respell needs the real class',
'00736590':'Binder 0x00736AE0\'s own function is CTCCombat_Methods::FrameUpdate; UpdateUsableMeleeCombatAbilities is private (AAE) in both PDBs (engine/CTCCombat.h ABILITYLIST opt-in): self port',
'00779e10':'ExperienceOrbMethods derives CTCBase with hand-placed members, not CTCDExperienceOrb',
'007310c0':'Binder 0x007311F0 (self CTCDoor_Methods::FrameUpdate) uses CTCDoor.h\'s flat branch; the protected PDB declaration lives only under FABLE_RETAIL_CTCDOOR_OPENCLOSE in the FABLE_RETAIL_CTCDOOR branch: a flat add-only opt-in or a port',
'005b43a2':'Free stand-in ?h_005b4ea8@@YGHHH@Z (stdcall int(int,int)) for the thiscall member AddClothingToThing(long, CThing*): different convention and types',
'0056ff3d':'Return type is the stand-in CTCHeroExperience_Calls*',
'0056ffc5':'Return type is the stand-in CTCInventoryAbilities_Calls*',
'005d9320':'PDB is the static __fastcall CTCInventoryItem::ClearCachedDefs (SI); the binder (0x00589C55, ~CTCInventory) declares a thiscall member of its local CTCInventory model: needs the CTCInventoryItem class (engine/CTCInventoryItem.h has no declaration) and a convention check of the call site',
}
args=['python3',A+'/tools/t3read.py','1','60']+['%s=%s'%(k,v) for k,v in N.items()]
print(subprocess.run(args,capture_output=True,text=True).stdout)
