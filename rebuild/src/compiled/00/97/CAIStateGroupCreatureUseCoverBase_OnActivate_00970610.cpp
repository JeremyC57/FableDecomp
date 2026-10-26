// CAIStateGroup_CreatureUseCoverBase::OnActivate() -- cache whether the creature has the dive-for-cover and both jump-out anims, say the combat dialogue's take-cover speech and reset the found-position count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970610, length 336 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThingCreatureBase::HasAnim @ 0x00662C70,
//          CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CombatDialogueManager::SaySpeech @ 0x008AFC40.
// Vtables: CAnimFilterAnyGroup 0x0127261C, CAnimFilterBase 0x0122FFCC. Strings DIVE_FOR_COVER / JUMP_OUT_LEFT / JUMP_OUT_RIGHT.
// Shape: each HasAnim goes through an inline (name, filter) wrapper so the name temporary is built before the filter's vtable
//          store and the filter restores CAnimFilterBase's vtable before the name dies; the second flag is an && of two
//          wrapper calls with conditional temporaries (ebx flag mask 3 / 0xF). The dialogue def is add-ref'd from its raw
//          pointer (release has no null test); creature and world are named locals so the pushes follow GetPWorld.
// Layout: engine/CAIStateGroup_CreatureUseCoverBase.h; retail CWorld+0x4C PThingCombatManager, CCombatManager+0x38
//          DialogueManager (PDB +0x40), CombatDialogueManager+0x0C PDialogueDef (PDB +0x10); +0x48 of the group is the
//          retail found-position count (PDB places ValidPositionsFound at +0x4C).
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CAnimFilterBase { public:                                // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual void v01();
    virtual void v02();
};
struct CAnimFilterAnyGroup : CAnimFilterBase {          // vtable 0x0127261C
};
class CThingCreatureBase { public:
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x00662C70
};
class CCombatDialogueDef { public:
    virtual void Destroy();
    virtual void Delete();                              // slot 1
    long m_refCount;                                    // +0x04
    unsigned char _08[0x4c - 0x08];
    unsigned long TakeCoverSpeechGroup;                 // +0x4c (descriptive)
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (--Object->m_refCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
};
class CombatDialogueManager { public:
    unsigned char _00[0x0c];
    CDefPointer<CCombatDialogueDef> PDialogueDef;       // +0x0c
    bool SaySpeech(CThingCreatureBase& talker, unsigned long speechGroup);   // 0x008AFC40
};
class CCombatManager { public:
    unsigned char _00[0x38];
    CombatDialogueManager DialogueManager;              // +0x38
};
class CWorld { public:
    unsigned char _00[0x4c];
    CCombatManager* PThingCombatManager;                // +0x4c
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                      // +0x18
};

struct CAIStateGroupCreatureUseCoverBaseMethods : CAIStateGroup_CreatureUseCoverBase {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThingCreatureBase* GetCreature() const { return Brain()->PCreature; }
    bool CreatureHasAnim(const CCharString& name, const CAnimFilterBase& filter) const { return GetCreature()->HasAnim(name, filter); }
    CWorld* GetPWorld() const;                          // 0x008FCFC0
    void OnActivate();
};

void CAIStateGroupCreatureUseCoverBaseMethods::OnActivate()
{
    HasDiveAnim = CreatureHasAnim("DIVE_FOR_COVER", CAnimFilterAnyGroup());
    HasJumpOutAnims = CreatureHasAnim("JUMP_OUT_LEFT", CAnimFilterAnyGroup()) && CreatureHasAnim("JUMP_OUT_RIGHT", CAnimFilterAnyGroup());
    unsigned long speechGroup;
    {
        CDefPointer<CCombatDialogueDef> def(GetPWorld()->PThingCombatManager->DialogueManager.PDialogueDef.Object);
        speechGroup = def->TakeCoverSpeechGroup;
    }
    CThingCreatureBase* creature = GetCreature();
    CWorld* world = GetPWorld();
    world->PThingCombatManager->DialogueManager.SaySpeech(*creature, speechGroup);
    ValidPositionsFound = 0;                            // +0x48 (retail STLport 4.6 header)
}