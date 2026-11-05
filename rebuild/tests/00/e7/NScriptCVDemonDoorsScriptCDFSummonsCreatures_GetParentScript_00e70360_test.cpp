// behaviour test for 0x00E70360 NScript::CV_DemonDoorsScript::CDF_SummonsCreatures::GetParentScript: returns the parent level
// script the entity was built with (the ParentClass reference), as a CScriptBase*, for two entities with two different parents.
#include <cstdio>
class CScriptBase { public: virtual ~CScriptBase() { } int Tag; };
namespace NScript {
class CV_DemonDoorsScript : public CScriptBase {
public:
    class CDF_SummonsCreatures {
    public:
        CDF_SummonsCreatures(CV_DemonDoorsScript& parent) : ParentClass(parent), PMasterData(0) { }
        virtual ~CDF_SummonsCreatures() { }
        virtual CScriptBase* GetParentScript() const;
    protected:
        CV_DemonDoorsScript& ParentClass;
        void* PMasterData;
    };
};
CScriptBase* CV_DemonDoorsScript::CDF_SummonsCreatures::GetParentScript() const
{
    return &ParentClass;
}
}
int main()
{
    NScript::CV_DemonDoorsScript a, b;
    a.Tag = 1; b.Tag = 2;
    NScript::CV_DemonDoorsScript::CDF_SummonsCreatures ea(a), eb(b);
    CScriptBase* pa = ea.GetParentScript();
    CScriptBase* pb = eb.GetParentScript();
    if (pa != &a || pb != &b || pa->Tag != 1 || pb->Tag != 2) { std::printf("00e70360_TEST FAIL\n"); return 1; }
    std::printf("00e70360_TEST PASS\n");
    return 0;
}