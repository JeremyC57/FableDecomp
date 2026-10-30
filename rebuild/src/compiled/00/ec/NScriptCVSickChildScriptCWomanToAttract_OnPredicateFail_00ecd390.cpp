// NScript::CV_SickChildScript::CWomanToAttract::OnPredicateFail() -- if the woman was killed (by anyone) before she handed over her object, gives the hero the ingredient object and the KilledSomeoneForMushroom morality and records LaughingWomanKilled on the quest
// Leaf: ?OnPredicateFail@CWomanToAttract@CV_SickChildScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ECD390, 177 bytes (padded to 192), plain `ret`.
//   PDB name ?OnPredicateFail@CWomanToAttract@CV_SickChildScript@NScript@@EAEXXZ.
// Callees: CScriptThing slot 18 MsgIsKilledBy(const CCharString&) (+0x48, 0x004AAAF0) on the entity's own thing (+8);
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; CDefString::operator CCharString() 0x00415D70;
//   CGameScriptInterface slot 121 GiveHeroObject(const CCharString&, long, bool) (+0x1e4, 0x008902E0) and slot 156
//   GiveHeroMorality(float) (+0x270, 0x00899EF0). String "" 0x0122D70E.
// Data: the script def 0x0143E90C: IngredientObject +0x738 (Ego_r.pdb 1940 - 0x5c), KilledSomeoneForMushroom +0x768
//   (1992 - 0x60: the 12-byte retail vector between them).
// Shape: `bool killed = !GivenObject && Me.MsgIsKilledBy("");` -- a frame-byte bool and the conditional temporary's
//   construction flag in ebx (its frame word pre-zeroed); the def string converts into a by-value temporary built
//   before the other arguments.
// Layout (Ego_r.pdb, entity scripts match): interface +4, Me +8, ParentClass +0x14, PMasterData +0x18, GivenObject +0x1c;
//   CV_SickChildScript LaughingWomanKilled retail +0x71 (PDB 133 - 0x14).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefString { public:
    long Index;
    operator CCharString() const;                  // 0x00415D70
};
class CScriptDef { public:
    unsigned char _pad[0x738];
    CDefString IngredientObject;                   // +0x738
    unsigned char _pad73c[0x768 - 0x73c];
    float KilledSomeoneForMushroom;                // +0x768
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CScriptThingMethods {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptThing : public CScriptThingMethods { public:        // 12 bytes, vtable 0x01238C8C
    void* PImp_Data;
    void* PImp_Info;
};
namespace NScript {
class CV_SickChildScript { public:
    unsigned char _pad[0x71];
    bool LaughingWomanKilled;                      // +0x71
    struct CWomanToAttract {
        virtual void v00();
        CGameScriptInterface* Game;                // +0x04
        CScriptThing          Me;                  // +0x08
        CV_SickChildScript*   ParentClass;         // +0x14
        void*                 PMasterData;         // +0x18
        bool                  GivenObject;         // +0x1c
        long                  TimesMadeLaugh;      // +0x20
        virtual void OnPredicateFail();
    };
};
void CV_SickChildScript::CWomanToAttract::OnPredicateFail()
{
    bool killed = !GivenObject && Me.MsgIsKilledBy("");
    if (killed) {
        Game->GiveHeroObject(pGlobalScriptDef->IngredientObject, -1, false);
        Game->GiveHeroMorality(pGlobalScriptDef->KilledSomeoneForMushroom);
        ParentClass->LaughingWomanKilled = true;
    }
}
}