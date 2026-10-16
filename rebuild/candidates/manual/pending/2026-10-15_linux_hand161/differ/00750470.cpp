// CTCChest::UpdateAnimation(float time) -- advance the chest's animation (its CTCAnimationComplex's animate-thing) by the given time step
// Grade: DIFFER(110v110) -- retail opens with `mov eax,ecx ; mov eax,[eax+4]` and keeps the thing in eax (bitset word in edx, the preloaded null component in ecx); ours loads the thing into edx and the null component into eax, and stores the time before the two zero words (retail zero words first); init-list vs body-assigned time, a GetTC(this,k) helper and an animation local were tried
//
// Retail 0x00750470, 110 bytes (`ret 4`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020, CTCAnimationComplex::GetAnimation @ 0x0070B460 (`mov eax,[ecx+0Ch]`; manifest
// "CWorld::DrawGetEnvironment" is propagated), CAnimateThing::UpdateFrameLogic(const update&) @ 0x0070E8B0.
// Shape: `CTCAnimationComplex* p = 0; if (HasTC(0x5a)) p = GetTC(0x5a);` gives the preloaded
// `xor ecx,ecx` and the whole-word bit test (`test edx,4000000h`); the 12-byte update record
// {time, 0, 0} is a frame local built by an inline constructor (the zero words are stored before
// the time).
struct CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

struct CAnimFrameUpdate {                                   // 12-byte update record
    float Time;                                             // +0x00
    long  Reserved0;                                        // +0x04
    long  Reserved1;                                        // +0x08
    CAnimFrameUpdate(float time) : Time(time), Reserved0(0), Reserved1(0) {}
};

struct CAnimateThing {
    void UpdateFrameLogic(const CAnimFrameUpdate& update);  // 0x0070E8B0
};

struct CTCAnimationComplex {
    CAnimateThing* GetAnimation();                          // 0x0070B460
};

struct CTCChest {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void UpdateAnimation(float time);
};

void CTCChest::UpdateAnimation(float time)
{
    CTCAnimationComplex* complex = 0;
    if (ParentThing->HasInterface(0x5a))
        complex = (CTCAnimationComplex*)ParentThing->GetInterface(0x5a);
    CAnimFrameUpdate update(time);
    CAnimateThing* animation = complex->GetAnimation();
    animation->UpdateFrameLogic(update);
}
