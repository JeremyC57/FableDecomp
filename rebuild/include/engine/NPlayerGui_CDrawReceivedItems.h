#ifndef FABLE_NPLAYERGUI_CDRAWRECEIVEDITEMS_H
#define FABLE_NPLAYERGUI_CDRAWRECEIVEDITEMS_H
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWRECEIVEDITEMS_PASS23
#include "engine/NPlayerGui_CDrawBase.h"
namespace NPlayerGui {
// Both original complete classes: no data beyond CDrawBase.
class CDrawReceivedItems:public CDrawBase {public:
 CDrawReceivedItems(CPlayerGui&,std::vector<CDrawBase*>&);
 ~CDrawReceivedItems();
 void Draw(const CEnginePrimitivePrepareInfo&);
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawReceivedItems)==40);
#endif
#endif
