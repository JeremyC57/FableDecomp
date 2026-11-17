// CAIStateGroup_MinionReactToUnconsciousPerson::Update() -- walk up to the (living) unconscious person, mark it discovered by this creature, then either (creature in life state 3) look at it and play the alert-to-player action, or post the STANDARD_MESSAGE_EVENT_FOUND_UNCONSCIOUS_PERSON message to a guard at the person's position; always Terminate()
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009378F0, length 535 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 (the by-value
// CIntelligentPointer<const CThing> target, vtable 0x01237F1C, inline at the first site), its out-of-line converting
// ctor @ 0x0091F990 (second site), AIAction_MoveToThingProximity @ 0x0097E380, CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface 0x40 = CTCDiscovery, bit 0 of +0x28), CTCDiscovery::SetDiscovered @ 0x007509D0, the
// creature's life-state getter @ 0x00662010, AIAction_LookAtThing @ 0x0097D6F0, CCreatureAction_AlertToPlayer ctor
// @ 0x009110F0 / ~CCreatureActionBase @ 0x00693EF0, AIAction_PerformActionAndWait @ 0x0097DB70, CCharString(const
// char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, GetPDefinitionManager @ 0x008FCFB0, CDefinitionManager::
// GetDef<CMessageEventDef> @ 0x007923C0, CThing::GetPos @ 0x004C73D0, AIAction_FindAGuardAndGiveMessage
// @ 0x009810B0 (def and position by value), Terminate @ 0x008FCF40.
// Data: "STANDARD_MESSAGE_EVENT_FOUND_UNCONSCIOUS_PERSON" @ 0x01292318; 0.0027777f look speed (immediate).
// Layout: engine/CAIStateGroup_MinionReactToUnconsciousPerson.h flattens the base, so the model is local (PBrain +4,
// PPerson +0x14); the alert action is 0xa8 bytes (retail CCreatureActionBase size).
// Shape: the discovery lookup's HasTC reads the bitset word into a named local and shifts it (`mov ecx,[eax+28h] ; and
// cl,bl`; the shifted-byte form narrows to a byte load and swaps this/&PPerson between ebp/edi), bound to a `const bool&`
// (the byte spill at +0x0b); the message def is released by hand through a pointer local (landed 0x00849140 shape) and
// passed by value through an add-ref copy; a named person local puts the flags push after GetPItem.
#include "fable_compiler.h"
// NF49 (round 7): the string, the def handle, the message-event def, the manager and the AI state-group base come from the shared
// headers (were local CCharString / CMessageEventDef / CDefPointer / CDefinitionManager / CAIStateGroupBase / CIntelligentPointerConstThing models): the manager accessor 0x008FCFB0 is the protected const
// ?PeekDefinitionManager@CAIStateGroupBase@@IBEABVCGameDefinitionManager@@XZ (Ego_r.pdb / FableWin.pdb; engine/CAIStateGroupBase.h
// _MEMBERS + _PEEKDEFINITIONMANAGER + _PROTECTED), was a made-up GetPDefinitionManager returning CDefinitionManager*; the lookup at
// 0x007923C0 is the by-name member template
// ??$GetDef@VCMessageEventDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCMessageEventDef@@@@@Z (FableWin.pdb),
// with its same-TU body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY, as landed 0x00849140); the hand-written release on a
// pointer local is the shared handle's scope-end destructor and the add-ref copy is its copy constructor. The AI actions are the
// header's protected members (_AIACTIONS 0x0097E380 / 0x0097D6F0, _PERFORMACTION 0x0097DB70, _FINDAGUARD 0x009810B0 taking
// CDefPointer<const CMessageEventDef> by value) with engine/fable_ai_action_enums.h; the CIntelligentPointer<const CThing>
// arguments are the shared template: the inline converting constructor (_CONVERT_INLINE, from the const base view) at the first
// site and the out-of-line non-const one 0x0091F990
// ??$?0VCThingCreatureBase@@@?$CIntelligentPointer@$$CBVCThing@@@@QAE@AAV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z
// (_CONVERT_MUTABLE_OUTOFLINE, FableWin.pdb) at the second. The overlay opt-in _CAIFILTERBASE_RESTORE_PROTECTED keeps the AI actions
// protected (IAE, both PDBs) after the nested filter's `public:`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE_OUTOFLINE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_FINDAGUARD
#define FABLE_RETAIL_CAISTATEGROUPBASE_CAIFILTERBASE_RESTORE_PROTECTED
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER_PROTECTED
#include "engine/fable_ai_action_enums.h"
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMessageEventDef.h"
class CTCBase;
#include "engine/C3DVector.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
        }
        return present != 0;
    }
};
class CThingCreatureBase : public CThing { public:
    long GetLifeState();                        // 0x00662010
};
class CTCDiscovery { public:                           // interface 0x40
    unsigned char _pad[0x0c];
    bool Discovered;                            // +0x0c
    void SetDiscovered(bool discovered, CThing* by);    // 0x007509D0
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
FABLE_FORCEINLINE bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                 // 0x00693EF0
};
class CCreatureAction_AlertToPlayer : public CCreatureActionBase { public:
    CCreatureAction_AlertToPlayer(CThingCreatureBase* creature);  // 0x009110F0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;          // +0x18
};
#include "engine/CAIStateGroupBase.h"
struct CAIStateGroup_MinionReactToUnconsciousPerson : CAIStateGroupBase {
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }   // (the row's helper, moved here from its local base model)
    CIntelligentPointer<CThingCreatureBase> PPerson;    // +0x14
    virtual void Update();
};

void CAIStateGroup_MinionReactToUnconsciousPerson::Update()
{
    if (IS_THING_ALIVE(PPerson)) {
        bool reached = false;
        if (AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED,
                CIntelligentPointer<const CThing>(static_cast<const CIntelligentPointer<CThingCreatureBase>&>(PPerson)),
                1.0f, reached, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f) == AI_ACTION_FINISHED && IS_THING_ALIVE(PPerson)) {
            CTCDiscovery* discovery;
            if (IS_THING_ALIVE(PPerson) && ((CThing*)PPerson.GetPItem())->GetInterface(&discovery, 0x40) && !discovery->Discovered)
                discovery->SetDiscovered(true, GetCreature());
            if (GetCreature()->GetLifeState() == 3) {
                if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, CIntelligentPointer<const CThing>(PPerson), 11, 0.0027777f) == AI_ACTION_FINISHED) {
                    if (AIAction_PerformActionAndWait(CCreatureAction_AlertToPlayer(GetCreature()), false) != 2) {
                        Terminate();
                        return;
                    }
                    Terminate();
                    return;
                }
            } else {
                CDefPointer<const CMessageEventDef> def;
                bool found;
                {
                    CCharString name("STANDARD_MESSAGE_EVENT_FOUND_UNCONSCIOUS_PERSON", -1);
                    found = PeekDefinitionManager().GetDef(name, &def);
                }
                if (found) {
                    CThing* person = ((CThing*)PPerson.GetPItem());
                    AIAction_FindAGuardAndGiveMessage(def, person->GetPos(), 0);
                }
            }
        }
    }
    Terminate();
}