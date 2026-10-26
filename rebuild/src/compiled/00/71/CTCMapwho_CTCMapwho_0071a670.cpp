// CTCMapwho::CTCMapwho(CThing* owner) -- construct the mapwho component: CTCBase(owner), the four corner mapwho nodes and SearchID zeroed, InMapwho cleared, then each node's back pointer set to this (TL, BR, TR, BL)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071A670, length 94 bytes.
// Leaf: ??0CTCMapwho
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800. Vtable: CTCMapwho @ 0x01265A3C.
// Layout: engine/CTCMapwho.h gives the PDB layout (0x54 bytes: four CMapwhoNode records {PMapwho, PNext, PPrevious, PCell}
// at +0x0c/+0x1c/+0x2c/+0x3c, SearchID +0x4c, the InMapwho bit in the byte at +0x50). The generated header is a flat POD
// with no base, vptr or constructor, so the constructor is written on a local model with the same offsets.
// Shape: every scalar store sits after the vtable store, so the nodes are member objects with inline constructors
// (`CMapwhoNode() : PMapwho(0), PNext(0), PPrevious(0), PCell(0)`) and SearchID is an initialiser; the bit clear is the
// `and byte ptr [esi+50h],0FEh` of a one-bit field; the four back-pointer stores are body assignments in source order.
class CThing;
struct CThingMapCell;
class CTCMapwho;

class CTCBase { public:
    CTCBase(CThing* owner);                        // 0x00686800
    virtual ~CTCBase();                            // 0x00686830
    CThing* m_pThing;                              // +0x04
    long    m_baseWord;                            // +0x08
};

struct CMapwhoNode {
    CTCMapwho*     PMapwho;                        // +0x00
    CMapwhoNode*   PNext;                          // +0x04
    CMapwhoNode*   PPrevious;                      // +0x08
    CThingMapCell* PCell;                          // +0x0c
    CMapwhoNode() : PMapwho(0), PNext(0), PPrevious(0), PCell(0) { }
};

class CTCMapwho : public CTCBase { public:
    CMapwhoNode   TLNode;                          // +0x0c
    CMapwhoNode   TRNode;                          // +0x1c
    CMapwhoNode   BLNode;                          // +0x2c
    CMapwhoNode   BRNode;                          // +0x3c
    unsigned long SearchID;                        // +0x4c
    unsigned char InMapwho : 1;                    // +0x50 bit 0
    CTCMapwho(CThing* owner);
    virtual ~CTCMapwho();
};

CTCMapwho::CTCMapwho(CThing* owner)
    : CTCBase(owner), SearchID(0)
{
    InMapwho = 0;
    TLNode.PMapwho = this;
    BRNode.PMapwho = this;
    TRNode.PMapwho = this;
    BLNode.PMapwho = this;
}