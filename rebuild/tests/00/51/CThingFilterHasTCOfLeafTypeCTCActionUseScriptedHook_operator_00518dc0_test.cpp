// Behaviour test for 0x00518DC0 CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>::operator()(thing): true only
// when the thing has interface 0xC2 and that component's class type name is "CTCActionUseScriptedHook".
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570 (strcmp-style, 0 = equal)

class CCharStringData { public:
    char*         m_pData;                      // +0x0
    unsigned long m_length;                     // +0x4
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, int length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
    bool SameText(const CCharStringData& other) const
    {
        if (PStringData->m_length != other.m_length)
            return false;
        return Compare_00411570(PStringData->m_pData, other.m_pData) ? false : true;
    }
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return SameText(*other.PStringData);
    }
};

enum ETCInterfaceType { TC_INTERFACE_ACTION_USE_SCRIPTED_HOOK = 0xC2 };   // enumerator name descriptive

class CThing;
class CTCBase {
public:
    virtual ~CTCBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetClassTypeName() const;   // slot 22 (+0x58)
    CThing* ParentThing;                        // +0x04
};
class CTCActionUseScriptedHook : public CTCBase {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_INTERFACE_ACTION_USE_SCRIPTED_HOOK; }
    static CCharString StatGetClassTypeName() { return CCharString("CTCActionUseScriptedHook"); }   // literal 0x01239FE4
};

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);       // 0x0040F020
    __forceinline CTCBase* Find(ETCInterfaceType type)
    {
        SEntry* it = LowerBound(type);
        SEntry* sel = (it == last || type < it->type) ? last : it;
        return sel->component;
    }
};

__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long& type)
{
    SEntry* lo = first;
    int len = last - first;
    while (len > 0) {
        int half = len >> 1;
        SEntry* middle = lo + half;
        if (middle->type < type) {
            lo = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return lo;
}

static int g_ctors = 0, g_dtors = 0;
long __fastcall Compare_00411570(const char* a, const char* b) { return strcmp(a, b); }
CCharString::CCharString(const char* text, int length)
{
    ++g_ctors;
    PStringData = (CCharStringData*)malloc(sizeof(CCharStringData));
    PStringData->m_pData = (char*)text;
    PStringData->m_length = length < 0 ? (unsigned long)strlen(text) : (unsigned long)length;
}
CCharString::~CCharString() { ++g_dtors; }
CTCBase::~CTCBase() { }
void CTCBase::v01() { } void CTCBase::v02() { } void CTCBase::v03() { } void CTCBase::v04() { } void CTCBase::v05() { }
void CTCBase::v06() { } void CTCBase::v07() { } void CTCBase::v08() { } void CTCBase::v09() { } void CTCBase::v10() { }
void CTCBase::v11() { } void CTCBase::v12() { } void CTCBase::v13() { } void CTCBase::v14() { } void CTCBase::v15() { }
void CTCBase::v16() { } void CTCBase::v17() { } void CTCBase::v18() { } void CTCBase::v19() { } void CTCBase::v20() { }
void CTCBase::v21() { }
CCharString CTCBase::GetClassTypeName() const { return CCharString("CTCBase"); }
class CTestHook : public CTCBase {
public:
    const char* Name;
    virtual CCharString GetClassTypeName() const { return CCharString(Name); }
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned int m_presentBits[9];             // +0x20 bitset<274> of present interface types
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline unsigned int HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1U << (interfaceType & 31));
    }
    template <class T> __forceinline bool GetTCOfLeafType(T** out)
    {
        if (HasTC(T::StatGetInterfaceType())) {
            CTCBase* tc = m_interfaces.Find(T::StatGetInterfaceType());
            if (tc->GetClassTypeName() == T::StatGetClassTypeName()) {
                *out = (T*)tc;
                return true;
            }
        }
        return false;
    }
};

template <class T> class CThingFilter_HasTCOfLeafType {
public:
    CThingFilter_HasTCOfLeafType() {}
    bool operator()(CThing& thing) const;
};
template <class T> bool CThingFilter_HasTCOfLeafType<T>::operator()(CThing& thing) const
{
    T* tc;
    return thing.GetTCOfLeafType(&tc);
}



int main()
{
    CTestHook hook; hook.Name = "CTCActionUseScriptedHook"; hook.ParentThing = 0;
    CTestHook other; other.Name = "CTCActionUseScriptedHooK"; other.ParentThing = 0;   // same length, one letter off
    CTestHook shortName; shortName.Name = "CTCAction"; shortName.ParentThing = 0;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0xC2; entries[1].component = &hook;
    entries[2].type = 0xD0; entries[2].component = 0;
    CThing* thing = (CThing*)malloc(sizeof(CThing)); memset(thing, 0, sizeof(*thing));
    thing->m_interfaces.first = entries; thing->m_interfaces.last = entries + 3;
    CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook> filter;
    int ok = 1;
    if (filter(*thing)) ok = 0;                                         // the present bit is clear
    if (g_ctors != 0) ok = 0;                                           // no names built without the interface
    thing->m_presentBits[0xC2 >> 5] |= 1U << (0xC2 & 31);
    if (!filter(*thing)) ok = 0;
    entries[1].component = &other;
    if (filter(*thing)) ok = 0;
    entries[1].component = &shortName;
    if (filter(*thing)) ok = 0;
    if (g_ctors != 6 || g_dtors != 6) ok = 0;                           // both temporaries destroyed each time
    printf(ok ? "00518dc0_TEST PASS\n" : "00518dc0_TEST FAIL\n");
    return ok ? 0 : 1;
}