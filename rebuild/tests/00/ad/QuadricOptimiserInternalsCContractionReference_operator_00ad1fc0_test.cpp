#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
static int g_deletedData = 0;
static void __fastcall DeleteData(void* data) { ++g_deletedData; }

class CCPPointerInfo { public:
    unsigned long RefCount;                                  // +0x00
    void (__fastcall* DeleteFunc)(void* data);               // +0x04
    void*         Data;                                      // +0x08
};

template<class T> class CCountedPointer { public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

namespace QuadricOptimiserInternals {

class CVertexPairContraction;

class CContractionReference { public:                               // 12 bytes
    CCountedPointer<CVertexPairContraction> Contraction;     // +0x00
    unsigned long                           SubIndex;        // +0x08
    CContractionReference& operator=(const CContractionReference& other);
};

CContractionReference& CContractionReference::operator=(const CContractionReference& other)
{
    Contraction = other.Contraction;
    SubIndex = other.SubIndex;
    return *this;
}

}

int main()
{
    using QuadricOptimiserInternals::CContractionReference;
    CCPPointerInfo* infoA = new CCPPointerInfo; infoA->RefCount = 1; infoA->DeleteFunc = DeleteData; infoA->Data = 0;
    CCPPointerInfo* infoB = new CCPPointerInfo; infoB->RefCount = 1; infoB->DeleteFunc = DeleteData; infoB->Data = 0;
    CContractionReference x, y;
    x.Contraction.Data = (QuadricOptimiserInternals::CVertexPairContraction*)infoA; x.Contraction.Info = infoA; x.SubIndex = 1;
    y.Contraction.Data = (QuadricOptimiserInternals::CVertexPairContraction*)infoB; y.Contraction.Info = infoB; y.SubIndex = 7;
    CContractionReference& r = (x = y);              // A released (count 1 -> 0: deleted), B shared
    if (&r != &x || x.Contraction.Info != infoB || x.Contraction.Data != y.Contraction.Data || x.SubIndex != 7 ||
        infoB->RefCount != 2 || g_deletedData != 1) {
        std::printf("00ad1fc0_TEST FAIL\n");
        return 1;
    }
    x.SubIndex = 3;
    x = y;                                           // same block: only the sub-index copies
    if (infoB->RefCount != 2 || x.SubIndex != 7 || g_deletedData != 1) {
        std::printf("00ad1fc0_TEST FAIL\n");
        return 1;
    }
    CContractionReference z;
    z.Contraction.Data = 0; z.Contraction.Info = 0; z.SubIndex = 9;
    x = z;                                           // B drops to 1, x becomes empty
    if (x.Contraction.Info != 0 || x.Contraction.Data != 0 || infoB->RefCount != 1 || g_deletedData != 1 || x.SubIndex != 9) {
        std::printf("00ad1fc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00ad1fc0_TEST PASS\n");
    return 0;
}