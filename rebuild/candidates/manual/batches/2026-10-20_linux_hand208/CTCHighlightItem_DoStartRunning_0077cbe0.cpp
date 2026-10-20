// CTCHighlightItem::DoStartRunning() -- spawns the item's highlight particle emitter just above the thing, keeps it, and switches the highlight to running
// Leaf: DoStartRunning
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077CBE0, 186 bytes.
// Callees: CDefClassBase::GetSubDef<CHighlightItemDef>(CDefPointer&) @ 0x0077D080 (same TU; see the stand-in
//          below), CThing::GetPosition() const @ 0x004C73D0, CreateEffectThing(long, const C3DVector*, bool)
//          @ 0x006E0880 (__fastcall), CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, ::SetPItem
//          @ 0x00A01B90 (x2), ::GetPItem @ 0x00A01B50, ::~CBaseIntelligentPointer @ 0x00A01C10; CIntelligentPointer
//          <CThing> vtable 0x01238C6C.
// Shape: the highlight def comes through the owner's def (CThing +0x70) into a CDefPointer local that VC keeps in
// edi for the whole body -- it can, because GetSubDef's body sits earlier in the same TU (it does not leak the
// local's address); the release at the end has no null test (the def was dereferenced). The spawn position is a
// by-value copy of GetPosition() raised by DistanceFromGround; the emitter is stored through the landed
// "assign through a stack CIntelligentPointer" shape (construct, SetPItem, member SetPItem(GetPItem()), destroy).
// Layout (Ego_r.pdb, matches retail): CTCHighlightItem StartRunning +0x0C, Running +0x0D, LifeTimer +0x10,
// PParticleEmitter +0x20; CHighlightItemDef ParticleEmitter +0x28, DistanceFromGround +0x34. The class is a local
// model: engine/CTCHighlightItem.h flattens the CTCBase vptr and has the intelligent pointer as raw bytes.
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

struct C3DVector {
    float X, Y, Z;
};

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                             // +0x70 CDefPointer<const CThingDef>::Object
    const C3DVector& GetPosition() const;            // 0x004C73D0
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                 // +4
    CBaseIntelligentPointer();                       // 0x00A01B10
    ~CBaseIntelligentPointer();                      // 0x00A01C10
    CThing* GetPItem() const;                        // 0x00A01B50
    void SetPItem(CThing* item);                     // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CTCHighlightItem {
    virtual void v00();
    CThing*                 ParentThing;             // +0x04 CTCBase
    unsigned char           _pad_0x08[4];            // +0x08 CTCBase flags
    bool                    StartRunning;            // +0x0C
    bool                    Running;                 // +0x0D
    long                    LifeTimer;               // +0x10
    C3DVector               StartPos;                // +0x14
    CBaseIntelligentPointer PParticleEmitter;        // +0x20 CIntelligentPointer<CThing>
    void DoStartRunning();
};

void CTCHighlightItem::DoStartRunning()
{
    CHighlightItemDefPointer def;
    ParentThing->PDef->GetSubDef(def);
    if (def->ParticleEmitter) {
        C3DVector position = ParentThing->GetPosition();
        position.Z += def->DistanceFromGround;
        CThing* emitter = CreateEffectThing(def->ParticleEmitter, &position, false);
        CIntelligentPointer pointer;
        pointer.SetPItem(emitter);
        PParticleEmitter.SetPItem(pointer.GetPItem());
    }
    LifeTimer = 0;
    StartRunning = false;
    Running = true;
}
