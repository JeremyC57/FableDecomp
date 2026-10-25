// CScriptBase::RunSubThreads() -- run the script's spawned functions, then its entity threads (tail call)
// Leaf: ?RunSubThreads@CScriptBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB8220, 16 bytes, thiscall: `push esi ; mov esi,ecx ; call 0x00CB7C40 ; mov ecx,esi ;
//   pop esi ; jmp 0x00CB8170`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 451 "0xCB8220 Destval_CRegisteredMusicEntry/Close = CScriptBase::RunSubThreads"):
//   Ego_r.pdb and FableWin.pdb both name ?RunSubThreads@CScriptBase@@QAEXXZ (public); its callees are the landed
//   CScriptBase::RunSpawnedFunctions 0x00CB7C40 and CScriptBase::RunEntityThreads 0x00CB8170 (both protected,
//   ?...@CScriptBase@@IAEXXZ in both PDBs; the RunEntityThreads row already names this tail call). The landed
//   `void __fastcall Close(S*)` with by-address helpers is the same bytes as a free function.
// Model: no engine/CScriptBase.h exists; only the three members are declared.
class CScriptBase {
protected:
    void RunSpawnedFunctions();                     // 0x00CB7C40
    void RunEntityThreads();                        // 0x00CB8170
public:
    void RunSubThreads();
};

void CScriptBase::RunSubThreads()
{
    RunSpawnedFunctions();
    RunEntityThreads();
}
