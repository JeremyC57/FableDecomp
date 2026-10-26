// CAIStateGroup_SeekEnemy::IsRunnable() -- runnable for a live enemy the creature is willing to attack: when it is the perceived thing the creature must be aware of it and have seen it within 15 seconds, otherwise the creature must be able to see it; latch the enemy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093B740, length 368 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface 8, bit 8 of +0x20, ternary pick into ecx for the
//          tracker's virtual slot 29 GetEnemy; 0x34 = CTCPerceiveThing, bit 20 of +0x24), IsWillingToAttackTarget
//          @ 0x008648A0 (__fastcall), CTCPerceiveThing::GetPerceivedThing @ 0x007446A0 / IsAwareOfThing @ 0x00643B3B,
//          CanCreatureSeeThing @ 0x008645E0 (__fastcall + a bool; manifest "CMainGameComponent::BeginInputLoading" is
//          wrong), CIntelligentPointer(CThing*) @ 0x004A96F0 and operator= @ 0x004A9760 (out of line at the first
//          site), CBaseIntelligentPointer GetPItem/SetPItem/dtor.
// Globals: FramesPerSecond @ 0x01375550, 15.0f @ 0x012925B8.
// Shape: the first assignment is an implicit conversion into the out-of-line operator=; the second site inlines it
//          (a __forceinline helper fed by an implicit conversion, so the temporary address is re-taken); the frame count is stored into the dead key slot for the fild.
// Layout: engine/CAIStateGroup_SeekEnemy.h (PEnemy +0x14); PBrain is the first dword at +0x04; the perception
//          component's +0x1c frame count is descriptive.
#include "engine/CAIStateGroup_SeekEnemy.h"

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
struct CTCEnemyTracker {                        // interface 8
    V16(a) V4(b) V4(c)
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};
class CTCPerceiveThing { public:                       // interface 0x34
    unsigned char _pad[0x1c];
    long FramesSinceSeen;                       // +0x1c (descriptive)
    CThing* GetPerceivedThing();                // 0x007446A0
public:
    bool IsAwareOfThing() const;   // 0x00643B3B
};
bool __fastcall IsWillingToAttackTarget(CThing* creature, CThing* target);           // 0x008648A0
bool __fastcall CanCreatureSeeThing(CThing* creature, CThing* target, bool flag);    // 0x008645E0
extern long FramesPerSecond;                    // 0x01375550

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item);     // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
    __forceinline void AssignInline(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupSeekEnemyMethods : CAIStateGroup_SeekEnemy {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CIntelligentPointer<CThing>& Enemy() { return *(CIntelligentPointer<CThing>*)PEnemy; }
    bool IsRunnable();
};

bool CAIStateGroupSeekEnemyMethods::IsRunnable()
{
    CTCEnemyTracker* tracker;
    if (((const CThing*)GetCreature())->GetInterfaceX(&tracker, 8)) {
        CThing* enemy = tracker->GetEnemy();
        if (enemy && !(enemy->m_flags91 & 1) && IsWillingToAttackTarget(GetCreature(), enemy)) {
            const CThing* self = GetCreature();
            if (self->HasTC(0x34)) {
                CTCPerceiveThing* perceive = self->GetTC<CTCPerceiveThing>(0x34);
                if (perceive->GetPerceivedThing() == enemy) {
                    if (perceive->IsAwareOfThing()) {
                        long frames = perceive->FramesSinceSeen;
                        if (frames != -1 && (float)frames < (float)FramesPerSecond * 15.0f) {
                            Enemy() = enemy;
                            return true;
                        }
                    }
                    return false;
                }
            }
            if (CanCreatureSeeThing(GetCreature(), enemy, true)) {
                Enemy().AssignInline(enemy);
                return true;
            }
        }
    }
    return false;
}