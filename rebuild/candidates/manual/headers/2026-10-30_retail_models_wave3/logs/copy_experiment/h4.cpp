#include "/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad/mig3/work/x/chain.h"
class CCharString { public: void* m_pStringData; };
class CFrontEndDef : public CParentDefClassBase { public:
    std::vector<CCharString> vAttractModeMovie;
    long ErrorMessageBackgroundGraphic;
    long ButtonABigGraphic;
    long ButtonBBigGraphic;
    CFrontEndDef& operator=(const CFrontEndDef& o);
    virtual void Copy(const CDefClassBase* p);
};
CFrontEndDef& CFrontEndDef::operator=(const CFrontEndDef& o)
{ CParentDefClassBase::operator=(o); vAttractModeMovie = o.vAttractModeMovie; ErrorMessageBackgroundGraphic = o.ErrorMessageBackgroundGraphic;
  ButtonABigGraphic = o.ButtonABigGraphic; ButtonBBigGraphic = o.ButtonBBigGraphic; return *this; }
void CFrontEndDef::Copy(const CDefClassBase* p) { *this = *static_cast<const CFrontEndDef*>(p); }
