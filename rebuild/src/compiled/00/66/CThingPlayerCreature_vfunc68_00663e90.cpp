// CThingPlayerCreature::vfunc_68(long animIndex, bool flag) -- vtable slot 68: start animIndex as the main animation (type 1, mode 1, percent 100) unless it is already the unfinished main animation; the animator's guard bit is held around SetMainAnim
// Leaf: vfunc_68
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00663E90, length 175 bytes, `ret 8`; slot 68 of vtable 0x012457FC =
//   CThingPlayerCreature (rtti-vtables.tsv; the slot's PDB name is not recovered).
// Callees: CInterfaceMap::LowerBound @ 0x0040F020 (TC 0x5a = CTCAnimation), CTCAnimation::GetAnimation @ 0x0070B460,
//   CAnimInit::Set @ 0x0070C050, CAnimateThing::SetMainAnim(const CAnimInit&, long, long) @ 0x0070D580.
// Layout: retail CThing interface bitset +0x20 (TC 0x5a = word +0x28 bit 26), interface map +0x44; CAnimateThing's
//   CurrentAnim.MainAnimSet: Anims (vector<short>) +0x04/+0x08, Finished +0x51 bit 1 (PDB +0x59: two 12-byte vectors);
//   the animator's guard bit +0x94 bit 0 (name unproven). CAnimInit as the landed CTCAnimationComplex::OnCreate model.
// Shape: the component pointer is null-initialised and set when HasTC (`xor ecx,ecx` preloaded; the lookup's end/hit choice is one ternary so the
//   component lands straight in ecx); the main anim index
//   is read through `*begin()` before the empty test; the init record is the landed inline CAnimInit constructor (zero
//   blend pair stored after the Set arguments).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CBlendPair {
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
class CAnimInit { public:                              // 0x2c bytes
    long          Mode;                         // +0x00
    unsigned char AnimType;                     // +0x04
    bool          Flag05;                       // +0x05
    long          AnimIndex;                    // +0x08
    float         Scales[3];                    // +0x0c
    CBlendPair    Blend;                        // +0x18
    unsigned long Flags;                        // +0x20
    long          Arg24;                        // +0x24
    long          Percent;                      // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};

struct CShortVector {                           // retail std::vector<short> (12 bytes)
    short* _M_start;
    short* _M_finish;
    short* _M_end_of_storage;
    short* begin() const { return _M_start; }
    short* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
};
class CAnimSet { public:
    long UpdateMode;                            // +0x00
    CShortVector Anims;                         // +0x04
    unsigned char _pad10[0x51 - 0x10];
    unsigned char Looping : 1;                  // +0x51 bit 0
    unsigned char Finished : 1;                 // +0x51 bit 1
};
class CAnimateThing { public:
    CAnimSet MainAnimSet;                       // +0x00 CurrentAnim.MainAnimSet
    unsigned char _pad54[0x94 - 0x54];
    unsigned char SettingMainAnim : 1;          // +0x94 bit 0
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
class CTCAnimationComplex { public:                           // interface type 0x5a
    CAnimateThing& GetAnimation();              // 0x0070B460
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

class CThingPlayerCreature : public CThing {
public:
    void vfunc_68(long animIndex, bool flag);
};

void CThingPlayerCreature::vfunc_68(long animIndex, bool flag)
{
    CTCAnimationComplex* component = 0;
    if (HasTC(0x5a))
        component = GetTC<CTCAnimationComplex>(0x5a);
    CAnimateThing& animation = component->GetAnimation();
    CAnimSet& main = animation.MainAnimSet;
    if (*main.Anims.begin() == animIndex && !main.Anims.empty() && !main.Finished)
        return;
    animation.SettingMainAnim = 1;
    animation.SetMainAnim(CAnimInit(1, 0, animIndex, 1, flag, 0, 100), 1, 0);
    animation.SettingMainAnim = 0;
}