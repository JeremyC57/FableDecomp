// Behaviour test for 0x00689270 operator!=(const CIntelligentPointer<const CThing>&, const CBaseObject*): true exactly
// when the pointer's current item is not the object (a null item compares equal to a null object).
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

class CBaseObject { public: int Id; };

class CBaseIntelligentPointer {
public:
    CBaseObject* Item;
    CBaseObject* GetPItem() const { return Item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CThing : public CBaseObject { };

template <class T>
bool __fastcall operator!=(const CIntelligentPointer<T>& pointer, const CBaseObject* object)
{
    return pointer.GetPItem() != object;
}

int main()
{
    CThing a, b;
    CIntelligentPointer<const CThing> p;
    p.Item = &a;
    if (p != &a) { std::printf("00689270_TEST FAIL\n"); return 1; }
    if (!(p != &b)) { std::printf("00689270_TEST FAIL\n"); return 1; }
    if (!(p != 0)) { std::printf("00689270_TEST FAIL\n"); return 1; }
    p.Item = 0;
    if (p != 0) { std::printf("00689270_TEST FAIL\n"); return 1; }
    if (!(p != &a)) { std::printf("00689270_TEST FAIL\n"); return 1; }
    std::printf("00689270_TEST PASS\n");
    return 0;
}