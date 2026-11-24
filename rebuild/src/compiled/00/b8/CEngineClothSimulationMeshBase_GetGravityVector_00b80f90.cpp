// Leaf: ?GetGravityVector@CEngineClothSimulationMeshBase@@UBE?AVC3DVector@@XZ
// CEngineClothSimulationMeshBase::GetGravityVector() const -- the simulation manager's gravity scaled by the cloth gravity factor, rotated into the mesh's local space
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B80F90, 69 bytes, thiscall, `ret 4` (C3DVector returned through the hidden pointer).
// Option S unit (owner ruling 2026-11-24): this function's one body lives in its retail TU-mate's row 0x00B80AD0 (Ego_r.pdb / FableWin.pdb
//   Engine_Simulation_Manager.obj), included below (no code of its own), as the EH funclet rows include their parent; that object's
//   ?GetGravityVector copy grades against this address. Before the unit this row carried a second strong ConvertVectorToLocal.
#include "../src/compiled/00/b8/CEngineClothSimulationMeshBase_ConvertVectorToLocal_00b80ad0.cpp"
