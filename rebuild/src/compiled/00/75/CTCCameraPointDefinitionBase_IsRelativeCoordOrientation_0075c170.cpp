// CTCCameraPointDefinitionBase::IsRelativeCoordOrientation() const -- whether the camera point uses relative orientation (UsingRelativeOrientation +0x3b)
// Leaf: ?IsRelativeCoordOrientation@CTCCameraPointDefinitionBase@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075c170, 9 bytes: mov al,[ecx+3bh] ; test al,al ; setne al ; ret
// Identity (NF120, PRIORITY q4415): Ego_r.pdb 0x0076AB90 / FableWin.pdb 0x024F45F0 ?IsRelativeCoordOrientation@CTCCameraPointDefinitionBase@@QBE_NXZ
//   (the CTCCameraPointDefinitionBase method list; IsRelativeCoordTranslation / IsRelativeCoordOrientation are adjacent in both PDBs as
//   0x0075C160 / 0x0075C170 are in retail); the body tests +0x3b, Ego_r.pdb UsingRelativeOrientation; the landed CameraUseCameraPoint
//   0x0089E190 calls both with this spelling (RELOCATION_MATCH). Was the generated placeholder CASuspendableProcess::CheckTermination.
// Layout: Ego_r.pdb CTCCameraPointDefinitionBase (0x40 bytes: CTCBase 0xC, CutInto +0xC .. CoordAxis +0x22, UsingRelativeCoords +0x3A,
//   UsingRelativeOrientation +0x3B, Activated +0x3C). No engine header exists for the class: the row's local model, respelled in place.
// Shape: `test al,al ; setne al` after the load is a normalising conditional (`b ? true : false`); a plain `return b;` is 4 bytes
//   and `b != false` 11 (NF120 grades).
class CTCCameraPointDefinitionBase { public:
    unsigned char _pad[0x3b];                         // CTCBase .. the members before +0x3b
    bool UsingRelativeOrientation;                         // +0x3b
    bool IsRelativeCoordOrientation() const;
};

bool CTCCameraPointDefinitionBase::IsRelativeCoordOrientation() const
{
    return UsingRelativeOrientation ? true : false;
}
