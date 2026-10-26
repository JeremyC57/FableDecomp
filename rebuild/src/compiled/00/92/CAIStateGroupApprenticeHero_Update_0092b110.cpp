// CAIStateGroup_ApprenticeHero::Update() -- while the apprentice's village says it is cook time, keep doing random apprentice activities (practise skills, spar, library, leaderboard, map); stop when one fails or the group is stopping, else schedule the next practice a minute ahead; then terminate the current action
// Leaf: ?Update@CAIStateGroup_ApprenticeHero
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092B110, 276 bytes incl. the 5-entry jump table, `ret`. Vtable 0x0128AEC4 slot 12.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, interface 0x23 =
//   CTCVillageMember: bitset word +0x24 bit 3), CTCVillageMember::GetPVillage 0x007EE540 (label "IS_THING_ALIVE<...>"
//   propagated), CTCVillage::IsCookTime 0x00658B20, CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890
//   (__fastcall), the virtual PracticeSkills (slot 14, +0x38, 0x0092A8A0), PracticeAgainstApprentice 0x0092A3F0,
//   GoToLibrary 0x0092ACE0, LookAtLeaderBoard 0x0092AE70, LookAtMap 0x0092AFE0, CAIStateGroupBase::IsStopping 0x008FD010,
//   GFGetCurrentFrame 0x0049D870 (__fastcall), CAIStateGroupBase::Terminate 0x008FCF40 (label "CXMVPlaybackInfo" propagated).
// Global: the frames-per-second long 0x01375550, read before the frame call (`imul esi,esi,3Ch` = one minute).
// Shape: retail tests the interface bit twice on one CSE'd byte (`mov dl,[eax+24h] ; mov cl,8 ; test cl,dl ; je` then
//   `test cl,dl ; je` again, the second arm reading the never-written component slot): the guard is the word-form
//   HasTC, the lookup an inline accessor returning an unset local (`CTC* tc; if (TestTC(k)) tc = GetTC(k); return tc;`,
//   GOTCHAS wave 25 chunk 141) whose test is written in a DIFFERENT (byte) form -- with the same expression in both
//   places (word or byte, or the out-parameter GetInterface) VC7.1 threads the second test away. The random pick is the
//   landed GetPWorldSeed / GFROR13 step (unsigned `div` by 5); `bool ok = false` before the switch sends the table's
//   out-of-range edge straight to Terminate like retail; every exit is written out (Terminate + return).
// Layout: engine/CAIStateGroup_ApprenticeHero.h flattens the CAIStateGroupBase vptr to a void* member, which cannot carry
//   the virtual PracticeSkills call, so the class is modelled locally (PBrain +0x04, NextFramePractice +0x14 as in the
//   header); CThing as in the landed 0x008337B0 (bitset +0x20, interface map +0x44).
class CTCBase;
struct CInterfaceMap {                               // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

class CThing { public:
    void*               __vftable;                   // +0x00
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];          // +0x20 bitset<274>
    CInterfaceMap       Components;                  // +0x44
    unsigned long HasTC(long interfaceType) const
    {
        return UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    bool TestTC(long interfaceType) const
    {
        return (((const unsigned char*)UsingComponents)[interfaceType >> 3] & (1 << (interfaceType & 7))) != 0;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

class CTCVillage { public:
    bool IsCookTime();                               // 0x00658B20
};
class CTCVillageMember { public:
    CTCVillage* GetPVillage();                       // 0x007EE540
};

unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
long __fastcall GFGetCurrentFrame();                     // 0x0049D870
extern long g_framesPerSecond;                           // 0x01375550

class CAIBrain { public:
    unsigned char _pad00[0x18];
    CThing*       PCreature;                         // +0x18
};

class CAIStateGroupBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11();
    virtual void Update();                           // slot 12
    virtual void v13();
    virtual bool PracticeSkills();                   // slot 14 (+0x38)
    CAIBrain*     PBrain;                            // +0x04
    unsigned char _pad08[0x14 - 0x08];
    CThing* GetPCreature() const { return PBrain->PCreature; }
    CTCVillageMember* GetVillageMember()
    {
        CTCVillageMember* tc;
        if (GetPCreature()->TestTC(0x23))
            tc = GetPCreature()->GetTC<CTCVillageMember>(0x23);
        return tc;
    }
    unsigned long* GetPWorldSeed();                  // 0x008FCFD0
    bool IsStopping();                               // 0x008FD010
    void Terminate();                                // 0x008FCF40
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

struct CAIStateGroup_ApprenticeHero : CAIStateGroupBase {
    long NextFramePractice;                          // +0x14
    bool PracticeAgainstApprentice();                // 0x0092A3F0
    bool GoToLibrary();                              // 0x0092ACE0
    bool LookAtLeaderBoard();                        // 0x0092AE70
    bool LookAtMap();                                // 0x0092AFE0
    virtual void Update();
};

void CAIStateGroup_ApprenticeHero::Update()
{
    CThing* creature = GetPCreature();
    if (!creature->HasTC(0x23))
        return;
    CTCVillage* village = GetVillageMember()->GetPVillage();
    if (village == 0)
        return;
    while (village->IsCookTime()) {
        bool ok = false;
        switch (Random(5)) {
        case 0: ok = PracticeSkills(); break;
        case 1: ok = PracticeAgainstApprentice(); break;
        case 2: ok = GoToLibrary(); break;
        case 3: ok = LookAtLeaderBoard(); break;
        case 4: ok = LookAtMap(); break;
        }
        if (!ok || IsStopping()) {
            Terminate();
            return;
        }
    }
    long framesPerSecond = g_framesPerSecond;
    NextFramePractice = GFGetCurrentFrame() + framesPerSecond * 60;
    Terminate();
}