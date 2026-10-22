// Grade: DIFFER(953v947) -- frame packing 0x40 v 0x34 (retail shares positions with the enemy/distance slot and the valid list with the velocity), constant-1 in bl in the first block where retail keeps ebx = 0 (spilling the enemy pointer), OldPosition address folded v add esi,0x18
// CAIStateGroup_CreatureSeekEnemyUnderground::Update() -- wait out any current action, pick up the targeting component's enemy if the remembered one is gone, then when the enemy is within 1.5x extended sight pick a random valid rise position near where it is heading (at least 1.5 from it) and finish; otherwise idle a frame and rise where the creature stands
// Retail: Fable.exe (TLC, VC7.1) @ 0x00920A00, 960 bytes (no EH frame).
// Callees: CThingCreatureBase::IsPerformingAnyAction 0x006622A0, AIAction_DoNothing 0x0097E200, CAIStateGroupBase::IsStopping
//   0x008FD010 / Terminate 0x008FCF40 / PeekThingSearchTools 0x008FD000 / PeekWorldMap 0x008FCFF0, CBaseIntelligentPointer
//   GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<const CThing> vtable
//   0x01237F1C), CVectorMap::LowerBound const 0x004365B0, CTCTargetingBase slot 29 GetEnemy (+0x74), CTCPerceiveThing::
//   GetExtendedSightRadius 0x007448F0, CThing::GetPosition 0x004C73D0, NCreatureHiding::GetValidPositions 0x0074A5D0
//   (__fastcall), vector<C3DVector>::reserve 0x00920FF0 / push_back 0x0066E630, rand 0x00BFEB16, free 0x00BFEA14.
//   Floats 1.5 @ 0x0125AADC, 15.0 @ 0x0124E640, 2.25 @ 0x0125B2D4.
// Layout: Ego_r.pdb CAIStateGroup_CreatureSeekEnemyUnderground PEnemy +0x14 (CIntelligentPointer<const CThing>), RisePos
//   +0x1c; CThing interface bitset +0x20, map +0x44, PhysicsTC +0x60, flags +0x91; CTCPhysicsBase Position +0x0c,
//   OldPosition +0x18 (PDB); interfaces 8 = CTCTargetingBase, 0x34 = CTCPerceiveThing.
#include <stdlib.h>
#include <math.h>
#include <new>
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const
    {
        C3DVector r;
        r.X = X - o.X;
        r.Y = Y - o.Y;
        r.Z = Z - o.Z;
        return r;
    }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator*(float s) const
    {
        C3DVector r;
        r.X = X * s;
        r.Y = Y * s;
        r.Z = Z * s;
        return r;
    }
    float Get2DLengthSquared() const { return Y * Y + X * X; }
    float Get2DLength() const { return (float)sqrt(Y * Y + X * X); }
};
template <class T> struct CAllocator { CAllocator() { } };
template <class T> struct CAllocProxy : CAllocator<T> {
    T* _M_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), _M_data(p) { }
};
template <class T> struct CVectorBase {         // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
    ~CVectorBase() { if (_M_start) free(_M_start); }       // the CRT free import thunk 0x00BFEA14
};
template <class T> struct CVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
    long size() const { return this->_M_finish - this->_M_start; }
    T& operator[](long n) const { return this->_M_start[n]; }
    void reserve(unsigned int count);           // 0x00920FF0
    void push_back(const T& x);                 // 0x0066E630
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector     Position;                     // +0x0c
    C3DVector     OldPosition;                  // +0x18
};
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x20 - 4];
    unsigned long UsingComponents[9];           // +0x20
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char _pad64[0x91 - 0x64];
    bool Dead : 1;                              // +0x91 bit 0
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction() const;         // 0x006622A0
};
struct CTCTargetingBase {                       // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};
struct CTCPerceiveThing {                       // interface 0x34
    float GetExtendedSightRadius() const;       // 0x007448F0
};
struct CWorldMap;
struct CThingSearchTools;
struct CTCShapeManager;
namespace NCreatureHiding {
    void __fastcall GetValidPositions(CVector<C3DVector>& out, const CWorldMap& map, const CThingSearchTools& tools,
                                      const C3DVector& pos, float radius, bool b, const CTCShapeManager* shapes);   // 0x0074A5D0
}
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing> vtable 0x01237F1C
    virtual void v01();
    __forceinline CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    void Terminate();                           // 0x008FCF40
    bool IsStopping();                          // 0x008FD010
    const CWorldMap& PeekWorldMap();            // 0x008FCFF0
    const CThingSearchTools& PeekThingSearchTools();   // 0x008FD000
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200
};
struct CAIStateGroup_CreatureSeekEnemyUnderground : CAIStateGroupBase {
    CIntelligentPointer<const CThing> PEnemy;   // +0x14
    C3DVector RisePos;                          // +0x1c
    __forceinline float EnemyDistance2D()
    {
        CThingCreatureBase* creature = GetCreature();
        CThing* enemy = PEnemy.GetPItem();
        return (enemy->GetPosition() - creature->GetPosition()).Get2DLength();
    }
    void Update();
};

void CAIStateGroup_CreatureSeekEnemyUnderground::Update()
{
    while (GetCreature()->IsPerformingAnyAction()) {
        if (AIAction_DoNothing() != AI_ACTION_RUNNING || IsStopping()) {
            Terminate();
            return;
        }
    }
    if (!PEnemy.GetPItem() || (PEnemy.GetPItem()->Dead)) {
        CThingCreatureBase* creature = GetCreature();
        CTCTargetingBase* targeting;
        if (creature->HasTC(8))
            targeting = (CTCTargetingBase*)creature->GetTC(8);
        CThing* enemy = targeting->GetEnemy();
        if (enemy && !(enemy->Dead))
            PEnemy = CIntelligentPointer<const CThing>(enemy);
    }
    if (IsStopping()) {
        Terminate();
        return;
    }
    CThingCreatureBase* self = GetCreature();
    CTCPerceiveThing* perceive = 0;
    if (self->HasTC(0x34))
        perceive = (CTCPerceiveThing*)self->GetTC(0x34);
    if (PEnemy.GetPItem() && !(PEnemy.GetPItem()->Dead)) {
        if (EnemyDistance2D() < perceive->GetExtendedSightRadius() * 1.5f) {
            CVector<C3DVector> positions;
            {
                C3DVector velocity = (PEnemy.GetPItem()->PhysicsTC->Position - PEnemy.GetPItem()->PhysicsTC->OldPosition) * 15.0f;
                NCreatureHiding::GetValidPositions(positions, PeekWorldMap(), PeekThingSearchTools(),
                                                   velocity + PEnemy.GetPItem()->GetPosition(), 2.0f, false, 0);
            }
            long count = positions.size();
            CVector<C3DVector> valid;
            valid.reserve(count);
            for (long i = 0; i < count; ++i) {
                if ((positions[i] - PEnemy.GetPItem()->GetPosition()).Get2DLengthSquared() >= 2.25f)
                    valid.push_back(positions[i]);
            }
            long n = valid.size();
            if (n != 0)
                RisePos = valid[rand() % n];
            Terminate();
            return;
        }
    }
    AIAction_DoNothing();
    RisePos = GetCreature()->GetPosition();
}
