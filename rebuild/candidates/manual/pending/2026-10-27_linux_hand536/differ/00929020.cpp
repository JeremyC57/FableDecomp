// CAIStateGroup_BanditPlayDice::IsRunnable() -- once the initiate frame is reached and the village has a dice game: pick the nearest free live dice-game position (a village marker of the group's marker def), runnable when one exists and another player is already seated or the join frame has passed
// Grade: DIFFER(509v507) -- residue: the village member found by the inline GetTC is loaded straight into ecx for GetPVillage in retail (`mov ecx,[eax+4]`); ours loads eax and copies (`mov eax,[eax+4] ; mov ecx,eax`), 2 bytes | tried: 2026-10-21 village local, list reference, forceinline distance helper; 2026-10-28 real-name rewrite (GetTC<CTCVillageMember>, CTCVillageMember::GetPVillage, PeekVillageMarkers list reference, CWorld::GetFrame) with GetTC passing the key as a temporary (fixed the frame, the member slot reuse and the &VillageMarkers list in ebp), scoped member block, const member, real CTCBase base + static_cast, non-const GetPVillage, distance helper (x87 order worse) | next: lane gap in the member register choice; try GetTC result stored through a memory-resident variable form if a landed caller of 0x007EE540 shows one
// Leaf: ?IsRunnable@CAIStateGroup_BanditPlayDice@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00929020, 507 bytes, thiscall, plain `ret` (virtual).
// Name: Ego_r.pdb `?IsRunnable@CAIStateGroup_BanditPlayDice@@MAE_NXZ` (protected virtual).
// Callees: CWorld::GetFrame 0x0049D870 (Ego_r `?GetFrame@CWorld@@SIJXZ`, static fastcall, reads CWorld::Frame), the class's own
//   GetDiceGame / GetMarkerType virtuals (slots 14/15), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline
//   CThing::GetTC<CTCVillageMember> 0x23 and GetTC<CTCExclusiveUse> 0x2F), CTCVillageMember::GetPVillage() const 0x007EE540
//   (Ego_r `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ`; body: PVillageThing +0x10 -> GetTC<CTCVillage> 0x22),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0.
// Layout: this = engine/CAIStateGroup_BanditPlayDice.h (DiceGamePosition +0x14, DiceGame +0x18, FrameToInitiate +0x1c, FrameToJoin
//   +0x20); CTCVillage::VillageMarkers +0x70 (Ego_r +0xEC: STLport lists are 4 bytes, so the 15 lists and the
//   12-byte PlayersCrimes set before it end at +0x70; read through FableWin's inline PeekVillageMarkers() const); CTCExclusiveUse user pointer +0x0c
//   (inline GetUser); CThing DefGlobalIndex +0x8c, Dead +0x91 bit 0.
#define CAIStateGroup_BanditPlayDice CAIStateGroup_BanditPlayDice_HeaderLayout
#include "engine/CAIStateGroup_BanditPlayDice.h"
#undef CAIStateGroup_BanditPlayDice

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return Y * Y + X * X + Z * Z; }
};

