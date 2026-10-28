// CTCHighlightItem::DoMovement() -- bob the highlighted item above the ground under its start position (a smoothed
// Grade: DIFFER(502v502) -- one operand order: the bounding-box centre sum loads MaxCoord.Z first (fld [eax+24h] ; fadd [eax+18h]) where retail loads MinCoord.Z first | tried (2026-10-21) operator+ member/free/copy-and-add, (Max + Min), component-wise, Min + (Max - Min) * 0.5f; (2026-10-28 retry, 3 grades) FableWin C3DBoundingBox::GetCenter() const returning `const C3DVector` as (Min + Max) * 0.5f, / 2.0f, copy += / *=, 0.5f * (..) per component and as a free operator*, Max - (Max - Min) * 0.5f, PeekMinCoord/PeekMaxCoord reference accessors, a box reference local with a float centreZ (VC7.1 still loads the higher offset first), the Z term inline in the final expression | next: lane gap (x87 operand order of a commutative add over one base register); FableWin line info for GetCenter
// Leaf: ?DoMovement@CTCHighlightItem@@IAEXXZ
// half-cosine over BobTime seconds, its mesh centred on the bob height) and spin it by RotationSpeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077C960, 502 bytes, plain `ret`.
#include <math.h>
#include "fable_string.h"

struct CRefCounted {                                 // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void Release() { if (--m_count == 0) Destroy(); }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointerBase {                             // CDefPointer<CDefBase>: the manager's return type
    CDefBase* m_p;
    CDefPointerBase() { }
    ~CDefPointerBase() { if (m_p) { m_p->ReleaseTemporary(); m_p = 0; } }
};
struct CHighlightItemDef : CDefBase {
    unsigned char _pad_0x08[0x20];
    long          ParticleEmitter;                   // +0x28
    long          PickupParticleEmitter;             // +0x2C
    float         RotationSpeed;                     // +0x30
    float         DistanceFromGround;                // +0x34
    float         BobDistance;                       // +0x38
    float         BobTime;                           // +0x3C
};
struct CHighlightItemDefPointer {                    // CDefPointer<const CHighlightItemDef>
    CHighlightItemDef* m_p;
    CHighlightItemDefPointer() : m_p(0) { }
    CHighlightItemDefPointer(const CDefPointerBase& other) : m_p((CHighlightItemDef*)other.m_p) { if (m_p) ++m_p->m_count; }
    ~CHighlightItemDefPointer() { if (m_p) m_p->Release(); }
    CHighlightItemDef* operator->() const { return m_p; }
    CHighlightItemDefPointer& operator=(const CHighlightItemDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) { m_p->Release(); m_p = 0; }
            m_p = other.m_p;
            if (m_p) ++m_p->m_count;
        }
        return *this;
    }
};
struct CDefinitionManager {
    CDefPointerBase GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad[0x1c - 4];
    CDefinitionManager* m_pManager;                  // +0x1C
    bool GetSubDef(CHighlightItemDefPointer& out);   // 0x0077D080
};

