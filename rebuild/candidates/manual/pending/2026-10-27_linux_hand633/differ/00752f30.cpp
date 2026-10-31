// CTCLook::UpdateHeadCreatureAnim(const CCharString& anim_name, ECreatureAnim type, CAnimateThing* animation) -- keeps the head-turn combination animation of `type` (11 horizontal, 12 vertical) on the creature: adds it from the cached head-turn anim entry when missing (a 50/50 frame pair), then refreshes its frame, remembering the previous one; without an entry nothing is touched, and when the animation cannot be found the head angles and velocities are zeroed
// Grade: DIFFER(226v226) -- callee-saved swap: retail type in ebx (loaded before `push ebp ; push edi`), this in edi; lane type in edi, this in ebx, ebp pushed after the entry test; the previous-frame spill sits in the dead anim-name argument slot (lane) instead of the 0.5f temporary's frame word (retail)
// Leaf: ?UpdateHeadCreatureAnim@CTCLook@@AAEXABVCCharString@@W4ECreatureAnim@@PAVCAnimateThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00752F30, length 226 bytes (`ret 0Ch`); speed TU (no frame pointer).
// Name: Ego_r PDB ?UpdateHeadCreatureAnim@CTCLook@@AAEXABVCCharString@@W4ECreatureAnim@@PAVCAnimateThing@@@Z (donor 0x007651B0,
// right after GetHeadCreatureAnimFrame 0x00764130 = retail 0x00752510, the callee here; Ego_r order lines up with retail).
// Callees: CAnimateThing::GetFirstCombinationAnimFromType(long) @ 0x0070C4E0 (twice; Ego_r 0x00576120, landed binding),
// CAnimInit::Init(uchar, ulong, long, EAnimUpdateMode, bool, ulong, long) @ 0x0070C050 (PDB signature; called by the inline PDB
// ctor CAnimInit(uchar, ulong, long, EAnimUpdateMode, bool, ulong, long)), CAnimateThing::AddCombinationAnim(const CAnimInit&, long,
// long, bool, bool) @ 0x0070D780, CTCLook::GetHeadCreatureAnimFrame(ECreatureAnim) const @ 0x00752510 (float in st0).
// Layout: Ego_r PDB CTCLook with the STLport TurningAnims vector (0xC, PDB Dinkumware 0x10): HeadAngleRelToBodyXY +0x2C,
// HeadAngleRelToBodyYZ +0x30, HeadVelocityXY +0x5C, HeadVelocityYZ +0x60, PHeadTurnAnimH +0xBC, PHeadTurnAnimV +0xC4 (PDB +0xC0/+0xC8;
// CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>, the entry starting with the anim index, -1 when none); CAnimInit 0x2C bytes
// (PDB) with CAnimFrameInfo Frame at +0x18; CAnimSet CurrentPercentageComplete +0x1C / LastPercentageComplete +0x20 (PDB +0x24/+0x28
// less the two STLport vectors). No engine/CTCLook.h exists. The anim-name argument is unused.
class CCharString;
class CAnimateThing;
enum ECreatureAnim { };
enum EAnimUpdateMode { };
class CAnimFrameInfo {
public:
    float Current;                                   // +0x00
    float Last;                                      // +0x04
    CAnimFrameInfo() : Current(0.0f), Last(0.0f) { }
    CAnimFrameInfo(const float& current, const float& last) : Current(current), Last(last) { }
};
class CAnimInit {                                    // 0x2c bytes
public:
    EAnimUpdateMode UpdateMode;                      // +0x00
    unsigned char   Type;                            // +0x04
    bool            Looping;                         // +0x05
    long            Anim;                            // +0x08
    float           Speed;                           // +0x0c
    float           Alpha;                           // +0x10
    float           LastAlpha;                       // +0x14
    CAnimFrameInfo  Frame;                           // +0x18
    unsigned long   Flags;                           // +0x20
    unsigned long   TypeFlags;                       // +0x24
    long            Priority;                        // +0x28
    void Init(unsigned char type, unsigned long type_flags, long anim, EAnimUpdateMode mode, bool looping, unsigned long flags, long priority);   // 0x0070C050
    CAnimInit(unsigned char type, unsigned long type_flags, long anim, EAnimUpdateMode mode, bool looping, unsigned long flags, long priority)
    {
        Init(type, type_flags, anim, mode, looping, flags, priority);
    }
};
class CAnimSet {
public:
    unsigned char _pad00[0x1c];
    float CurrentPercentageComplete;                 // +0x1c
    float LastPercentageComplete;                    // +0x20
};
class CAnimateThing {
public:
    CAnimSet* GetFirstCombinationAnimFromType(long type);   // 0x0070C4E0
    void AddCombinationAnim(const CAnimInit& init, long a, long b, bool c, bool d);   // 0x0070D780
};
namespace NAnimationEntryBodge {
class CAnimEntryLite {
public:
    long Anim;                                       // +0x00 (-1: none)
};
}
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
};

class CTCLook {
public:
    unsigned char     _pad00[0x2c];
    float             HeadAngleRelToBodyXY;          // +0x2c
    float             HeadAngleRelToBodyYZ;          // +0x30
    unsigned char     _pad34[0x5c - 0x34];
    float             HeadVelocityXY;                // +0x5c
    float             HeadVelocityYZ;                // +0x60
    unsigned char     _pad64[0xbc - 0x64];
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PHeadTurnAnimH;   // +0xbc
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PHeadTurnAnimV;   // +0xc4
private:
    float GetHeadCreatureAnimFrame(ECreatureAnim type) const;   // 0x00752510
    void UpdateHeadCreatureAnim(const CCharString& anim_name, ECreatureAnim type, CAnimateThing* animation);
};
void CTCLook::UpdateHeadCreatureAnim(const CCharString& anim_name, ECreatureAnim type, CAnimateThing* animation)
{
    NAnimationEntryBodge::CAnimEntryLite* entry;
    if (type == 11)
        entry = PHeadTurnAnimH.Data;
    else if (type == 12)
        entry = PHeadTurnAnimV.Data;
    else
        return;
    if (!entry)
        return;
    long anim = entry->Anim;
    if (anim == -1)
        return;
    if (!animation->GetFirstCombinationAnimFromType(type)) {
        CAnimInit init((unsigned char)type, 0, anim, (EAnimUpdateMode)0, false, 0, 100);
        init.Frame = CAnimFrameInfo(0.5f, 0.5f);
        animation->AddCombinationAnim(init, 0, 0, false, false);
    }
    CAnimSet* set = animation->GetFirstCombinationAnimFromType(type);
    if (set) {
        float last = set->CurrentPercentageComplete;
        set->CurrentPercentageComplete = GetHeadCreatureAnimFrame(type);
        set->LastPercentageComplete = last;
    } else {
        HeadAngleRelToBodyXY = 0.0f;
        HeadAngleRelToBodyYZ = 0.0f;
        HeadVelocityXY = 0.0f;
        HeadVelocityYZ = 0.0f;
    }
}
