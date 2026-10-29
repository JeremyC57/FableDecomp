// CAIStateGroup_MinionSitAtSchool::AllocChairSittingMode() const -- picks the chair sitting mode at random: half the time a studious sit (CModeSitChairStudious on the creature), otherwise the CAIStateGroup_UsesSeat default
// Leaf: ?AllocChairSittingMode@CAIStateGroup_MinionSitAtSchool@@MBEPAVCModeSitBase@NCreatureMode@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F7D0, 96 bytes (+ int3 padding), thiscall virtual (slot 15), `ret`.
// Identity (proven): Ego_r.pdb ?AllocChairSittingMode@CAIStateGroup_MinionSitAtSchool@@MBEPAVCModeSitBase@NCreatureMode@@XZ
//   (0x009345D0); the label agrees. RTTI: CAIStateGroup_MinionSitAtSchool : CAIStateGroup_MinionSitOnSeatBase : CAIStateGroup_UsesSeat.
// Callees: CAIStateGroupBase::GetPWorldSeed() const 0x008FCFD0, GFROR13(unsigned long) 0x00497890 (Ego_r.pdb, __fastcall),
//   CAIStateGroup_UsesSeat::AllocChairSittingMode() const 0x00938280 (tail `jmp`), operator new(unsigned int) 0x00BFEA1A,
//   NCreatureMode::CModeSitChairStudious::CModeSitChairStudious(CThing&) 0x00910830 (Ego_r.pdb; 0x48 bytes per `push 48h`).
// Shape: GFRandom (Ego_r.pdb, inlined: LCG step 0x24A1/0x24DF, ROR13, modulo 2) into a named choice, then a zero `mode`
//   local declared after it (the `xor ecx,ecx ; sub eax,ecx` switch base and the default `mov eax,ecx`); each case
//   assigns `mode` and breaks.
// ---- shared AI state-group model (retail STLport layout; the generated engine headers are flat Ego_r layouts) ----
// CAIStateGroupBase (RTTI: : CBaseClass): vptr +0, PBrain +4, ScriptAIPriority +8, Name +0xC, Terminated +0x10, Priority
// +0x11, Enabled +0x12 (0x14 bytes). CAIBrain::Creature +0x18 (engine/CAIBrain.h agrees). CThing (retail): UsingComponents
// bitset<274> +0x20, Components CVectorMap +0x44, Dead bit 0 of +0x91 (Ego_r +0xA1 minus 0x10: bitset, CVectorMap, list).
// Getter identities (Ego_r.pdb donor order = retail order 0x008FCF40..0x008FD010): Terminate 0x008FCF40, PeekWorld
// 0x008FCFA0, GetWorld 0x008FCFC0, GetPWorldSeed 0x008FCFD0, PeekThingSearchTools 0x008FD000, IsStopping 0x008FD010.
#include "engine/CAIBrain.h"                           // Creature +0x18
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};
class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseObject : public CBaseClass { };
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    void* PData;                                        // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
};
template <class T> class CCountedPointer {
public:
    T* Data;                                            // +0x00
    T* operator->() const { return Data; }
};
class CClass : public CBaseObject { };
class CThing : public CClass {
public:
    unsigned char _pad04[0x1C];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    unsigned char Components[0x10];                     // +0x44 CVectorMap<ETCInterfaceType, CTCBase*, ...>
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Dead : 1;                             // +0x91 bit 0
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject {
public:
    bool IsPerformingAnyAction() const;                 // 0x006622A0
};
class CThingAICreature : public CThingCreatureBase { };
class CThingObject : public CThingGameObject { };
class CThingBuilding;

#include "engine/CWorld.h"                            // PThingCombatManager_ptr +0x4C, PMessageEventManager_Data +0x60
class CThingSearchTools;
class CWorld;
class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*     PBrain;                               // +0x04
    long          ScriptAIPriority;                     // +0x08
    CCharString   Name;                                 // +0x0C
    bool          Terminated;                           // +0x10
    unsigned char Priority;                             // +0x11
    bool          Enabled;                              // +0x12
    void Terminate();                                   // 0x008FCF40
protected:
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
    CWorld& GetWorld() const;    // 0x008FCFC0
public:
    protected:
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    public:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    bool IsStopping() const;                            // 0x008FD010
};
long __fastcall GFROR13(unsigned long value);           // 0x00497890 (Ego_r.pdb ?GFROR13@@YIJK@Z)
// GFRandom (Ego_r.pdb ?GFRandom@@YIKKPAK@Z), inlined here: one LCG step of the seed, rotated, modulo the range.
inline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return *seed % range;
}
namespace NCreatureMode {
class CModeSitBase {
public:
    virtual ~CModeSitBase();
};
class CModeSitChairStudious : public CModeSitBase {     // 0x48 bytes
public:
    CModeSitChairStudious(CThing& thing);               // 0x00910830
    unsigned char _pad04[0x48 - 0x04];
};
}
class CAIStateGroup_UsesSeat : public CAIStateGroupBase {
protected:
    virtual NCreatureMode::CModeSitBase* AllocChairSittingMode() const;   // 0x00938280
};
class CAIStateGroup_MinionSitOnSeatBase : public CAIStateGroup_UsesSeat { };
class CAIStateGroup_MinionSitAtSchool : public CAIStateGroup_MinionSitOnSeatBase {
protected:
    virtual NCreatureMode::CModeSitBase* AllocChairSittingMode() const;
};

NCreatureMode::CModeSitBase* CAIStateGroup_MinionSitAtSchool::AllocChairSittingMode() const
{
    unsigned long choice = GFRandom(2, GetPWorldSeed());
    NCreatureMode::CModeSitBase* mode = 0;
    switch (choice) {
    case 0:
        mode = new NCreatureMode::CModeSitChairStudious(*PBrain->Creature);
        break;
    case 1:
        mode = CAIStateGroup_UsesSeat::AllocChairSittingMode();
        break;
    }
    return mode;
}