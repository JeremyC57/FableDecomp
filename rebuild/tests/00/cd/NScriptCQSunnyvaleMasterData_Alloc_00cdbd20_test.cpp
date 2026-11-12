// Behaviour test for pSunnyvaleDataAlloc_func (0x00CDBD20): allocate a CQ_SunnyvaleMasterData -- base constructed,
// TeddySolution default-constructed, the BoastRegions vector empty -- and return it as its CScriptDataBase.
#include <cstdio>

static int g_baseCtors = 0;
static int g_stringCtors = 0;
class CCharString {
public:
    void* PStringData;
    CCharString() : PStringData(0) { ++g_stringCtors; }
};
template <class T> class CVector {
public:
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
class CScriptDataBase {
protected:
    CScriptDataBase() { ++g_baseCtors; }
public:
    virtual ~CScriptDataBase() { }
    virtual int Kind() const { return 0; }
};
class CQ_SunnyvaleMasterData : public CScriptDataBase {
public:
    CQ_SunnyvaleMasterData() { }
    virtual int Kind() const { return 1; }
    unsigned char _gameflowState04[0x50];
    CCharString TeddySolution;
    unsigned char _gameflowState58[0xDC];
    CVector<CCharString> BoastRegions;
    bool SingingStonesInSync;
};

CScriptDataBase* pSunnyvaleDataAlloc_func()
{
    return new CQ_SunnyvaleMasterData;
}

int main()
{
    CScriptDataBase* data = pSunnyvaleDataAlloc_func();
    CQ_SunnyvaleMasterData* master = (CQ_SunnyvaleMasterData*)data;
    if (data == 0 || data->Kind() != 1 || g_baseCtors != 1 || g_stringCtors != 1 || master->TeddySolution.PStringData != 0
        || master->BoastRegions._M_start != 0 || master->BoastRegions._M_finish != 0 || master->BoastRegions._M_end_of_storage != 0) {
        std::printf("00cdbd20_TEST FAIL\n");
        return 1;
    }
    delete data;
    std::printf("00cdbd20_TEST PASS\n");
    return 0;
}