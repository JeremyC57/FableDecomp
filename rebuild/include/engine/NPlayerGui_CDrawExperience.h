#ifndef FABLE_NPLAYERGUI_CDRAWEXPERIENCE_H
#define FABLE_NPLAYERGUI_CDRAWEXPERIENCE_H
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWEXPERIENCE_PASS23
#include "engine/NPlayerGui_CDrawBase.h"
namespace NPlayerGui {
// Both original class records: CDrawBase only, no added fields.
class CDrawExperience:public CDrawBase {
public:
 CDrawExperience(CPlayerGui&,std::vector<CDrawBase*>&);
 ~CDrawExperience();
 void Draw(const CEnginePrimitivePrepareInfo&);
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawExperience)==40);
#endif
#endif
