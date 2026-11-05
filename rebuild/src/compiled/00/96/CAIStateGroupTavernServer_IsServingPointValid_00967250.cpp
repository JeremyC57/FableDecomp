// CAIStateGroup_TavernServer::IsServingPointValid() -- is the reserved seat of the tavern table still valid
// Leaf: ?IsServingPointValid@CAIStateGroup_TavernServer_Methods@@QAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00967250, length 108 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// @ 0x0040F020 (inlined GetInterface, type 0xB8 = bit 24 of the presence word at +0x34),
// CTCTavernTable::IsSeatValid(long) @ 0x007B8570 (manifest label "CTCInventoryBase::GetSelectedCategory" is
// propagated across a byte-identical body and rejected).
//
// Shape: the table thing comes from PTable (+0x24); its tavern-table component (interface 0xB8) is fetched with
// the landed out-parameter GetInterface template (the component lands in eax, the miss path reads the never-
// written frame slot that also held the key). The seat index (+0x94, SeatIndex) is checked on the component;
// an invalid seat is forgotten (-1). The false path returns the call's own al (no `xor al,al`): the result is a
// named bool local returned on both paths, the true path's `mov al,1` is the constant-folded `return ok`.
#include "engine/CAIStateGroup_TavernServer.h"

class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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

class CTCTavernTable { public:                         // interface type 0xB8 component
    bool IsSeatValid(long seatIndex);           // 0x007B8570
};

struct CAIStateGroup_TavernServer_Methods : CAIStateGroup_TavernServer {
    CIntelligentPointer<CThing>& TablePtr() { return *(CIntelligentPointer<CThing>*)PTable; }
    bool IsServingPointValid();
};

bool CAIStateGroup_TavernServer_Methods::IsServingPointValid()
{
    CTCTavernTable* table;
    TablePtr().GetPItem()->GetTC(TCI_TAVERN_TABLE, &table);
    if (!table->IsSeatValid(SeatIndex)) {
        SeatIndex = -1;
        return false;
    }
    return true;
}