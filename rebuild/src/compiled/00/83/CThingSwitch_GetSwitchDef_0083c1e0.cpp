// CThingSwitch::GetSwitchDef() -- the switch's def, fetched from the definition manager by the thing's def index and returned as a counted pointer
// Leaf: GetSwitchDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083C1E0, 76 bytes (+ int3 padding).
// Callees: CThing::GetDefinitionManager @ 0x004C7990 (propagated "CViewBase::GetDisplayEngine"),
//          CDefinitionManager::GetDef<CThingPhysicalSwitchDef> @ 0x0083CFA0, the def's Destroy (vtable slot 1).
// Identity: called from CThingSwitch's WorldUpdate (vtable slot 19, 0x0083CD70); the manifest label
//   "CThingBuilding::GetFloorMaterial" is propagated. CThing::DefGlobalIndex (unsigned short) at retail +0x8c.
// Shape: a CDefPointer local filled by the manager, returned by value through the hidden pointer (`ret 4`): the
//   copy constructor's add-ref and the local's release share one null test and keep both count stores
//   (`inc edx ; mov eax,edx ; dec eax ; mov [ecx+4],edx ; mov [ecx+4],eax`).
class CDefPointeeBase { public:                // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();             // slot 1
    long m_refCount;                    // +0x04
};

class CThingPhysicalSwitchDef : public CDefPointeeBase { public: };

template<class T> class CDefPointer { public:
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

class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CThingPhysicalSwitchDef>* out);   // 0x0083CFA0
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingPhysicalSwitchDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThingSwitch { public:
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