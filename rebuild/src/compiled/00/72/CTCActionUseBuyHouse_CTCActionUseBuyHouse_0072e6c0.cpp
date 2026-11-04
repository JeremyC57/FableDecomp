// CTCActionUseBuyHouse::~CTCActionUseBuyHouse() -- buy-house action-use component dtor: leave the global-update TC list, release the buy-house def, then the inline ~CTCActionUseBase (its action-use def) and ~CTCBase
// Leaf: ??1CTCActionUseBuyHouse@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072E6C0.
// Identity: Ego_r.pdb ??1CTCActionUseBuyHouse@@UAE@XZ; base CTCActionUseBase (PActionUseDef +0xc, Usable +0x10, implicit dtor),
//   own PDef CDefPointer<const CBuyHouseDef> +0x14.
// Shape: `RemoveFromGlobalTCs();` then the def pointers released in reverse order and ~CTCBase.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CActionUseDef;
class CBuyHouseDef;

class CTCActionUseBase : public CTCBase {
public:
    CDefPointer<const CActionUseDef> PActionUseDef;                        // +0x0c
    bool                             Usable;                               // +0x10
};

enum EHouseQuestion { };

class CTCActionUseBuyHouse : public CTCActionUseBase {
public:
    CTCActionUseBuyHouse(CThing&);
    virtual ~CTCActionUseBuyHouse();
    CDefPointer<const CBuyHouseDef>  PDef;                                 // +0x14
    bool                             WaitingForConfirmation;               // +0x18
    EHouseQuestion                   HouseQuestion;                        // +0x1c
};
FABLE_STATIC_ASSERT(sizeof(CTCActionUseBuyHouse) == 0x20);

CTCActionUseBuyHouse::~CTCActionUseBuyHouse()
{
    RemoveFromGlobalTCs();
}