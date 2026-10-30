#pragma optimize("g", on)
#include "/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad/mig3/work/x/chainH.h"
CParentDefClassBase& CParentDefClassBase::operator=(const CParentDefClassBase& o)
{ CDefClassBase::operator=(o); InstantiationName = o.InstantiationName; SubDefInfoMap = o.SubDefInfoMap; return *this; }
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& o)
{ CDefPointeeBase::operator=(o); PDefManager = o.PDefManager; GlobalIndex = o.GlobalIndex; Setup = o.Setup; Template = o.Template; DefaultValsApplied = o.DefaultValsApplied; return *this; }
class CCharString { public: void* m_pStringData; };
class CFrontEndDef : public CParentDefClassBase { public:
    std::vector<CCharString> vAttractModeMovie;
    long ErrorMessageBackgroundGraphic;
    long ButtonABigGraphic;
    long ButtonBBigGraphic;
    virtual void Copy(const CDefClassBase* p);
};
void CFrontEndDef::Copy(const CDefClassBase* p) { *this = *static_cast<const CFrontEndDef*>(p); }
