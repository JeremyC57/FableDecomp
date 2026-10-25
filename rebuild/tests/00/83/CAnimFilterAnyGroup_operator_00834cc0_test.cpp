#include <cstdio>
// Behaviour test for 0x00834CC0 CAnimFilterAnyGroup::operator(): every entry passes, called directly and through the
// vtable (slot 0). Links against the source object for the operator; the other virtuals are defined here.
class CAnimationEntry { public: long Group; };
class CAnimFilterBase {
public:
    CAnimFilterBase() { }
    virtual bool operator()(const CAnimationEntry& entry) const;
    virtual CAnimFilterBase* Clone() const;
    virtual ~CAnimFilterBase();
};
class CAnimFilterAnyGroup : public CAnimFilterBase {
public:
    CAnimFilterAnyGroup() { }
    virtual bool operator()(const CAnimationEntry& entry) const;
    virtual CAnimFilterBase* Clone() const;
};
bool CAnimFilterBase::operator()(const CAnimationEntry&) const { return false; }
CAnimFilterBase* CAnimFilterBase::Clone() const { return 0; }
CAnimFilterBase::~CAnimFilterBase() { }
CAnimFilterBase* CAnimFilterAnyGroup::Clone() const { return new CAnimFilterAnyGroup; }
int main()
{
    CAnimationEntry a, b; a.Group = 0; b.Group = 77;
    CAnimFilterAnyGroup any;
    const CAnimFilterBase& filter = any;
    CAnimFilterBase base;
    if (!any(a) || !any(b) || !filter(a) || !filter(b) || base(a)) { std::printf("00834cc0_TEST FAIL\n"); return 1; }
    CAnimFilterBase* copy = any.Clone();
    bool ok = copy && (*copy)(b);
    delete copy;
    if (!ok) { std::printf("00834cc0_TEST FAIL clone\n"); return 1; }
    std::printf("00834cc0_TEST PASS\n");
    return 0;
}
