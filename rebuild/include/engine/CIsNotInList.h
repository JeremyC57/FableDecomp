#ifndef FABLE_ENGINE_CISNOTINLIST_H
#define FABLE_ENGINE_CISNOTINLIST_H
#ifdef FABLE_RETAIL_CAISTATEGROUP_TESTTESTTEST_PASS27
namespace {
class CIsNotInList {
    std::vector<const CThingObject*> Things;
public:
    CIsNotInList(std::vector<const CThingObject*>& things) : Things(things) {}
    bool operator()(const CThingObject&) const;
};
}
#endif
#endif