// Stand-in body of GetSubDef<CHighlightItemDef> (0x0077D080, the landed 0x0077E430 shape): compiled above the
// caller with auto_inline off so VC7.1 knows the out pointer does not escape. Only DoStartRunning is graded.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CHighlightItemDefPointer& out)
{
    long* pIndex;
    {
        CCharString name("CHighlightItemDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CHighlightItemDefPointer result(m_pManager->GetPDefFromGlobalIndex(*pIndex));
        out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)


const double PI = 3.14159265358979323846;
inline float CosineEase(float t)
{
    return (1.0 - cos(t * 0.5f * PI)) * 0.5;
}

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    void RotateAboutAxis(const C3DVector& axis, float angle);   // 0x00A22F80
};
struct CThingPlacement {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void SetPosition(const C3DVector& position);       // slot 30 (+0x78)
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual const CRightHandedSet& GetOrientation() const;      // slot 72 (+0x120)
    virtual void v73();
    virtual void v74();
    virtual void SetOrientation(const CRightHandedSet& rhs);    // slot 75 (+0x12c)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector     Position;                                     // +0x0c
    void SetAsMoving(bool moving);                              // 0x006B0130 (CTCPhysicsBase)
};
struct CThingDrawState {
    unsigned char _pad00[0x34];
    float         Scale;                                        // +0x34
    unsigned char _pad38[0x44 - 0x38];
    long          GraphicIndex;                                 // +0x44
    unsigned char _pad48[0x4c - 0x48];
    float         ScalePercent;                                 // +0x4c
};
struct CThing {
    unsigned char    _pad_0x00[0x60];
    CThingPlacement* PPlacement;                                // +0x60
    CThingDrawState* PDrawState;                                // +0x64
    unsigned char    _pad_0x68[0x70 - 0x68];
    CDefClassBase*   PDef;                                      // +0x70 CDefPointer<const CThingDef>::Object
};
struct C3DBoundingBox {                                         // Ego_r.pdb, 24 bytes
    C3DVector MinCoord; C3DVector MaxCoord;
    // FableWin.pdb ?GetCenter@C3DBoundingBox@@QBE?BVC3DVector@@XZ (inline here)
    const C3DVector GetCenter() const { return (MinCoord + MaxCoord) * 0.5f; }
};
struct C3DMeshInfo {                                            // Ego_r.pdb: CMeshDataBankEntry base (vptr, Bank, Index), MeshData +0x0c
    unsigned char  _pad00[0x10];
    C3DBoundingBox BoundingBox;                                 // +0x10
};
struct CMeshDataBank {
    const C3DMeshInfo* PeekMeshInfo(unsigned long index) const; // 0x00A24260 (Ego_r.pdb; the second of the eight Peek accessors 0xA24250..)
};
struct CWorld {
    unsigned char _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;                               // +0x3c CCountedPointer<CMeshDataBank>::Data (engine/CWorld.h)
};
struct CThingFilterBase {
    virtual bool Accept(CThing* thing);
};
struct CThingFilterAll : CThingFilterBase {                     // vtable 0x0126AF08
    CThingFilterAll() { }
    virtual bool Accept(CThing* thing);
};
struct CWorldMap {
    float GetHeightZBelowAt(const C3DVector& position, float offset, bool flag, CThingFilterBase* filter);   // 0x0063E950
};
extern long g_FramesPerSecond;                                  // 0x01375550

struct CTCHighlightItem {
    virtual void v00();
    CThing*       ParentThing;             // +0x04 CTCBase
    unsigned char _pad_0x08[4];            // +0x08 CTCBase flags
    bool          StartRunning;            // +0x0C
    bool          Running;                 // +0x0D
    long          LifeTimer;               // +0x10
    C3DVector     StartPos;                // +0x14
    unsigned char PParticleEmitter[8];     // +0x20 CIntelligentPointer<CThing>
    CWorld* GetWorld() const;              // 0x00686A70 (CTCBase)
    CWorldMap* GetWorldMap() const;        // 0x00686D20 (CTCBase)
    void DoMovement();
};

void CTCHighlightItem::DoMovement()
{
    CHighlightItemDefPointer def;
    ParentThing->PDef->GetSubDef(def);
    CThingPlacement* placement = ParentThing->PPlacement;
    C3DVector position = placement->Position;
    float groundZ;
    {
        CThingFilterAll filter;
        groundZ = GetWorldMap()->GetHeightZBelowAt(StartPos, 0.0f, false, &filter);
    }
    C3DVector newPosition = position;
    float period = g_FramesPerSecond * def->BobTime;
    float half = period * 0.5f;
    float phase = fmod((float)LifeTimer, period);
    float bob;
    if (phase > half)
        bob = 1.0f - CosineEase((phase - half) / half);
    else
        bob = CosineEase(phase / half);
    CThingDrawState* drawState = ParentThing->PDrawState;
    float scale = drawState->ScalePercent * drawState->Scale * 0.01f;
    long graphicIndex = drawState->GraphicIndex;
    const CMeshDataBank* bank = GetWorld()->PMainMeshBank;
    C3DVector centre = bank->PeekMeshInfo(graphicIndex)->BoundingBox.GetCenter();
    newPosition.Z = def->DistanceFromGround - def->BobDistance * 0.5f + bob * def->BobDistance + groundZ - centre.Z * scale;
    placement->SetPosition(newPosition);
    CRightHandedSet rhs = placement->GetOrientation();
    rhs.RotateAboutAxis(C3DVector(0.0f, 0.0f, 1.0f), def->RotationSpeed * (1.0f / 360.0f));
    placement->SetOrientation(rhs);
    placement->SetAsMoving(true);
}
