// CWaterPatchTesselatorBase<CTVertexWaterBackground>::GetSizeOfVertex() -- constant-return member: returns 0x38 (sizeof the vertex). __thiscall.
// Leaf: ?GetSizeOfVertex@?$CWaterPatchTesselatorBase@VCTVertexWaterBackground@@@@EAEJXZ
// Identity (NF385): retail vftable 0x012A3334 (RTTI CWaterPatchTesselatorBase<CTVertexWaterBackground>) slot 3 = FableWin slot 3
// ?GetSizeOfVertex@?$CWaterPatchTesselatorBase@VCTVertexWaterBackground@@@@EAEJXZ (Engine_landscape_tesselator.obj #44; Ego_r.pdb names the WaterBackground instance too).
// Private virtual returning long (both PDBs); retail `mov eax, 0x38 ; ret`. Was modelled as a plain int member of a non-template class.
class CTVertexWaterBackground;
template <class T> class CWaterPatchTesselatorBase {
    virtual long GetSizeOfVertex();
};
template <> long CWaterPatchTesselatorBase<CTVertexWaterBackground>::GetSizeOfVertex() { return 0x38; }
