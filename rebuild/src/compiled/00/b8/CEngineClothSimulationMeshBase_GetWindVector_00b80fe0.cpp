// Leaf: ?GetWindVector@CEngineClothSimulationMeshBase@@UBE?AVC3DVector@@XZ
// CEngineClothSimulationMeshBase::GetWindVector() const -- sample the simulation manager's noisy wind at the mesh position, scale it by 100 and return it rotated into the mesh's local space
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80FE0, length 112 bytes (+ int3 padding).
// Option S unit (owner ruling 2026-11-24): this function's one body lives in its retail TU-mate's row 0x00B80AD0 (Ego_r.pdb / FableWin.pdb
//   Engine_Simulation_Manager.obj), included below (no code of its own), as the EH funclet rows include their parent; that object's
//   ?GetWindVector copy grades against this address. Before the unit this row carried a second strong ConvertVectorToLocal (QBE stand-in)
//   and called 0x00B80B70 as the stand-in CSimulationEnvironment::GetWindAt; both now use their PDB spellings.
#include "../src/compiled/00/b8/CEngineClothSimulationMeshBase_ConvertVectorToLocal_00b80ad0.cpp"
