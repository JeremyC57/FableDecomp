// CTCFishingRod::CTCFishingRod(CThing* parent) -- CTCBase(parent), this vtable, the def pointer cleared, then filled from the parent's def class (GetSubDef<CFishingRodDef>)
// Leaf: ??0CTCFishingRod
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2450, length 46 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CDefClassBase::GetSubDef<CFishingRodDef>(CDefPointer<..>*) @ 0x007E2390
// (on the def class at +0x70 of the parent thing; the bool result is dropped). Vtable 0x01270104 = CTCFishingRod
// (rtti-vtables.tsv); the landed factory 0x004D5F65 allocates 0x10 bytes for it.
// Layout: PDB CTCFishingRod (16 bytes): PDef +0xc (CDefPointer<CFishingRodDef const>).
// Shape: the def pointer's address is formed once (`lea eax,[esi+0Ch]`, before the vtable store) and serves both the
// inline constructor's zero store (`mov dword ptr [eax],0`) and the GetSubDef argument; ParentThing is re-read for the
// def class chain. No EH frame: the module is built without /EHsc.
struct CDefPointeeBase;

struct CDefPointer {                             // CDefPointer<CFishingRodDef const>
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer* out);            // 0x007E2390 (GetSubDef<CFishingRodDef>)
};

struct CThing {
    unsigned char  _pad[0x70];
    CDefClassBase* m_pDefClass;                  // +0x70
};

struct CTCBase {
    virtual void v00();
    CThing*       ParentThing;                   // +0x04
    bool          ToBeRemoved;                   // +0x08
    bool          IsInGlobalUpdateTCs;           // +0x09
    CTCBase(CThing* parent);                     // 0x00686800
};

struct CTCFishingRod : CTCBase {
    virtual void v00();                          // own vtable (0x01270104)
    CDefPointer PDef;                            // +0x0c
    CTCFishingRod(CThing* parent);
};

CTCFishingRod::CTCFishingRod(CThing* parent)
    : CTCBase(parent), PDef()
{
    ParentThing->m_pDefClass->GetSubDef(&PDef);
}
