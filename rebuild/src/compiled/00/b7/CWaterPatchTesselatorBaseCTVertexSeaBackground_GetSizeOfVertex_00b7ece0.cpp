// CWaterPatchTesselatorBase<CTVertexSeaBackground>::GetSizeOfVertex() -- constant-return member: returns 0xc (sizeof the vertex). __thiscall.
// Leaf: ?GetSizeOfVertex@?$CWaterPatchTesselatorBase@VCTVertexSeaBackground@@@@EAEJXZ
// Identity (NF385): retail vftable 0x012A334C (RTTI CWaterPatchTesselatorBase<CTVertexSeaBackground>) slot 3 = FableWin slot 3
// ?GetSizeOfVertex@?$CWaterPatchTesselatorBase@VCTVertexSeaBackground@@@@EAEJXZ (Engine_landscape_tesselator.obj #51; Ego_r.pdb names the WaterBackground instance too).
// Private virtual returning long (both PDBs); retail `mov eax, 0xc ; ret`. Was modelled as a plain int member of a non-template class.
class CTVertexSeaBackground;
template <class T> class CWaterPatchTesselatorBase {
    virtual long GetSizeOfVertex();
};
template <> long CWaterPatchTesselatorBase<CTVertexSeaBackground>::GetSizeOfVertex() { return 0xc; }
