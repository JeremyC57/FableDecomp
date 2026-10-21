// CCreatureAction_NavToPos::FrameUpdate() -- ask the creature's navigation component for the next position on the route to
// Grade: DIFFER(423v421) -- register choice: retail keeps &PCreature in ebx (ours ebp) and loads the navigation component straight into ecx while copying NavPos through edx/eax (ours eax then mov ecx,eax, copying through ecx/edx); flow, tail-duplicated base FrameUpdate, frame layout (named CNavigationInput) match; tried switch, temp input, member-init copy
// NavPos: on failure (0) move to where it stands and give up; on a step (2) move to position + step and, unless the player
// auto-faces, look at it (0x11); on arrival (1) move to where it stands and give up; then the base FrameUpdate
// Leaf: ?FrameUpdate@CCreatureAction_NavToPos
// Layout: retail CCreatureActionBase is 0xa8 (Ego_r.pdb +0xc): PreviousPhysicsComponent +0xa8, NavPos +0xac (PDB +0xb8);
//   CThing interface bitset +0x20, map +0x44; CPlayer auto-face flag +0x20a.
// Shape: the navigation lookup reads its out pointer unconditionally (same-TU LowerBound stand-in: the key word is the out
//   pointer's word); the result switch duplicates the base FrameUpdate tail.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CPlayer {
    unsigned char _pad[0x20a];
    unsigned char m_autoFaceTarget;                   // +0x20a
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                   // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                       // +0x44
    const C3DVector& GetPosition() const;             // 0x004C73D0
    CPlayer* GetPlayer() const;                       // 0x004C7A30
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CNavigationInput {
    C3DVector Position;
    long      Flags;
    long      Mode;
    long      Extra;
    CNavigationInput(const C3DVector& position) : Flags(0), Mode(3), Extra(0) { Position.X = position.X; Position.Y = position.Y; Position.Z = position.Z; }
};
struct CCreatureNavOutput {
    long      Result;                                 // 0 failed, 1 arrived, 2 step
    C3DVector Step;
};
struct CTCCreatureNavigation {                        // interface 7
    CCreatureNavOutput GetNextNavPosOnRoute(const CNavigationInput& input, bool a, bool b, bool c, bool d, bool e);   // 0x006CBAC0
};
struct CTCLook {                                      // interface 0x43
    void LookAtPos(const C3DVector& position, long priority);   // 0x00753E90
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CTCPhysicsBase;
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void GiveUp();                            // slot 4 (+0x10)
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    void SetMoveToPos(const C3DVector& position);     // 0x006929E0
    void FrameUpdate();                               // 0x00694020
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_NavToPos : CCreatureActionBase {
    CTCPhysicsBase* PreviousPhysicsComponent;         // +0xa8
    C3DVector       NavPos;                           // +0xac
    void FrameUpdate();
};

void CCreatureAction_NavToPos::FrameUpdate()
{
    CTCCreatureNavigation* navigation;
    GetCreature()->GetInterface(&navigation, 7);
    CNavigationInput input(NavPos);
    CCreatureNavOutput output = navigation->GetNextNavPosOnRoute(input, false, true, true, false, false);
    if (output.Result == 0) {
        SetMoveToPos(GetCreature()->GetPosition());
        GiveUp();
    } else if (output.Result == 2) {
        C3DVector position = GetCreature()->GetPosition() + output.Step;
        SetMoveToPos(position);
        CTCLook* look;
        if (GetCreature()->GetInterface(&look, 0x43) && !GetCreature()->GetPlayer()->m_autoFaceTarget)
            look->LookAtPos(position, 0x11);
    } else if (output.Result == 1) {
        SetMoveToPos(GetCreature()->GetPosition());
        GiveUp();
    }
    CCreatureActionBase::FrameUpdate();
}
