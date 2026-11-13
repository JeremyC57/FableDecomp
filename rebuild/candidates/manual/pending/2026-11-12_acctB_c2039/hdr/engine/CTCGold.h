#ifndef FABLE_ENGINE_CTCGOLD_H
#define FABLE_ENGINE_CTCGOLD_H
// New header (chunk 2039, row 0x006ACA30 CThingPlayerCreature::OnCollisionWithObject): the gold thing component on the polymorphic
// CTCBase (define FABLE_RETAIL_CTCBASE). Ego_r.pdb CTCGold is 16 bytes, Gold at +0x0C (struct_layouts_egor.tsv). FableWin.pdb
// ?StatGetInterfaceType@CTCGold@@SI?AW4ETCInterfaceType@@XZ = TCI_GOLD 204 (the row's PeekTC key 0CCh); Ego_r.pdb / FableWin.pdb
// ?GetGold@CTCGold@@QBEJXZ, out of line in retail at 0x007CBDC0 (landed under that spelling).
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCGold : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_GOLD; }
    long GetGold() const;                                                // 0x007CBDC0
private:
    long Gold;                                                           // +0x0c
};
#endif
