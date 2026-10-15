// CThingSwitch::GetSwitchDef() -- the switch's def, fetched from the definition manager by the thing's def index and returned as a counted pointer
// Leaf: GetSwitchDef
// Grade: DIFFER(82v76) -- the address-taken CDefPointer local is re-read from its slot after the add-ref store (`inc [eax+4] ; mov eax,[esp+4] ; dec [eax+4] ; mov ecx,[esp+4]`); retail loads it once into ecx and keeps both count stores (`inc edx ; mov eax,edx ; dec eax ; mov [ecx+4],edx ; mov [ecx+4],eax`) -- the wave-24k "CDefPointer local re-read from its slot" lane gap
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083C1E0, 76 bytes (+ int3 padding).
// Callees: CThing::GetDefinitionManager @ 0x004C7990 (propagated "CViewBase::GetDisplayEngine"),
//          CDefinitionManager::GetDef<CThingPhysicalSwitchDef> @ 0x0083CFA0, the def's Destroy (vtable slot 1).
// Identity: called from CThingSwitch's WorldUpdate (vtable slot 19, 0x0083CD70); the manifest label
//   "CThingBuilding::GetFloorMaterial" is propagated. CThing::DefGlobalIndex (unsigned short) at retail +0x8c.
// Shape: a CDefPointer local filled by the manager, returned by value through the hidden pointer (`ret 4`): the
//   copy constructor's add-ref and the local's release share one null test and keep both count stores
//   (`inc edx ; mov eax,edx ; dec eax ; mov [ecx+4],edx ; mov [ecx+4],eax`).
struct CDefPointeeBase {                // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();             // slot 1
    long m_refCount;                    // +0x04
};

struct CThingPhysicalSwitchDef : CDefPointeeBase { };

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
};

struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CThingPhysicalSwitchDef>* out);   // 0x0083CFA0
};

struct CThingSwitch {
    unsigned char _pad00[0x8c];
    unsigned short DefGlobalIndex;      // +0x8c (CThing; PDB +0x9c)
    CDefinitionManager* GetDefinitionManager();     // 0x004C7990 (CThing)
    CDefPointer<CThingPhysicalSwitchDef> GetSwitchDef();
};

CDefPointer<CThingPhysicalSwitchDef> CThingSwitch::GetSwitchDef()
{
    CDefPointer<CThingPhysicalSwitchDef> def;
    GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
    return def;
}
