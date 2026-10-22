// Grade: DIFFER(516v516) -- esi/edi swapped throughout (this in esi, retail edi); everything else matches
// CAIStateGroup_CreatureUseCoverBase::MoveIntoCover(const C3DVector&) -- dive into cover when 2.4..2.8 units away (and not strafing), otherwise strafe there firing at the target; counts the successful arrivals
// Retail: Fable.exe (TLC, VC7.1) @ 0x009711D0, 528 bytes (ret 4).
// Callees: CVectorMap::LowerBound const 0x004365B0 (same-TU stand-in; inlined GetInterface 0x31 = CTCCreatureModeManager),
//   CTCCreatureModeManager::HasMode 0x0073A8A0, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem
//   0x00A01B90, CThing::GetPosition 0x004C73D0, CCreatureAction_DuckAndCover(CThingCreatureBase*, const C3DVector&, bool)
//   0x009717A0 / dtor 0x0062E900 (catalog "~CTCLightningOrb", propagated), CAIStateGroupBase::AIAction_PerformActionAndWait
//   0x0097DB70 / AIAction_ProjectileAttackAndStrafeToPos 0x0097F3A0 / Terminate 0x008FCF40 (Terminated = true; catalog
//   "CXMVPlaybackInfo", propagated) / GetPWorld 0x008FCFC0 (catalog "CPlayer::GetCurrentMode", propagated).
// Layout: CAIStateGroupBase (engine header, 0x14); CAIStateGroup_EnemyCreatureBase::PTargetThing +0x14 (Ego_r.pdb);
// CAIStateGroup_CreatureUseCoverBase HasDiveAnim +0x1c, TimingCritical +0x1e (Ego_r.pdb), ValidPositionsFound +0x48
// (PDB +0x4c less 4: the retail STLport CoverList vector is 12 bytes).
#include <math.h>
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float Get2DLength() const { return sqrtf(X * X + Y * Y); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
inline float Get2DDistance(const C3DVector& a, const C3DVector& b)
{
    float dy = a.Y - b.Y;
    float dx = a.X - b.X;
    float d2 = dx * dx + dy * dy;
    return sqrtf(d2);
}
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CTCCreatureModeManager {                 // interface type 0x31
    bool HasMode(unsigned long mode);           // 0x0073A8A0
};
struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType) const
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
        return has;
    }
};
struct CThingCreatureBase : CThing { };

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {    // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCreatureActionBase {
    virtual void v00();
};
struct CCreatureAction_DuckAndCover : CCreatureActionBase {
    unsigned char _body[0x120 - 4];
    CCreatureAction_DuckAndCover(CThingCreatureBase* creature, const C3DVector& direction, bool flag);   // 0x009717A0
    ~CCreatureAction_DuckAndCover();            // 0x0062E900
};
struct CWorldLocator {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28();
    virtual const C3DVector* GetPosition(C3DVector* scratch);   // slot 11 (+0x2c)
};
struct CWorld {
    unsigned char _pad00[0x34];
    CWorldLocator* PLocator;                    // +0x34
    CWorldLocator* GetLocator() const { return PLocator; }
};
inline float Get2DDistanceTo(const CThing* thing, const C3DVector* pos)
{
    return Get2DDistance(thing->GetPosition(), *pos);
}
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT, MOVEMENT_RUN = 5 };

struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    void Terminate();                           // 0x008FCF40
    CWorld* GetPWorld();                        // 0x008FCFC0
    EAIActionReturn AIAction_PerformActionAndWait(CCreatureActionBase* action);   // 0x0097DB70
    EAIActionReturn AIAction_ProjectileAttackAndStrafeToPos(EAIActionRepeat repeat, C3DVector pos,
                                                           CIntelligentPointerConstThing target, bool& speed,
                                                           EMovementType anim, bool extra);   // 0x0097F3A0
};
struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CIntelligentPointerCreature PTargetThing;   // +0x14
};
struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroup_EnemyCreatureBase {
    bool HasDiveAnim;                           // +0x1c
    bool HasJumpOutAnims;                       // +0x1d
    bool TimingCritical;                        // +0x1e
    unsigned char _pad1f[0x48 - 0x1f];
    long ValidPositionsFound;                   // +0x48
    bool MoveIntoCover(const C3DVector& coverPos);
};

bool CAIStateGroup_CreatureUseCoverBase::MoveIntoCover(const C3DVector& coverPos)
{
    TimingCritical = true;
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterfaceT(&modes, 0x31);
    bool strafing = modes->HasMode(0x19);
    EAIActionReturn result;
    bool moving;
    do {
        if (!IS_THING_ALIVE(PTargetThing)) {
            Terminate();
            TimingCritical = false;
            return false;
        }
        C3DVector offset = coverPos - GetCreature()->GetPosition();
        float distance = offset.Get2DLength();
        if (!strafing && HasDiveAnim && distance > 2.4f && distance < 2.8f) {
            result = AIAction_PerformActionAndWait(&CCreatureAction_DuckAndCover(GetCreature(), offset, false));
        } else {
            C3DVector scratch;
            result = AIAction_ProjectileAttackAndStrafeToPos(AI_ACTION_REPEAT_POLL, coverPos, PTargetThing, moving,
                                                            MOVEMENT_RUN, Get2DDistance(GetCreature()->GetPosition(), *GetPWorld()->GetLocator()->GetPosition(&scratch)) < 10.0f);
        }
    } while (result == AI_ACTION_RUNNING);
    TimingCritical = false;
    if (result == AI_ACTION_DONE) {
        ++ValidPositionsFound;
        return true;
    }
    return false;
}
