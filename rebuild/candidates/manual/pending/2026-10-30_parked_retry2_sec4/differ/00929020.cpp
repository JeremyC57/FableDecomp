// CAIStateGroup_BanditPlayDice::IsRunnable() -- once the initiate frame is reached and the village has a dice game: pick the nearest free live dice-game position (marker def) of the creature's village, runnable when one exists and another player is already seated or the join frame has passed
// Grade: DIFFER(509v507) nd 43->9 -- levers: PDB respell CWorld::GetFrame (static __fastcall 0x0049D870) and the village cage list taken as a reference `CThingList& cages = GFGetPVillageInterface(villager)->VillageCages;` (retail keeps &VillageCages in ebp). Residue: the villager component loads into eax then mov ecx,eax (retail straight into ecx, +2 bytes) and the distance sum adds the first two squares before the third (retail). Tried the ternary GetInterface form (48), GetLengthSquared X/Y/Z orders and a sequenced sum (9-13). Next: GFGetPVillageInterface argument shape
// Leaf: IsRunnable@CAIStateGroup_BanditPlayDice
#include "engine/CAIStateGroup_BanditPlayDice.h"
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return Y * Y + X * X + Z * Z; }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short DefGlobalIndex;              // +0x8c (PDB +0x9c; retail CThing is 0x10 smaller)
    unsigned char _pad8e[0x91 - 0x8e];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
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
__forceinline float GetDistanceSquared(const CThing* a, const CThing* b)
{
    return (a->GetPosition() - b->GetPosition()).GetLengthSquared();
}
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTCExclusiveUse {                        // interface 0x2f
    unsigned char _pad[0xc];
    CBaseIntelligentPointer User;               // +0x0c
};
struct CThingListNode { CThingListNode* Next; CThingListNode* Prev; CBaseIntelligentPointer Value; };
struct CThingList { CThingListNode* Head; unsigned long Size; unsigned long _alloc; };
struct CTCVillage {
    unsigned char _pad[0x70];
    CThingList VillageCages;                    // +0x70 (PDB name; the dice-game marker things)
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 (Ego_r.pdb ?GetFrame@CWorld@@SIJXZ)
struct CAIBrain { unsigned char _pad[0x18]; CThing* Creature; };
struct CDiceDispatch {                          // the state group's own vtable (0x0128AD54)
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13();
    virtual CTCDiceGame* GetDiceGame();         // slot 14 (+0x38) 0x00929220
    virtual long GetMarkerType();               // slot 15 (+0x3c) 0x00928550
};
struct DiceMethods : CAIStateGroup_BanditPlayDice {
    bool IsRunnable();
    __forceinline CThing* GetCreature() { return (*(CAIBrain**)_pad_0x04)->Creature; }
};
bool DiceMethods::IsRunnable()
{
    if (CWorld::GetFrame() < FrameToInitiate)
        return false;
    DiceGame = ((CDiceDispatch*)this)->GetDiceGame();
    if (!DiceGame)
        return false;
    bool positionTaken = false;
    CThing* best = 0;
    float bestDistance = 0.0f;
    long markerType = ((CDiceDispatch*)this)->GetMarkerType();
    CTCVillager* villager;
    if (!GetCreature()->GetInterface(&villager, 0x23))
        return false;
    CThingList& cages = GFGetPVillageInterface(villager)->VillageCages;
    for (CThingListNode* node = cages.Head->Next; node != cages.Head; node = node->Next) {
        if (node->Value.GetPItem() && !(node->Value.GetPItem()->m_flags91 & 1)
            && node->Value.GetPItem()->DefGlobalIndex == markerType) {
            CTCExclusiveUse* position;
            node->Value.GetPItem()->GetInterface(&position, 0x2f);
            if (position->User.GetPItem()) {
                positionTaken = true;
            } else {
                float distance = GetDistanceSquared(node->Value.GetPItem(), GetCreature());
                if (!best || distance < bestDistance) {
                    best = node->Value.GetPItem();
                    bestDistance = distance;
                    DiceGamePosition = position;
                }
            }
        }
    }
    if (best && (positionTaken || CWorld::GetFrame() > FrameToJoin))
        return true;
    return false;
}
