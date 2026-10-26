// Behaviour test for CEngineInternalAttachedPrimitive::CEngineInternalAttachedPrimitive @ 0x00B3A910
#include <cstdio>
static int g_roots = 0, g_strings = 0; static long g_type = -1, g_flags = -1;
class CBaseClass { public: CBaseClass() { ++g_roots; } virtual ~CBaseClass() {} };
class CEnginePrimitiveHandle : public CBaseClass { public: void* m_pPrimitive; CEnginePrimitiveHandle() : CBaseClass(), m_pPrimitive(0) {} };
class CCharString { public: void* PStringData; CCharString() : PStringData(0) { ++g_strings; } };
class CEnginePrimitiveAttachmentInfo { public:
    unsigned int Flags; CCharString ParentDummyName; long ParentDummyIndex; CCharString ChildDummyName; long ChildDummyIndex;
    CEnginePrimitiveAttachmentInfo() : Flags(0) {}
};
class CEngineInternalPrimitiveBase { public: long m_type; CEngineInternalPrimitiveBase(long type, long flags) : m_type(type) { g_type = type; g_flags = flags; } virtual ~CEngineInternalPrimitiveBase() {} };
class CEngineInternalAttachedPrimitive : public CEngineInternalPrimitiveBase { public:
    CEnginePrimitiveHandle ParentHandle; long ParentGroupIndex; CEnginePrimitiveHandle OutputPrimitive; CEnginePrimitiveAttachmentInfo AttachmentInfo;
    CEngineInternalAttachedPrimitive() : CEngineInternalPrimitiveBase(0x2e, 0) {}
};
int main()
{
    CEngineInternalAttachedPrimitive p;
    if (g_type != 0x2e || g_flags != 0 || g_roots != 2 || g_strings != 2 || p.ParentHandle.m_pPrimitive || p.OutputPrimitive.m_pPrimitive ||
        p.AttachmentInfo.Flags != 0 || p.AttachmentInfo.ParentDummyName.PStringData) {
        std::printf("00b3a910_TEST FAIL\n"); return 1;
    }
    std::printf("00b3a910_TEST PASS\n");
    return 0;
}