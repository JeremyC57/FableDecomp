#include <cstdio>
#include <string>
#include <vector>
#include <utility>
// Model of ConstructExpressionCategoryList's decision logic: title suffix and which branch runs.
struct Log { std::wstring title; bool unavailable; bool arrow_deleted; unsigned long offsets; int lists_built; long default_child; };
static std::wstring GFIntToWideString(long v){ wchar_t b[32]; std::swprintf(b,L"%d",v); return b; }
static void Construct(const std::wstring& menuparam,const std::vector<std::pair<long,long> >& items,bool has_arrow,Log& log)
{
    std::wstring t(menuparam);
    if(!items.empty()){ t+=L" x"; t+=GFIntToWideString((long)items.size()); }
    log.title=t; log.unavailable=false; log.arrow_deleted=false; log.offsets=0; log.lists_built=0; log.default_child=-1;
    if(items.empty()){ if(has_arrow) log.arrow_deleted=true; log.unavailable=true; }
    else { log.offsets=(unsigned long)items.size(); log.default_child=0; log.lists_built=1; }
}
int main()
{
    std::vector<std::pair<long,long> > items; Log l;
    Construct(L"Taunts",items,true,l);
    if(l.title!=L"Taunts"||!l.unavailable||!l.arrow_deleted||l.lists_built){ std::printf("00627400_TEST FAIL\n"); return 1; }
    Construct(L"Taunts",items,false,l);
    if(l.arrow_deleted||!l.unavailable){ std::printf("00627400_TEST FAIL\n"); return 1; }
    items.push_back(std::make_pair(3L,1L)); items.push_back(std::make_pair(7L,2L)); items.push_back(std::make_pair(3L,5L));
    Construct(L"Taunts",items,true,l);
    if(l.title!=L"Taunts x3"||l.unavailable||l.offsets!=3||l.lists_built!=1||l.default_child!=0){ std::printf("00627400_TEST FAIL\n"); return 1; }
    std::printf("00627400_TEST PASS\n");
    return 0;
}