// CAIStateGroup_SellToBuyer::IsRunnable() -- runnable while the creature is in a two-party interaction of type 8 (trade) and a buy order message is waiting
// Leaf: ?IsRunnable@CAIStateGroup_SellToBuyer@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094C010, 63 bytes (+ int3 padding), thiscall virtual (slot 11), `ret`.
// Identity (proven): Ego_r.pdb donor order GetBuyersOrder (0x00970380) / IsRunnable (0x009703D0) = retail 0x0094BFC0 / this row
//   (vtable slot 11); the label agrees.
// Callees: CAIStateGroup_SellToBuyer::GetBuyersOrder() 0x0094BFC0 (this batch).
// Layout: Ego_r.pdb CThingCreatureBase PCreatureInteraction CCountedPointer<CCreatureInteraction> at retail +0x12C (Ego_r
//   +0x150); CCreatureInteraction Participants (STLport list<CThingCreatureBase*>: one node pointer) +0, Type +4.
// Shape: the counted pointer bound to a const reference (the `add eax,12Ch ; mov eax,[eax]` load), tested through its Data
//   and read through the inline operator->; STLport list::size() walks the nodes inline; `if (...) return true; return false;`.
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
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
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T* Data;                                            // +0x00
    CCPPointerInfo* Info;                               // +0x04
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
class CThingCreatureBase;

namespace std {
template <class T> class list {                         // STLport 4.6: one pointer to the sentinel node
public:
    struct _Node { _Node* _M_next; _Node* _M_prev; T _M_data; };
    _Node* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (const _Node* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
};
}
class CCreatureInteraction {                            // Ego_r.pdb: Participants list, Type (retail +0 / +4)
public:
    std::list<CThingCreatureBase*> Participants;        // +0x00
    long Type;                                          // +0x04 ECreatureInteractionType
};
class CThingCreatureBase : public CThingGameObject {
public:
    bool IsPerformingAnyAction() const;                 // 0x006622A0
    unsigned char _pad94[0x12C - 0x94];
    CCountedPointer<CCreatureInteraction> PCreatureInteraction;   // +0x12C (Ego_r.pdb +0x150)
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
public:
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0
public:
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    bool IsStopping() const;                            // 0x008FD010
};
class CMessageEvent;
class CAIStateGroup_SellToBuyer : public CAIStateGroupBase {
public:
    virtual bool IsRunnable();
private:
    const CMessageEvent* GetBuyersOrder();              // 0x0094BFC0
};

bool CAIStateGroup_SellToBuyer::IsRunnable()
{
    const CCountedPointer<CCreatureInteraction>& interaction = PBrain->Creature->PCreatureInteraction;
    if (interaction.Data && interaction->Type == 8 && interaction->Participants.size() == 2 && GetBuyersOrder())
        return true;
    return false;
}