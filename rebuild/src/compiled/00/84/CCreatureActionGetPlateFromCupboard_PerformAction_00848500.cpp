// CCreatureAction_GetPlateFromCupboard::PerformAction() -- create the plate thing at the cupboard, put it in the creature's carry slot and add creature mode 5
// Leaf: ?PerformAction@CCreatureAction_GetPlateFromCupboard@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00848500, 275 bytes, virtual PerformAction.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E), CBaseIntelligentPointer::GetPItem 0x00A01B50
//   (PCreature +0x08), CThing::GetPos 0x004C73D0, CreateObject(long, const C3DVector*, long, float*, unsigned long,
//   const CCharString&) 0x00703210 (__fastcall; landed CAIStateGroupBase::CreateItem form), ~CCharString 0x0099EAE0,
//   CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface: type
//   0x46 = bit 6 of the word at +0x28, CTCCarrying; type 0x31 = bit 17 of the word at +0x24, CTCCreatureModeManager),
//   CTCCarrying::PutThingInCarrySlot(CThing&, long, bool) 0x006A1F80, CTCCreatureModeManager::AddMode 0x0073AE10.
// Layout: retail CCreatureActionBase is 0xa8 bytes; this class's cupboard thing pointer at +0xa8 and plate def index at
//   +0xac; CThing +0x70 the def, +0x90 the player byte (signed); CCarryableDef +0x28 the carry slot.
// Shape: the empty name is an implicit conversion built before any other argument; the player id is an inline
//   long-returning accessor used straight as the CreateObject argument (a named local / byte accessor rotate the
//   movsx and the two frame-address pushes); both component lookups are unchecked (retail reads the uninitialised
//   out variable's slot on the miss path) and the inline GetInterface stores its entry pick as ONE ternary so the
//   carrying component lands straight in ecx; the carryable def is a CDefPointer local released at the end
//   (DecRefToZero, null test folded after the dereference).
#include "fable_compiler.h"
class CThing;
class CThingObject;
#include "engine/C3DVector.h"
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCARRYABLEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE   // agentNF25b: the GetSubDef<CCarryableDef> template body (0x005CC10E) kept out of line, visible to the TU
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad54[0x1c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x90 - 0x74];
    signed char m_player;                             // +0x90
    long GetPlayer() const { return m_player; }
    const C3DVector& GetPos() const;             // 0x004C73D0
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingObject : public CThing { public: };
CThingObject* __fastcall CreateObject(long definition, const C3DVector* position, long playerID,
                                      float* orientation, unsigned long flags, const CCharString& name);   // 0x00703210
class CBaseObject;
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCCarrying { public:
    void PutThingInCarrySlot(CThing& thing, long slot, bool immediately);   // 0x006A1F80 (Ego_r/FableWin ?PutThingInCarrySlot@CTCCarrying@@QAEXAAVCThing@@J_N@Z)
};
class CTCCreatureModeManager { public:
    bool AddMode(unsigned long mode);                 // 0x0073AE10
};
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
struct CCreatureAction_GetPlateFromCupboard : CCreatureActionBase {
    CThing* m_pCupboard;                              // +0xa8
    long m_plateDefIndex;                             // +0xac
    void PerformAction();
};

void CCreatureAction_GetPlateFromCupboard::PerformAction()
{
    CThingObject* plate;
    {
        CCharString name("");
        plate = CreateObject(m_plateDefIndex, &m_pCupboard->GetPos(), PCreature.GetPItem()->GetPlayer(), 0, 0, name);
    }
    CDefPointer<const CCarryableDef> carryDef;
    plate->m_pDef->GetSubDef(&carryDef);
    CTCCarrying* carrying;
    PCreature.GetPItem()->GetInterface(&carrying, 0x46);
    long slot = carryDef->ActiveCarrySlot;
    carrying->PutThingInCarrySlot(*plate, slot, true);
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->AddMode(5);
}