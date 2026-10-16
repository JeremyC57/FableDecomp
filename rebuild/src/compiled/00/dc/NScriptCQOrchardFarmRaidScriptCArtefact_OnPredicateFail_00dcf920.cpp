// NScript::CQ_OrchardFarmRaidScript::CArtefact::OnPredicateFail() -- the artefact entity's predicate failed: find the parent quest's crate whose thing matches this entity's and erase it from CrateList
// Leaf: ?OnPredicateFail@CArtefact
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCF920, length 233 bytes (+7 int3). Ego_r.pdb
// ?OnPredicateFail@CArtefact@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ.
// Callees: the crate's pimpl virtual slot 78 (+0x138) with the entity's thing word (+0x0c), vector<CScriptThing>::erase(iterator)
// @ 0x00CD3152 (catalog "std::vector::pop_back"), operator delete @ 0x00BFE9BC, CBase::~CBase @ 0x0099A2E0.
// Shape: each crate is copied into a CScriptThing local (vtable 0x01238C8C, pimpl add-ref when the count block is non-null);
// the test is an inline CScriptThing member forwarding to the pimpl's virtual; on a match the element is erased and the
// local destroyed before returning; otherwise the local's inline destructor (counted release, zeroing, CBase dtor) runs
// and the loop re-reads ParentClass->CrateList's end. The local CScriptThing / pimpl model is the landed
// CM_HiddenBooty::Init 0x00E93080 one.
// Layout: retail CArtefact (PDB 0x1c): entity thing word +0x0c, ParentClass +0x14; retail CQ_OrchardFarmRaidScript CrateList
// +0x54 (PDB +0x68, the usual -0x14 retail shift; STLport vector of 12-byte CScriptThing).
void __cdecl operator delete(void* p);             // 0x00BFE9BC
struct CBase {
    virtual void v00();
    ~CBase();                                      // 0x0099A2E0
};
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CScriptThingImp {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual bool IsThing(void* thing) const;       // slot 78 (+0x138)
};
struct CScriptThingPImp {                          // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    SCountBlock*     Info;
    __forceinline CScriptThingPImp(const CScriptThingPImp& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    __forceinline ~CScriptThingPImp()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThing : CBase {                      // 12 bytes, vtable 0x01238C8C
    CScriptThingPImp PImp;
    virtual void v00();
    __forceinline CScriptThing(const CScriptThing& other) : PImp(other.PImp) {}
    __forceinline ~CScriptThing() {}
    bool IsThing(void* thing) const { return PImp.Data->IsThing(thing); }
};
struct CScriptThingVector {                        // STLport vector<CScriptThing> (12 bytes)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThing* begin() { return _M_start; }
    CScriptThing* end() { return _M_finish; }
    CScriptThing* erase(CScriptThing* position);   // 0x00CD3152
};

namespace NScript {
struct CQ_OrchardFarmRaidScript {
    unsigned char _pad[0x54];
    CScriptThingVector CrateList;                  // +0x54
    struct CArtefact;
};

struct CQ_OrchardFarmRaidScript::CArtefact {
    virtual void v00();
    long _f04;                                     // +0x04
    long _f08;                                     // +0x08
    void* m_pThing;                                // +0x0c the entity's thing
    long _f10;                                     // +0x10
    CQ_OrchardFarmRaidScript* ParentClass;         // +0x14
    void* PMasterData;                             // +0x18 CQ_SunnyvaleMasterData*
    void OnPredicateFail();
};

void CQ_OrchardFarmRaidScript::CArtefact::OnPredicateFail()
{
    for (CScriptThing* it = ParentClass->CrateList.begin(); it != ParentClass->CrateList.end(); ++it) {
        CScriptThing crate(*it);
        if (crate.IsThing(m_pThing)) {
            ParentClass->CrateList.erase(it);
            return;
        }
    }
}
}