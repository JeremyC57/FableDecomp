// CTCAssassinRush::OnInitialActivate() -- start the rush: enter creature mode 0x25, fade the hero and every live carried item to the def's alpha (remembering the items), then either suspend a non-dead player-type thing's animation or read the stick override
// Leaf: OnInitialActivate
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail 0x007AA6B0, 368 bytes, 8-aligned ebp frame, `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (interfaces 0x31 mode manager, 0x46 inventory),
// CTCCreatureModeManager::AddMode 0x0073AE10, CTCGraphicAppearance::SetAlpha(uchar) 0x004BFAC0, the inventory's item
// collector 0x006A2220 (vector<CThing*>&), CBaseIntelligentPointer ctor/SetPItem/dtor 0x00A01B10/0x00A01B90/0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C), vector<CIntelligentPointer<CThing>>::push_back 0x004AD520, free
// 0x00BFEA14, the thing's 0x00833A10 (bool, bool), CTCAssassinRush::UpdateDirectionOverride 0x007A9960.
// Layout = Ego_r.pdb CTCAssassinRush: ParentThing +4, PDef +0x14 (def alpha byte +0x74), ItemAlphaList +0x64; CThing
// interface bitset +0x20, map +0x44, graphic appearance +0x64, dead flag +0x91 bit 0, thing type byte +0x10.
// Shape: the mode manager comes from the out-parameter GetInterface (component kept in eax), the inventory from HasTC + a
// ternary GetTC into a named local declared before the zeroed item vector (so the zero stores follow the lookup); the
// loop caches the end pointer; each live item goes through an intelligent-pointer temporary into push_back. The 8-byte
// stack alignment (`and esp,-8`) comes out of VC7.1 for this body as written (no double local is needed).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CTCBase;
class CBaseObject;
class CThing;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void SetAlpha(unsigned char alpha);                    // 0x004BFAC0
};
}
using NGraphicAppearance::CTCGraphicAppearance;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template <class T> struct CRetailVector {                  // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector() { if (_M_start) free(_M_start); }
    void push_back(const T& value);                        // 0x004AD520
};
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};

class CTCCreatureModeManager { public:
    bool AddMode(unsigned long mode);                      // 0x0073AE10
};
class CTCInventory { public:
    void GetAllItems(CRetailVector<CThing*>& items);       // 0x006A2220
};
class CSpecialAbilitiesAssassinRushDef { public:
    unsigned char _pad00[0x74];
    unsigned char Alpha;                                   // +0x74
};
class CTCAssassinRush {
public:
    void*                                      __vftable;  // +0x00
    CThing*                                    ParentThing; // +0x04
    unsigned char                              _pad08[0x14 - 0x08];
    CSpecialAbilitiesAssassinRushDef*          PDef;       // +0x14
    unsigned char                              _pad18[0x64 - 0x18];
    CRetailVector<CIntelligentPointer<CThing> > ItemAlphaList;   // +0x64
    void UpdateDirectionOverride();                        // 0x007A9960
    void OnInitialActivate();
};

void CTCAssassinRush::OnInitialActivate()
{
    CThing* thing = ParentThing;
    CTCCreatureModeManager* modes;
    if (thing->GetTC(TCI_ENTITY_MODE_MANAGER, &modes))
        modes->AddMode(0x25);
    if (thing->GetTCGraphicAppearance())
        thing->GetTCGraphicAppearance()->SetAlpha(PDef->Alpha);
    if (thing->HasTC(TCI_CARRYING)) {
        CTCInventory* inventory = ((CTCInventory*)&thing->GetTC(TCI_CARRYING));
        CRetailVector<CThing*> items;
        inventory->GetAllItems(items);
        for (CThing** it = items._M_start, ** end = items._M_finish; it != end; ++it) {
            CThing* item = *it;
            if (item && !item->IsDead()) {
                ItemAlphaList.push_back(CIntelligentPointer<CThing>(item));
                if (item->GetTCGraphicAppearance())
                    item->GetTCGraphicAppearance()->SetAlpha(PDef->Alpha);
            }
        }
    }
    if (!thing->IsDead() && thing->GetType() == 1)
        ((CThingAICreature*)ParentThing)->SuspendAI(true, (void*)false);
    else
        UpdateDirectionOverride();
}