class CAnimFilterBase
// Leaf: GetAnimFilter
{
public:
    int filterData[1];
};

class CModeBlock
{
public:
    CAnimFilterBase* GetAnimFilter();
private:
    void* vfptr;
    void* ParentThing;
    CAnimFilterBase animFilter;
};

CAnimFilterBase* CModeBlock::GetAnimFilter()
{
    return &animFilter;
}