// Behaviour test for 00852750: src/00852750.cpp copied verbatim; SetPThingOrigin points PThingOrigin at the thing and copies its
// placeholder position into CoordOrigin and the placeholder's right-handed set into CoordAxis.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NCameraModes::CScriptModuleBase::SetPThingOrigin(const CThing* thing) -- point PThingOrigin at the thing and take its position as CoordOrigin and its orientation set as CoordAxis
// Retry 10b (getter sweep + hand): the placeholder is read through an inline `GetPlaceholder()` getter (FIXES B-15) and the inline
// CIntelligentPointer operator= copies through a named item local (`const CBaseObject* item = other.GetPItem(); SetPItem(item);`),
// which runs GetPItem before the `lea edi,[esi+0Ch]` of PThingOrigin as retail does.
// Leaf: SetPThingOrigin
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852750, length 160 bytes (152 + int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, ::SetPItem @ 0x00A01B90 (twice),
// ::GetPItem @ 0x00A01B50 (twice), ::~CBaseIntelligentPointer @ 0x00A01C10, and the thing placeholder's
// virtual at slot 72 (+0x120) returning its const CRightHandedSet*.
// Vtable: 0x01237F1C (CIntelligentPointer<const CThing>, stored in the 8-byte frame temporary).
// Shape: `PThingOrigin = thing;` -- the raw pointer converts to a CIntelligentPointer temporary (base ctor,
// vtable, SetPItem), the inline operator= copies it (SetPItem(other.GetPItem())), the temporary dies
// through the out-of-line base dtor. The position is CThing::GetPosition inlined (placeholder at +0x60,
// position at +0xc); both records are plain implicit copies.
// Layout: PDB NCameraModes::CScriptModuleBase (see 0x005ED070).
struct CBaseObject;

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CThingPlaceholder {
    unsigned char _pad00[0x0c];
    C3DVector     Position;                          // +0x0c
    virtual void v00();
};
struct CThingPlaceholder_V {                         // the placeholder's vtable view
    void* vtbl;
};

struct CPlaceholder {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet* GetRHSet() const; // slot 72 (+0x120)
    float         _pad04[2];
    C3DVector     Position;                          // +0x0c
};

struct CThing {
    unsigned char _pad00[0x60];
    CPlaceholder* m_pPlaceholder;                    // +0x60
    CPlaceholder* GetPlaceholder() const { return m_pPlaceholder; }
    const C3DVector& GetPosition() const { return m_pPlaceholder->Position; }
    const CRightHandedSet* GetRHSet() const { return m_pPlaceholder->GetRHSet(); }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                       // 0x00A01B10
    ~CBaseIntelligentPointer();                      // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CBaseObject* item);          // 0x00A01B90
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {        // vtable 0x01238C6C
    virtual void v00();
};
struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer_CThingConst() {}
    CIntelligentPointer_CThingConst(const CThing* thing) { SetPItem((const CBaseObject*)thing); }
    CIntelligentPointer_CThingConst& operator=(const CIntelligentPointer_CThingConst& other)
    {
        const CBaseObject* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
    const CThing* operator->() const { return (const CThing*)GetPItem(); }
};

struct CBase;
struct CGameCameraManager;

namespace NCameraModes {

struct CScriptModuleBase {                           // vtable 0x01255F74
    virtual void v00();
    CIntelligentPointer_CThing      PCamPoint;       // +0x04
    CIntelligentPointer_CThingConst PThingOrigin;    // +0x0c
    C3DVector                       CoordOrigin;     // +0x14
    CRightHandedSet                 CoordAxis;       // +0x20
    CBase*                          POwner;          // +0x38
    CGameCameraManager*             PCameraManager;  // +0x3c
    bool                            SelfTerminate;   // +0x40
    bool                            ShortenOnCollision; // +0x41
    long                            CutOutOverride;  // +0x44
    float                           FOVOverride;     // +0x48
    void SetPThingOrigin(const CThing* thing);
};

void CScriptModuleBase::SetPThingOrigin(const CThing* thing)
{
    PThingOrigin = thing;
    CPlaceholder* placeholder = PThingOrigin->GetPlaceholder();
    CoordOrigin = placeholder->Position;
    CoordAxis = *placeholder->GetRHSet();
}

}
// ---- end source ----
void CThingPlaceholder::v00() {}
void CPlaceholder::v00() {} void CPlaceholder::v01() {} void CPlaceholder::v02() {} void CPlaceholder::v03() {} void CPlaceholder::v04() {} void CPlaceholder::v05() {} void CPlaceholder::v06() {} void CPlaceholder::v07() {} void CPlaceholder::v08() {} void CPlaceholder::v09() {} void CPlaceholder::v10() {} void CPlaceholder::v11() {} void CPlaceholder::v12() {} void CPlaceholder::v13() {} void CPlaceholder::v14() {} void CPlaceholder::v15() {} void CPlaceholder::v16() {} void CPlaceholder::v17() {} void CPlaceholder::v18() {} void CPlaceholder::v19() {} void CPlaceholder::v20() {} void CPlaceholder::v21() {} void CPlaceholder::v22() {} void CPlaceholder::v23() {} void CPlaceholder::v24() {} void CPlaceholder::v25() {} void CPlaceholder::v26() {} void CPlaceholder::v27() {} void CPlaceholder::v28() {} void CPlaceholder::v29() {} void CPlaceholder::v30() {} void CPlaceholder::v31() {} void CPlaceholder::v32() {} void CPlaceholder::v33() {} void CPlaceholder::v34() {} void CPlaceholder::v35() {} void CPlaceholder::v36() {} void CPlaceholder::v37() {} void CPlaceholder::v38() {} void CPlaceholder::v39() {} void CPlaceholder::v40() {} void CPlaceholder::v41() {} void CPlaceholder::v42() {} void CPlaceholder::v43() {} void CPlaceholder::v44() {} void CPlaceholder::v45() {} void CPlaceholder::v46() {} void CPlaceholder::v47() {} void CPlaceholder::v48() {} void CPlaceholder::v49() {} void CPlaceholder::v50() {} void CPlaceholder::v51() {} void CPlaceholder::v52() {} void CPlaceholder::v53() {} void CPlaceholder::v54() {} void CPlaceholder::v55() {} void CPlaceholder::v56() {} void CPlaceholder::v57() {} void CPlaceholder::v58() {} void CPlaceholder::v59() {} void CPlaceholder::v60() {} void CPlaceholder::v61() {} void CPlaceholder::v62() {} void CPlaceholder::v63() {} void CPlaceholder::v64() {} void CPlaceholder::v65() {} void CPlaceholder::v66() {} void CPlaceholder::v67() {} void CPlaceholder::v68() {} void CPlaceholder::v69() {} void CPlaceholder::v70() {} void CPlaceholder::v71() {}
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointer_CThing::v00() {}
void CIntelligentPointer_CThingConst::v00() {}
void NCameraModes::CScriptModuleBase::v00() {}

static int g_live = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++g_live; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --g_live; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)m_pItem; }
void CBaseIntelligentPointer::SetPItem(const CBaseObject* item) { m_pItem = (void*)item; }
static CRightHandedSet g_rh;
const CRightHandedSet* CPlaceholder::GetRHSet() const { return &g_rh; }
int main()
{
    static CPlaceholder ph; ph.Position.X = 1; ph.Position.Y = 2; ph.Position.Z = 3;
    g_rh.Up.X = 4; g_rh.Up.Y = 5; g_rh.Up.Z = 6; g_rh.Forward.X = 7; g_rh.Forward.Y = 8; g_rh.Forward.Z = 9;
    static unsigned char thingMem[0x64]; CThing* thing = (CThing*)thingMem; thing->m_pPlaceholder = &ph;
    static unsigned char modMem[sizeof(NCameraModes::CScriptModuleBase)];
    NCameraModes::CScriptModuleBase* m = (NCameraModes::CScriptModuleBase*)modMem;
    m->SetPThingOrigin(thing);
    CHECK(m->PThingOrigin.m_pItem == (void*)thing);
    CHECK(m->CoordOrigin.X == 1 && m->CoordOrigin.Y == 2 && m->CoordOrigin.Z == 3);
    CHECK(m->CoordAxis.Up.X == 4 && m->CoordAxis.Up.Z == 6 && m->CoordAxis.Forward.Y == 8 && m->CoordAxis.Forward.Z == 9);
    CHECK(g_live == 0);
    if (fails == 0) printf("00852750_TEST PASS\n"); else printf("00852750_TEST FAIL\n");
    return fails;
}