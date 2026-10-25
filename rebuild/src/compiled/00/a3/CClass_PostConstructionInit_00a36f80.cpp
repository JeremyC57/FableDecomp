// CClass::PostConstructionInit(CClassFactory& factory) -- hand the newly constructed class object to its factory:
// factory.OnCreateClass(*this) (CClassFactory vtable slot 2), a tail call.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36F80, length 15 bytes (padded to 16), `jmp [edx+8]`.
// Callers: CThing::OnCreate 0x004C9560 (`push [0x013B8A1C]; mov ecx,esi`) and the factory Create functions 0x00A18F70 /
//   0x00A1CBF0.
// Name: Ego_r.pdb 0x00B76790 / FableWin.pdb 0x03195660 ?PostConstructionInit@CClass@@QAEXAAVCClassFactory@@@Z; the
//   CClassFactory vtable 0x0129CF70 (RTTI .?AVCClassFactory@@) slot 2 is OnCreateClass(CClass&) 0x0051DA20
//   (?OnCreateClass@CClassFactory@@MAEXAAVCClass@@@Z). Was catalogued `CThingFilter_IsValid::operator()` (same bytes as a
//   `filter->V2(this)` call, but no filter caller reaches it).
// Model: local polymorphic CClassFactory (engine/CClassFactory.h and engine/CClass.h flatten the vptr); slots 2 and 3 are
//   protected with CClass a friend, as the PDB access (MAE) shows.
// Leaf: ?PostConstructionInit@CClass@@QAEXAAVCClassFactory@@@Z
class CClass;
class CClassTypeInit;

class CClassFactory {
    friend class CClass;
public:
    virtual ~CClassFactory();                                  // slot 0 (??_E 0x00A38080)
protected:
    virtual void AddNewClassType(const CClassTypeInit& init);  // slot 1 0x00A36F90
    virtual void OnCreateClass(CClass& c);                     // slot 2 0x0051DA20
    virtual void OnDeleteClass(CClass& c);                     // slot 3 0x0051DA30
};

class CClass {
public:
    void PostConstructionInit(CClassFactory& factory);
};

void CClass::PostConstructionInit(CClassFactory& factory)
{
    factory.OnCreateClass(*this);
}
