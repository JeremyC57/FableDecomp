// CTCInventoryMagic::OnCreate() -- fetches the magic-inventory sub-def from the owner's def, points the inventory at its inventory def, runs the base OnCreate and empties the screen's entry vector
// Grade: RELOCATION_MATCH (108 bytes) -- the owner def read into a raw local with hand-written null-tested add-ref/release around GetSubDef, the sub-def out pointer zeroed between them (the form that matched the Weapons/Trade OnCreate siblings)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072DC70, length 108 bytes (+ int3). Vtable 0x012668EC slot 4 (the catalog's
// "vfunc_4"; slot 4 of the CTCBase vtable is OnCreate).
// Callees: CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) @ 0x00485815 (manifest names a propagated CAbilityDef
// instantiation), CTCInventoryBase::SetPDef(long) @ 0x005BDB4D, CTCInventoryBase::OnCreate() @ 0x005C0121, the one-argument
// vector::resize(n) @ 0x00443F90 on the NHeroInformationScreens::CBase subobject's vector (manifest "AddPrimitive", propagated),
// the defs' slot 1 on release.
// Shape: the owner's def is a copied CDefPointer temporary from an inline by-value GetDef() (add-ref and release both null-tested); the sub-def out
// pointer is zeroed before the call and released by hand at the end without a null test (`dec [esi+4] ; jne`).
// Layout: CThing PDef +0x70; retail CTCInventoryBase 0x130 bytes, ParentThing +0x04; the CBase subobject at +0x130 (vptr) with
// its 8-byte-element vector at +0x134; the magic def's inventory-def index at +0x94 (retail offset).
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
template <class T> struct CCopiedDefPointer {
    T* Object;
    CCopiedDefPointer(const CDefPointer<T>& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CCopiedDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CInventoryMagicDef : CDefPointeeBase {
    unsigned char _pad08[0x94 - 0x08];
    long InventoryDefIndex;                     // +0x94
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CInventoryMagicDef>* out);   // 0x00485815
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    CCopiedDefPointer<CDefClassBase> GetDef() const { return PDef; }
};

struct CTCInventoryBase_Magic {                 // retail 0x130 bytes
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void OnCreate();                    // slot 4
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x130 - 0x08];
    void SetPDef(long globalDefIndex);          // 0x005BDB4D
};
struct CScreenEntryVector {                     // STLport vector of 8-byte entries
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    void resize(unsigned long n);               // 0x00443F90
};
namespace NHeroInformationScreens {
struct CBase {                                  // vtable 0x0123A3E0
    virtual void v00();
    virtual void v01();
    virtual void v02();
    CScreenEntryVector Entries;                 // +0x04
};
}
class CTCInventoryMagic : public CTCInventoryBase_Magic, public NHeroInformationScreens::CBase {
public:
    virtual void OnCreate();
};

void CTCInventoryMagic::OnCreate()
{
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<CInventoryMagicDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    CInventoryMagicDef* magicDef = def.Object;
    SetPDef(magicDef->InventoryDefIndex);
    CTCInventoryBase_Magic::OnCreate();
    Entries.resize(0);
    if (--magicDef->m_refCount == 0)
        magicDef->Destroy();
}
