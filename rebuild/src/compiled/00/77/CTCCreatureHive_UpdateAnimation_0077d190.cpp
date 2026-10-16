// CTCCreatureHive::UpdateAnimation() -- while the hive is animating, advance its animation by one frame (1 / fps seconds) and stop animating once the main anim set is empty or finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077D190, length 153 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x5A, the
// CTCAnimationComplex: word 2 bit 26, `test dword ptr [eax+28h],4000000h`), CTCAnimationComplex::GetAnimation @ 0x0070B460
// (`mov eax,[ecx+0Ch]`; the manifest's "CWorld::DrawGetEnvironment" is propagated), CAnimateThing::UpdateFrameLogic @ 0x0070E8B0.
// Globals/constants: frames per second @ 0x01375550, 1.0f @ 0x0122DED8.
// Layout: Ego_r.pdb CTCCreatureHive: Animating +0xC. CAnimateThing starts with CurrentAnim.MainAnimSet (retail CAnimSet:
// Anims vector at +4 {first,last,end}, the Looping/Finished bit byte at +0x51 = PDB +0x59 less the two 4-byte-shorter vectors).
// The UpdateFrameLogic argument is a 12-byte frame record {seconds, 0, 0} built on the stack (type not established).
// Shape: GetInterface result unchecked (the absent arm reads the unset out pointer from the key's frame word).
struct CTCBaseInterface;
struct CInterfaceMap {                         // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];            // +0x20 std::bitset<274> UsingComponents
    CInterfaceMap m_interfaces;                // +0x44 Components
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
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};
extern long g_framesPerSecond;                 // 0x01375550

struct CAnimFrameTime {                        // 12-byte UpdateFrameLogic argument
    float Seconds;
    long  Reserved1;
    long  Reserved2;
    CAnimFrameTime(float seconds) : Seconds(seconds), Reserved1(0), Reserved2(0) {}
};
struct CAnimateThing {                         // CurrentAnim.MainAnimSet first (retail CAnimSet layout)
    long   UpdateMode;                         // +0x00
    short* AnimsFirst;                         // +0x04 vector<short> Anims
    short* AnimsLast;                          // +0x08
    short* AnimsEnd;                           // +0x0c
    unsigned char _pad10[0x51 - 0x10];
    unsigned char Looping : 1;                 // +0x51 bit 0
    unsigned char Finished : 1;                // +0x51 bit 1
    void UpdateFrameLogic(const CAnimFrameTime& time);   // 0x0070E8B0
};
struct CTCAnimationComplex {
    CAnimateThing& GetAnimation();             // 0x0070B460
};
struct CTCCreatureHive : CTCBase {
    bool Animating;                            // +0x0c
    void UpdateAnimation();
};

void CTCCreatureHive::UpdateAnimation()
{
    if (Animating) {
        CTCAnimationComplex* animation;
        ParentThing->GetInterface(&animation, 0x5a);
        CAnimateThing& anim = animation->GetAnimation();
        anim.UpdateFrameLogic(CAnimFrameTime(1.0f / g_framesPerSecond));
        if (anim.AnimsFirst == anim.AnimsLast || anim.Finished)
            Animating = false;
    }
}