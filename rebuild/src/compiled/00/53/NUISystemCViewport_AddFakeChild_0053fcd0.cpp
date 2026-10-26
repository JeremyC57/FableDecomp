// NUISystem::CViewport::AddFakeChild(CComponent* child) -- append the component to the fake-child vector at +0x164 and make this viewport its parent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053FCD0, length 48 bytes.
// Callees: vector<CComponent*>::push_back(const CComponent*&) @ 0x00486024 (manifest "Vector_PushBack"; the
// argument slot's address is passed: `lea eax,[esp+8] ; push eax`), then the child's virtual slot 90
// (+0x168: SetParent(CComponent*)) with `this` pushed. The child is re-read from its slot for the call.
// Layout: CViewport's fake-child vector sits at +0x164 (the constructor at 0x0054EA00 clears it); the
// component vtable slot count is modelled with fillers.
namespace NUISystem {

class CComponent;

struct CComponentVector {                      // 12-byte retail vector {first, last, end}
    CComponent* m_pFirst;
    CComponent* m_pLast;
    CComponent* m_pEnd;
    void push_back(CComponent* const& child);  // 0x00486024
};

class CComponent { public:
    virtual void v00();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70();
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80();
    virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85();
    virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void SetParent(CComponent* parent);   // slot 90 (+0x168)
};

class CViewport : public CComponent { public:
    unsigned char    _pad04[0x160];            // +0x004..+0x163
    CComponentVector m_fakeChildren;           // +0x164
    void AddFakeChild(CComponent* child);
};

void CViewport::AddFakeChild(CComponent* child)
{
    m_fakeChildren.push_back(child);
    child->SetParent(this);
}

}