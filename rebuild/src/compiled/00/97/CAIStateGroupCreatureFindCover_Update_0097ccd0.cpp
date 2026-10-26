// CAIStateGroup_CreatureFindCover::Update() -- take cover from a live enemy: re-aware the perception of it, pick a hidden cover position, move in and keep ducking while close to the cover
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097CCD0, length 351 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, Terminate @ 0x008FCF40, CVectorMap::LowerBound @ 0x0040F020 (inlined
//          GetInterface 0x34 = CTCPerceiveThing, bit 20 of +0x24), CTCPerceiveThing::GetPerceivedThing @ 0x007446A0 (manifest
//          "_Cons_val<...>" is propagated) and SetAsDirectlyAwareOfPerceivedThing @ 0x007449B0, GetHiddenCoverPosAndSetUpCoverThing
//          @ 0x00970CD0, SetFailedToFindCover @ 0x00970590, MoveIntoCover @ 0x009711D0, DuckBehindCover(bool) @ 0x009713E0,
//          GetHiddenCoverPos(C3DVector&, CThing*) @ 0x00970930, CThing::GetPosition @ 0x004C73D0, IsStillRunnable @ 0x0097CB10.
// Globals: 1.0f @ 0x0122DED8.
// Shape: the enemy is fetched before the perception's target call (ebp) for the comparison; after moving into cover the
//          group ducks while the cover and the enemy live, the hidden cover position stays within 1.0 of the creature (XY,
//          a named float from fsqrt) and the group is still runnable; every exit terminates.
// Layout: engine/CAIStateGroup_CreatureUseCoverBase.h (pack(1): enemy +0x14, PCoverThing +0x1F) behind engine/CAIStateGroupBase.h.
#define CAIStateGroup_CreatureUseCoverBase CAIStateGroup_CreatureUseCoverBaseLayout
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#undef CAIStateGroup_CreatureUseCoverBase

class CAIStateGroupBase
{
public:
    void Terminate();   // 0x008FCF40
};
class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase
{
public:
};
class CAIStateGroup_CreatureUseCoverBase : public CAIStateGroup_EnemyCreatureBase, public CAIStateGroup_CreatureUseCoverBaseLayout
{
};

extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float x, y;
    float GetLength() const { return (float)sqrt(x * x + y * y); }
};

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CTCPerceiveThing { public:                       // interface 0x34
    CThing* GetPerceivedThing();                // 0x007446A0
    void SetAsDirectlyAwareOfPerceivedThing();  // 0x007449B0
};
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
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
__forceinline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
__forceinline bool IS_THING_ALIVE(CThing* const& thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CIntelligentPointerVector {              // vector<CIntelligentPointer<CThing> > (retail: first, last, end of storage)
    CIntelligentPointer<CThing>* first;
    CIntelligentPointer<CThing>* last;
    CIntelligentPointer<CThing>* endOfStorage;
    unsigned long size() const { return (unsigned long)(last - first); }
    void erase(CIntelligentPointer<CThing>* from, CIntelligentPointer<CThing>* to);   // 0x004AC0F0
    void clear() { erase(first, last); }
};


class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupCreatureFindCoverMethods : CAIStateGroup_CreatureUseCoverBase {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CIntelligentPointer<CThing>& Enemy() { return *(CIntelligentPointer<CThing>*)_pad_0x14; }
    CIntelligentPointer<CThing>& Cover() { return *(CIntelligentPointer<CThing>*)PCoverThing; }
    bool GetHiddenCoverPosAndSetUpCoverThing(C3DVector& result);   // 0x00970CD0
    void SetFailedToFindCover();                // 0x00970590
    bool MoveIntoCover(C3DVector& pos);         // 0x009711D0
    bool DuckBehindCover(bool dive);            // 0x009713E0
    bool GetHiddenCoverPos(C3DVector& result, CThing* cover);   // 0x00970930
    bool IsStillRunnable();                     // 0x0097CB10
    void Update();
};

void CAIStateGroupCreatureFindCoverMethods::Update()
{
    if (!IS_THING_ALIVE(Enemy())) {
        Terminate();
        return;
    }
    CTCPerceiveThing* perception;
    if (GetCreature()->GetInterface(&perception, 0x34)) {
        CThing* enemy = Enemy().GetPItem();
        if (perception->GetPerceivedThing() == enemy)
            perception->SetAsDirectlyAwareOfPerceivedThing();
    }
    C3DVector coverPos;
    if (!GetHiddenCoverPosAndSetUpCoverThing(coverPos)) {
        SetFailedToFindCover();
        Terminate();
        return;
    }
    if (MoveIntoCover(coverPos)) {
        do {
            DuckBehindCover(true);
            if (!IS_THING_ALIVE(Cover().GetPItem()))
                break;
            if (!IS_THING_ALIVE(Enemy()))
                break;
            C3DVector hidden;
            GetHiddenCoverPos(hidden, Cover().GetPItem());
            const C3DVector& pos = GetCreature()->GetPosition();
            C2DVector delta;
            delta.x = hidden.X - pos.X;
            delta.y = hidden.Y - pos.Y;
            float distance = delta.GetLength();
            if (distance > 1.0f)
                break;
        } while (IsStillRunnable());
    }
    Terminate();
}