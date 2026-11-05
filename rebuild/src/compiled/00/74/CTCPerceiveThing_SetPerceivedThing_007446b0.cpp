// CTCPerceiveThing::SetPerceivedThing(const CThing* thing) -- point PerceivedThing at the thing (through a temporary intelligent pointer) and add or remove the parent thing from the player GUI's perceiver drawing
// Leaf: SetPerceivedThing
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007446B0, 124 bytes. Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10.
// Shape: `PerceivedThing = CIntelligentPointer(thing);` -- the temporary is built in the frame (vtable 0x01237F1C stored after the argument push), its inline operator= is SetPItem(other.GetPItem()), then its destructor.
// Model (local; engine/CTCPerceiveThing.h flattens the CIntelligentPointer member to a byte array): PerceivedThing +0x0c
//   (CIntelligentPointer<CThing const>), FramesSinceDirectlyPerceivedThing +0x1c, the flag bytes +0x2c..+0x33.
//   (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) @ 0x013B8790: CPlayerGui::AddPerceivingThing @ 0x004378F0 / RemovePerceivingThing @ 0x00437900 are
//   `mov ecx,[ecx+1B8h] ; jmp` forwarders to CDrawPerceivers (0x0064A2DB AddPerceivingThing / 0x0064639C); the catalog's
//   "_Cons_val<...>" labels on them are propagated.
// The add-or-remove tail is the inline UpdateDrawPerceivers(): the perceiving thing is drawn while its target exists.
// Shape: the CIntelligentPointer operator= names the GetPItem() CALL result (`item = other.GetPItem(); SetPItem(item);`), which
//   keeps the member's address (this of SetPItem) after the temporary's GetPItem call as retail; the inline form hoists it.
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing const>, vtable 0x01237F1C
    CIntelligentPointer(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); return *this; }
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:                             // the global (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) @ 0x013B8790
    void AddPerceivingThing(CThing* thing);     // 0x004378F0 (forwards to CDrawPerceivers @ +0x1b8; catalog "_Cons_val<...>")
    void RemovePerceivingThing(CThing* thing);  // 0x00437900 (forwards to CDrawPerceivers @ +0x1b8; catalog "_Cons_val<...>")
};
class CDefPointeeBase;
#include "engine/C3DVector.h"
class CTCPerceiveThing { public:
    void*     __vftable;                        // +0x00 (CTCBase)
    CThing*   ParentThing;                      // +0x04
    bool      ToBeRemoved;                      // +0x08
    bool      IsInGlobalUpdateTCs;              // +0x09
    CIntelligentPointer PerceivedThing;         // +0x0c
    float     DistanceFromPerceivedThingSquared;// +0x14
    float     AwarenessTimer;                   // +0x18
    long      FramesSinceDirectlyPerceivedThing;// +0x1c
    C3DVector LastPerceivedPos;                 // +0x20
    bool      Perceiving;                       // +0x2c
    bool      LineOfSight;                      // +0x2d
    bool      ExtendedLineOfSight;              // +0x2e
    bool      IgnoreFOVLineOfSight;             // +0x2f
    bool      DirectAwareness;                  // +0x30
    bool      Smelt;                            // +0x31
    bool      Heard;                            // +0x32
    bool      SearchingForThingToPerceive;      // +0x33
    CDefPointeeBase* PDef;                      // +0x34 CDefPointer<CPerceivedThingDef const>::Object
    long      UpdateOffset;                     // +0x38
    float     SoundRadius;                      // +0x3c
    float     SightRadius;                      // +0x40
    float     ExtendedSightRadius;              // +0x44
    float     SmellRadius;                      // +0x48
    float     GiveUpChaseRadius;                // +0x4c
    bool      PerceptionCombat;                 // +0x50
    long      FrameHitByPerceivedThing;         // +0x54
    void UpdateDrawPerceivers()
    {
        if (PerceivedThing.GetPItem())
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().AddPerceivingThing(ParentThing);
        else
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(ParentThing);
    }
    void SetPerceivedThing(const CThing* thing);
    void SetThingLineOfSight(bool lineOfSight);
    void SetThingExtendedLineOfSight(bool lineOfSight);
};

void CTCPerceiveThing::SetPerceivedThing(const CThing* thing)
{
    {
        CIntelligentPointer newThing(thing);
        PerceivedThing = newThing;
    }
    UpdateDrawPerceivers();
}