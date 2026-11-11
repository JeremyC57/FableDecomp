// std::__copy_backward(std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >* first, std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >* last, std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@PAU12@H@std@@YIPAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBAE30, 143 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00cbc3c0 source (STL_linearinsert_00cbc3c0.cpp) names std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>>; binder ??$__copy_backward@PAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@PAU12@H@std@@YIPAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>>.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_NSCRIPT
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CEntityScriptBindingBase.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};
template std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >* std::__copy_backward<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >*,std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >*,int >(std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >*, std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >*, std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >*, std::random_access_iterator_tag const&, int*);
