// Behaviour test for 008d0190: src/008d0190.cpp copied verbatim; the intelligent pointer and the placement stubbed so
// the script update always runs and the position/angle copy happens only for a live thing with a placement.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGameScriptThing::FrameUpdate() -- runs the per-frame virtual update (slot 71), then, when the scripted thing is alive and placed, caches its position (+0x28) and XY angle (+0x40)
// Retry 10b (getsw sweep, FIXES B-15): the placement is read through an inline getter `GetPlacement()`; the plain member read gives the parked register/scheduling difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D0190, length 112 bytes.
// Callees: this's virtual slot 71 (+0x11C), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (five calls), the placement
//          component's virtual GetAngleXY (slot 63, +0xFC, float).
// Shape: the thing's placement record at +0x60 is re-fetched through GetPItem for every use; its position (+0x0c) is
// copied dword by dword through a `lea edx,[edi+28h]` destination (plain C3DVector assignment from a reference).
class C3DVector { public:
    float X, Y, Z;
};
struct CThingPlacement_Retail {                    // the thing's placement / physics record (+0x60)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetAngleXY() const;              // slot 63 (+0xFC)
    unsigned char _pad04[8];
    C3DVector     m_position;                      // +0x0c
};
struct CThing_Retail {
    unsigned char  _pad00[0x60];
    CThingPlacement_Retail* m_pPlacement;          // +0x60
    CThingPlacement_Retail* GetPlacement() const { return m_pPlacement; }
    unsigned char  _pad64[0x2d];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    const C3DVector& GetPosition() const { return m_pPlacement->m_position; }   // inline twin of 0x004C73D0
    float GetAngleXY() const { return m_pPlacement->GetAngleXY(); }
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<CThing>: vptr + item (8 bytes)
    void*          m_pVtbl;
    void*          m_pItem;
    CThing_Retail* GetPItem() const;               // 0x00A01B50
};
class CGameScriptThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70();
    virtual void UpdateScript();                   // slot 71 (+0x11C)
    unsigned char           _pad04[8];
    CBaseIntelligentPointer PThing;                // +0x0c CIntelligentPointer<CThing>
    unsigned char           _pad14[0x14];
    C3DVector               Pos;                   // +0x28
    C3DVector               HomePos;               // +0x34
    float                   AngleXY;               // +0x40
    void FrameUpdate();
};

