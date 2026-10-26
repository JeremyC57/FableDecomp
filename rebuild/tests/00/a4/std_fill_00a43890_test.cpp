// behaviour test for 0x00a43890 (std::fill over CDataBankTextEntry (intrusive counted pointer + words))
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
static int g_destroyed = 0;
class CIVCountedPointeeBase { public:
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;                               // +0x04
};

template <class T>
class CIVCountedPointer { public:
    T* Object;                                  // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};

namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase { public: };
class CDataBankTextEntry { public:                     // 8 bytes
    CIVCountedPointer<CTextEntry> PText;        // +0x00
    long                          NarratorIndex; // +0x04
};
}
using NGameText::CDataBankTextEntry;

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(CDataBankTextEntry*, CDataBankTextEntry*, const CDataBankTextEntry&);
}

int main()
{
    
    NGameText::CTextEntry o1, o2, o3;
    o1.m_count = 1; o2.m_count = 5; o3.m_count = 1;
    CDataBankTextEntry v[3];
    v[0].PText.Object = &o1; v[1].PText.Object = &o2; v[2].PText.Object = &o3;
    v[0].NarratorIndex = 1; v[1].NarratorIndex = 2; v[2].NarratorIndex = 3;
    CDataBankTextEntry val; val.PText.Object = &o2; val.NarratorIndex = 42;
    std::fill(v, v + 2, val);
    // o1 released and destroyed; o2 self-assigned (no change in count); o2 gained one reference for v[0]
    int ok = g_destroyed == 1 && o1.m_count == 0 && o2.m_count == 6 && v[0].PText.Object == &o2 && v[1].PText.Object == &o2
          && v[0].NarratorIndex == 42 && v[1].NarratorIndex == 42 && v[2].NarratorIndex == 3 && v[2].PText.Object == &o3 && o3.m_count == 1;
    CDataBankTextEntry nul; nul.PText.Object = 0; nul.NarratorIndex = 0;
    std::fill(v, v + 1, nul);
    ok = ok && v[0].PText.Object == 0 && o2.m_count == 5;
    if (!ok) { printf("00a43890_TEST FAIL\n"); return 1; }
    printf("00a43890_TEST PASS\n");
    return 0;
}