class CTCBase;
enum ETCInterfaceType { TC_VILLAGE = 0x22, TC_VILLAGE_MEMBER = 0x23, TC_EXCLUSIVE_USE = 0x2f };
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* n) : _M_node(n) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Tp* operator->() const { return &((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
    bool operator==(const _List_iterator& o) const { return _M_node == o._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> iterator;
    typedef _List_iterator<_Tp> const_iterator;            // (one iterator type is enough for this read-only walk)
    _List_node_base* _M_node;                              // +0x00 (STLport: the list is the one node pointer)
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    iterator LowerBound(const K& key);                     // 0x0040F020
    iterator end() { return this->_M_finish; }
};

class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                         // 0x00A01B50
private:
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
    T* Get() const { return (T*)GetPItem(); }
};

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad_0x04[0x20 - 0x04];
    unsigned long UsingComponents[9];                      // +0x20 bitset<274>
    CVectorMap<ETCInterfaceType, CTCBase*> Components;     // +0x44
    unsigned char _pad_0x50[0x8c - 0x50];
    unsigned short DefGlobalIndex;                         // +0x8c
    unsigned char _pad_0x8e[0x91 - 0x8e];
    bool Dead : 1;                                         // +0x91 bit 0
    bool IsDead() const { return Dead != 0; }
    const C3DVector& GetPos() const;                       // 0x004C73D0
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!(UsingComponents[T::InterfaceType / 32] & (1UL << (T::InterfaceType % 32))))
            return false;
        CVectorMap<ETCInterfaceType, CTCBase*>& components = Components;
        CVectorMap<ETCInterfaceType, CTCBase*>::iterator p = components.LowerBound((ETCInterfaceType)T::InterfaceType);
        if (p == components.end() || (ETCInterfaceType)T::InterfaceType < p->first)
            p = components.end();
        *out = (T*)p->second;
        return true;
    }
};
class CThingAICreature : public CThing { };
template <class T> inline bool IS_THING_ALIVE(const T& thing)
{
    if (thing.GetPItem() != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}

class CTCExclusiveUse {
public:
    enum { InterfaceType = TC_EXCLUSIVE_USE };
    unsigned char               _pad00[0x0c];
    CIntelligentPointer<CThing> PUser;                     // +0x0c
    CThing* GetUser() const { return PUser.Get(); }
};
class CThingMarker : public CThing { };
class CTCVillage {
public:
    enum { InterfaceType = TC_VILLAGE };
    unsigned char _pad00[0x70];
    std::list<CIntelligentPointer<CThingMarker> > VillageMarkers;   // +0x70
    const std::list<CIntelligentPointer<CThingMarker> >& PeekVillageMarkers() const { return VillageMarkers; }   // FableWin inline
};
class CTCVillageMember {
public:
    enum { InterfaceType = TC_VILLAGE_MEMBER };
    CTCVillage* GetPVillage() const;                       // 0x007EE540
};
class CTCDiceGame;
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870

class CAIBrain {
public:
    unsigned char     _pad00[0x18];
    CThingAICreature* Creature;                            // +0x18
    CThingAICreature& GetCreature() const { return *Creature; }
};
#define VS(n) virtual void _vslot##n();
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    VS(1) VS(2) VS(3) VS(4) VS(5) VS(6) VS(7) VS(8) VS(9) VS(10) VS(11) VS(12) VS(13)
    CAIBrain* PBrain;                                      // +0x04
    unsigned char _pad08[0x14 - 0x08];
protected:
    CThingAICreature& GetCreature() const { return PBrain->GetCreature(); }
};
class CAIStateGroup_BanditPlayDice : public CAIStateGroupBase {
protected:
    virtual CTCDiceGame* GetDiceGame();                    // slot 14 (+0x38), 0x00929220
    virtual long GetMarkerType();                          // slot 15 (+0x3c), 0x00928550
    virtual bool IsRunnable();
public:
    CTCExclusiveUse* DiceGamePosition;                     // +0x14
    CTCDiceGame*     DiceGame;                             // +0x18
    long             FrameToInitiate;                      // +0x1c
    long             FrameToJoin;                          // +0x20
    bool             WasCarryingWeapon;                    // +0x24
};

bool CAIStateGroup_BanditPlayDice::IsRunnable()
{
    if (CWorld::GetFrame() < FrameToInitiate)
        return false;
    DiceGame = GetDiceGame();
    if (!DiceGame)
        return false;
    bool position_taken = false;
    CThing* best = 0;
    float best_distance = 0.0f;
    long marker_type = GetMarkerType();
    CTCVillageMember* member;
    if (!GetCreature().GetTC(&member))
        return false;
    CTCVillage* village = member->GetPVillage();
    const std::list<CIntelligentPointer<CThingMarker> >& markers = village->PeekVillageMarkers();
    for (std::list<CIntelligentPointer<CThingMarker> >::const_iterator it = markers.begin(); it != markers.end(); ++it) {
        if (IS_THING_ALIVE(*it) && it->Get()->DefGlobalIndex == marker_type) {
            CTCExclusiveUse* position;
            it->Get()->GetTC(&position);
            if (position->GetUser()) {
                position_taken = true;
            } else {
                float distance = (it->Get()->GetPos() - GetCreature().GetPos()).GetLengthSquared();
                if (!best || distance < best_distance) {
                    best = it->Get();
                    best_distance = distance;
                    DiceGamePosition = position;
                }
            }
        }
    }
    if (best && (position_taken || CWorld::GetFrame() > FrameToJoin))
        return true;
    return false;
}
