// CTCCameraPointBuilding::CTCCameraPointBuilding(CThing* parent) -- CTCCameraPointDefinitionBase(parent), three cleared flags, own vtable
// Leaf: CTCCameraPointBuilding
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8193, length 35 bytes.
// Callees: CTCCameraPointDefinitionBase::CTCCameraPointDefinitionBase(CThing*) @ 0x0075C180 (stores vtable
// 0x01269B04 = CTCCameraPointDefinitionBase; the manifest's "CTCOpinionSource" label on it is propagated).
// Vtable 0x0123ECBC = CTCCameraPointBuilding (rtti-vtables.tsv): the chunk label "CTCEditor" is propagated.
// PDB CTCCameraPointBuilding (0x44 bytes): InMyBuilding +0x40, ExitPoint +0x41, StartPoint +0x42 (the base
// occupies +0x00..+0x3f).
// Shape: /Os pass-through base argument; `xor al,al` and the three byte stores are plain init-list
// initialisers, scheduled BEFORE the derived vtable store.
#pragma optimize("s", on)
struct CThing;

struct CTCCameraPointDefinitionBase_Calls {
    virtual void v00();
    unsigned char _body[0x40 - 4];                          // CTCBase (+0x04..+0x0b) and the camera-point fields
    CTCCameraPointDefinitionBase_Calls(CThing* parent);     // 0x0075C180
};

struct CTCCameraPointBuilding : CTCCameraPointDefinitionBase_Calls {
    bool InMyBuilding;                                      // +0x40
    bool ExitPoint;                                         // +0x41
    bool StartPoint;                                        // +0x42
    virtual void v00();                                     // own vtable 0x0123ECBC
    CTCCameraPointBuilding(CThing* parent);
};

CTCCameraPointBuilding::CTCCameraPointBuilding(CThing* parent)
    : CTCCameraPointDefinitionBase_Calls(parent)
    , InMyBuilding(false)
    , ExitPoint(false)
    , StartPoint(false)
{
}
