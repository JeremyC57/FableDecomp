// CCarriedReadableDef::CCarriedReadableDef() -- the CSubDefClassBase base, this def's vtable, and its CDefString members unset (TablePos -1)
// Leaf: ??0CCarriedReadableDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5ECA, 26 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x0123DB94 (RTTI .?AVCCarriedReadableDef@@ : CSubDefClassBase : CDefClassBase : ...); FableWin.pdb / Ego_r.pdb
//   ??0CCarriedReadableDef@@QAE@XZ; the -1 stores sit on the class's CDefString members (Ego_r.pdb layout, retail = Ego_r.pdb).
// Callees: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0.
// Shape (family K089, representative 0x0044C1DC): the implicit-style default constructor in a size TU: the base call, the
//   vtable store, each CDefString's inline `TablePos(-1)` as `or dword ptr [m],-1`; the POD members stay uninitialised.
#pragma optimize("s", on)
class CDefString {                                       // 4 bytes, an index into the def string table
public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};
class CSubDefClassBase {                                            // retail 0x28 bytes (members not touched here)
public:
    CSubDefClassBase();                                             // 0x0044C0C0
    virtual ~CSubDefClassBase();
    unsigned char _body[0x28 - 4];
};
class CCarriedReadableDef : public CSubDefClassBase {
public:
    CCarriedReadableDef();
    CDefString TitleTextTag;                            // +0x28
    CDefString BodyTextTag;                             // +0x2C
};
CCarriedReadableDef::CCarriedReadableDef()
{
}