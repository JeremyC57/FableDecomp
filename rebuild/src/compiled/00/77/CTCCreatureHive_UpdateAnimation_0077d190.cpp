// CTCCreatureHive::UpdateAnimation() -- while the hive is animating, advance its animation by one frame (1 / fps seconds) and stop animating once the main anim set is empty or finished
// Leaf: ?UpdateAnimation@CTCCreatureHive@@QAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
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
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

struct CAnimFrameTime {                        // 12-byte UpdateFrameLogic argument
    float Seconds;
    long  Reserved1;
    long  Reserved2;
    CAnimFrameTime(float seconds) : Seconds(seconds), Reserved1(0), Reserved2(0) {}
};
class CAnimateThing { public:                         // CurrentAnim.MainAnimSet first (retail CAnimSet layout)
    long   UpdateMode;                         // +0x00
    short* AnimsFirst;                         // +0x04 vector<short> Anims
    short* AnimsLast;                          // +0x08
    short* AnimsEnd;                           // +0x0c
    unsigned char _pad10[0x51 - 0x10];
    unsigned char Looping : 1;                 // +0x51 bit 0
    unsigned char Finished : 1;                // +0x51 bit 1
    void UpdateFrameLogic(const CAnimFrameTime& time);   // 0x0070E8B0
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();             // 0x0070B460
};
class CTCCreatureHive : public CTCBase { public:
    bool Animating;                            // +0x0c
    void UpdateAnimation();
};

void CTCCreatureHive::UpdateAnimation()
{
    if (Animating) {
        CTCAnimationComplex* animation;
        ParentThing->GetTC(TCI_ANIMATION, &animation);
        CAnimateThing& anim = animation->GetAnimation();
        anim.UpdateFrameLogic(CAnimFrameTime(1.0f / CMainGameComponent::GetConstantFPS()));
        if (anim.AnimsFirst == anim.AnimsLast || anim.Finished)
            Animating = false;
    }
}