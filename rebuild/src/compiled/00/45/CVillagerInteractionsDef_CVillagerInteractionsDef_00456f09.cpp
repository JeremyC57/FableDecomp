// CVillagerInteractionsDef::CVillagerInteractionsDef() -- the CParentDefClassBase base, this def's vtable, and its CDefString members unset (TablePos -1)
// Leaf: ??0CVillagerInteractionsDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00456F09, 26 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x01233F24 (RTTI .?AVCVillagerInteractionsDef@@ : CParentDefClassBase : CDefClassBase : ...); FableWin.pdb / Ego_r.pdb
//   ??0CVillagerInteractionsDef@@QAE@XZ; the -1 stores sit on the class's CDefString members (Ego_r.pdb layout, retail = Ego_r.pdb - 4 (CParentDefClassBase holds an STLport vector)).
// Callees: CParentDefClassBase::CParentDefClassBase() 0x00430370.
// Shape (family K089, representative 0x0044C1DC): the implicit-style default constructor in a size TU: the base call, the
//   vtable store, each CDefString's inline `TablePos(-1)` as `or dword ptr [m],-1`; the POD members stay uninitialised.
#pragma optimize("s", on)
class CDefString {                                       // 4 bytes, an index into the def string table
public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};
class CParentDefClassBase {                                            // retail 0x3C bytes (members not touched here)
public:
    CParentDefClassBase();                                             // 0x00430370
    virtual ~CParentDefClassBase();
    unsigned char _body[0x3C - 4];
};
class CVillagerInteractionsDef : public CParentDefClassBase {
public:
    CVillagerInteractionsDef();
    CDefString Animation1;                              // +0x3C
    CDefString Animation2;                              // +0x40
    float      Distance;                                // +0x44
};
CVillagerInteractionsDef::CVillagerInteractionsDef()
{
}