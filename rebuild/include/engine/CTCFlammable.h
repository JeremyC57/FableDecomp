#ifndef FABLE_ENGINE_CTCFLAMMABLE_H
#define FABLE_ENGINE_CTCFLAMMABLE_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS
// Opt-in (agentNF22, row 0x00780410 CTCFlammable::SetOnFire): the Ego_r.pdb members up to FirePoints. PDef +0x0c
// (CDefPointer<const CFlammableDef>, one word), FirePoints +0x10 = vector<NFire::CFirePoint> (retail STLport, 12 bytes;
// CFirePoint is 0x2c bytes, Resistance +0x0c). The row that opts in completes NFire::CFirePoint.
#include "stlport/fable_containers.h"
namespace NFire { class CFirePoint; }
#endif
// Declared callable interface only; no instance is constructed or sized by this row.
class CTCFlammable:public CTCBase {public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_FLAMMABLE;}
 bool IsOnFire()const; void SetOnFire();
#ifdef FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS
 const void* PDef;                                  // +0x0c CDefPointer<const CFlammableDef>
 std::vector<NFire::CFirePoint> FirePoints;         // +0x10
#endif
};
#endif
