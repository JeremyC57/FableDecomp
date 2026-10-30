// CScytheMarker::Main() -- the scythe marker entity script: spawn the rival hero Scythe (CREATURE_RIVAL_HERO_SCYTHE) at the marker with the ScytheNearOracle script, then remove the marker
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E29F50, length 195 bytes (`ret`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 and ~CCharString @ 0x0099EAE0 (the two names),
// CScriptThing slot 6 GetPos (+0x18), CGameScriptInterface slots 91 CreateCreature(const CCharString&, const C3DVector&,
// const CCharString&, bool) (CScriptThing by value) and 108 RemoveThing(const CScriptThing&, bool, bool), operator delete
// @ 0x00BFE9BC and CBaseClass::~CBaseClass @ 0x0099A2E0 (the inlined ~CScriptThing of the created creature).
// Layout = CEntityScriptBase {vptr, Game +4, CScriptThing Me +8} (+ ParentClass +0x14, PMasterData +0x18; PDB 28 bytes).
// Shape: the landed CSP_NymphTrigger::Main 0x00DED380 spawn (implicit CCharString conversions built before every other
// argument, the result a named local) without the wait loop; in this module ~CScriptThing is inline -- the CScriptThing
// vtable store (0x01238C8C), the PImp counted-pointer release (`--count; if (count == 0)` re-reading the block, the
// destroy member pointer on the item, delete, both words cleared) and the out-of-line CBaseClass destructor -- as in
// the landed CScriptThingFunctor_00e241b0 destructor.
#include <stddef.h>
void __cdecl operator delete(void* block);          // 0x00BFE9BC

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CCountedItem { };
struct SCountBlock {
    long m_references;                               // +0x00
    void (CCountedItem::*m_pfnDestroy)();            // +0x04 (called with the item as `this`)
    CCountedItem* m_pItem;                           // +0x08
};
class CGameScriptThing;
struct CCountedPointer_GameScriptThing {             // CCountedPointer<CGameScriptThing>
    CGameScriptThing* Data;                          // +0x00
    SCountBlock*      Info;                          // +0x04
    ~CCountedPointer_GameScriptThing()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                (Info->m_pItem->*Info->m_pfnDestroy)();
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CScriptThing : public CBaseClass { public:                   // vtable 0x01238C8C, 12 bytes
    CCountedPointer_GameScriptThing PImp;            // +0x04
    virtual ~CScriptThing() { }
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPos() const;         // slot 6 (+0x18)
};
class CGameScriptInterface { public:                     // vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv names)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090();
    virtual CScriptThing CreateCreature(const CCharString& defName, const C3DVector& position, const CCharString& scriptName, bool b) const;   // slot 91 (+0x16c)
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
};
class CScytheMarker {
public:
    void*                 __vftable;                 // +0x00
    CGameScriptInterface* Game;                      // +0x04
    CScriptThing          Me;                        // +0x08
    void*                 ParentClass;               // +0x14
    void*                 PMasterData;               // +0x18
    void Main();
};

void CScytheMarker::Main()
{
    CScriptThing scythe = Game->CreateCreature("CREATURE_RIVAL_HERO_SCYTHE", Me.GetPos(), "ScytheNearOracle", false);
    Game->RemoveThing(Me, false, true);
}