void CGameScriptThing::FrameUpdate()
{
    UpdateScript();
    if (PThing.GetPItem() && !PThing.GetPItem()->m_dead && PThing.GetPItem()->GetPlacement()) {
        Pos = PThing.GetPItem()->GetPosition();
        AngleXY = PThing.GetPItem()->GetAngleXY();
    }
}
// ---- end source ----
void CThingPlacement_Retail::v00() {} void CThingPlacement_Retail::v01() {} void CThingPlacement_Retail::v02() {} void CThingPlacement_Retail::v03() {} void CThingPlacement_Retail::v04() {} void CThingPlacement_Retail::v05() {} void CThingPlacement_Retail::v06() {} void CThingPlacement_Retail::v07() {} void CThingPlacement_Retail::v08() {} void CThingPlacement_Retail::v09() {} void CThingPlacement_Retail::v10() {} void CThingPlacement_Retail::v11() {} void CThingPlacement_Retail::v12() {} void CThingPlacement_Retail::v13() {} void CThingPlacement_Retail::v14() {} void CThingPlacement_Retail::v15() {} void CThingPlacement_Retail::v16() {} void CThingPlacement_Retail::v17() {} void CThingPlacement_Retail::v18() {} void CThingPlacement_Retail::v19() {} void CThingPlacement_Retail::v20() {} void CThingPlacement_Retail::v21() {} void CThingPlacement_Retail::v22() {} void CThingPlacement_Retail::v23() {} void CThingPlacement_Retail::v24() {} void CThingPlacement_Retail::v25() {} void CThingPlacement_Retail::v26() {} void CThingPlacement_Retail::v27() {} void CThingPlacement_Retail::v28() {} void CThingPlacement_Retail::v29() {} void CThingPlacement_Retail::v30() {} void CThingPlacement_Retail::v31() {} void CThingPlacement_Retail::v32() {} void CThingPlacement_Retail::v33() {} void CThingPlacement_Retail::v34() {} void CThingPlacement_Retail::v35() {} void CThingPlacement_Retail::v36() {} void CThingPlacement_Retail::v37() {} void CThingPlacement_Retail::v38() {} void CThingPlacement_Retail::v39() {} void CThingPlacement_Retail::v40() {} void CThingPlacement_Retail::v41() {} void CThingPlacement_Retail::v42() {} void CThingPlacement_Retail::v43() {} void CThingPlacement_Retail::v44() {} void CThingPlacement_Retail::v45() {} void CThingPlacement_Retail::v46() {} void CThingPlacement_Retail::v47() {} void CThingPlacement_Retail::v48() {} void CThingPlacement_Retail::v49() {} void CThingPlacement_Retail::v50() {} void CThingPlacement_Retail::v51() {} void CThingPlacement_Retail::v52() {} void CThingPlacement_Retail::v53() {} void CThingPlacement_Retail::v54() {} void CThingPlacement_Retail::v55() {} void CThingPlacement_Retail::v56() {} void CThingPlacement_Retail::v57() {} void CThingPlacement_Retail::v58() {} void CThingPlacement_Retail::v59() {} void CThingPlacement_Retail::v60() {} void CThingPlacement_Retail::v61() {} void CThingPlacement_Retail::v62() {}
void CGameScriptThing::v00() {} void CGameScriptThing::v01() {} void CGameScriptThing::v02() {} void CGameScriptThing::v03() {} void CGameScriptThing::v04() {} void CGameScriptThing::v05() {} void CGameScriptThing::v06() {} void CGameScriptThing::v07() {} void CGameScriptThing::v08() {} void CGameScriptThing::v09() {} void CGameScriptThing::v10() {} void CGameScriptThing::v11() {} void CGameScriptThing::v12() {} void CGameScriptThing::v13() {} void CGameScriptThing::v14() {} void CGameScriptThing::v15() {} void CGameScriptThing::v16() {} void CGameScriptThing::v17() {} void CGameScriptThing::v18() {} void CGameScriptThing::v19() {} void CGameScriptThing::v20() {} void CGameScriptThing::v21() {} void CGameScriptThing::v22() {} void CGameScriptThing::v23() {} void CGameScriptThing::v24() {} void CGameScriptThing::v25() {} void CGameScriptThing::v26() {} void CGameScriptThing::v27() {} void CGameScriptThing::v28() {} void CGameScriptThing::v29() {} void CGameScriptThing::v30() {} void CGameScriptThing::v31() {} void CGameScriptThing::v32() {} void CGameScriptThing::v33() {} void CGameScriptThing::v34() {} void CGameScriptThing::v35() {} void CGameScriptThing::v36() {} void CGameScriptThing::v37() {} void CGameScriptThing::v38() {} void CGameScriptThing::v39() {} void CGameScriptThing::v40() {} void CGameScriptThing::v41() {} void CGameScriptThing::v42() {} void CGameScriptThing::v43() {} void CGameScriptThing::v44() {} void CGameScriptThing::v45() {} void CGameScriptThing::v46() {} void CGameScriptThing::v47() {} void CGameScriptThing::v48() {} void CGameScriptThing::v49() {} void CGameScriptThing::v50() {} void CGameScriptThing::v51() {} void CGameScriptThing::v52() {} void CGameScriptThing::v53() {} void CGameScriptThing::v54() {} void CGameScriptThing::v55() {} void CGameScriptThing::v56() {} void CGameScriptThing::v57() {} void CGameScriptThing::v58() {} void CGameScriptThing::v59() {} void CGameScriptThing::v60() {} void CGameScriptThing::v61() {} void CGameScriptThing::v62() {} void CGameScriptThing::v63() {} void CGameScriptThing::v64() {} void CGameScriptThing::v65() {} void CGameScriptThing::v66() {} void CGameScriptThing::v67() {} void CGameScriptThing::v68() {} void CGameScriptThing::v69() {} void CGameScriptThing::v70() {}
static int g_updates = 0; static CThing_Retail* g_item = 0;
void CGameScriptThing::UpdateScript() { ++g_updates; }
float CThingPlacement_Retail::GetAngleXY() const { return 1.25f; }
CThing_Retail* CBaseIntelligentPointer::GetPItem() const { return g_item; }
int main()
{
    static CThingPlacement_Retail placement; placement.m_position.X = 1; placement.m_position.Y = 2; placement.m_position.Z = 3;
    static CThing_Retail thing; memset(&thing, 0, sizeof(thing));
    CGameScriptThing gst; gst.Pos.X = gst.Pos.Y = gst.Pos.Z = 0; gst.AngleXY = 0;
    gst.FrameUpdate(); CHECK(g_updates == 1 && gst.Pos.X == 0);                  // no item
    g_item = &thing;
    gst.FrameUpdate(); CHECK(g_updates == 2 && gst.Pos.X == 0);                  // no placement
    thing.m_pPlacement = &placement; thing.m_dead = 1;
    gst.FrameUpdate(); CHECK(gst.Pos.X == 0);                                    // dead
    thing.m_dead = 0;
    gst.FrameUpdate();
    CHECK(g_updates == 4 && gst.Pos.X == 1 && gst.Pos.Y == 2 && gst.Pos.Z == 3 && gst.AngleXY == 1.25f);
    if (fails == 0) printf("008d0190_TEST PASS\n"); else printf("008d0190_TEST FAIL\n");
    return fails;
}