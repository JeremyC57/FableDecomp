// behaviour test for 0x004EB6C8: CMultiStaticMeshDef::Copy calls CDefClassBase::operator= with this and the source object, then
// vector<CMultiStaticMeshEntryDef>::operator= on Meshes (+0x28) of both
// NF337 (NF324 Q2 form): the row now defines the PDB member CMultiStaticMeshDef::Copy (?Copy@CMultiStaticMeshDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). Both callees (??4CDefClassBase@@QAEAAV0@ABV0@@Z and the vector's operator=) are stubbed as members here.
#include <cstdio>

class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other);
};
class CMultiStaticMeshEntryDef;
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:
    vector& operator=(const vector& __x);
};
}
class CMultiStaticMeshDef { public:
    virtual void Copy(const CDefClassBase* other);
};

static int g_calls = 0, g_vcalls = 0;
static const void* g_this = 0;
static const void* g_other = 0;
static const void* g_vthis = 0;
static const void* g_vother = 0;
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other)
{
    ++g_calls; g_this = this; g_other = &other;
    return *this;
}
template <> std::vector<CMultiStaticMeshEntryDef>& std::vector<CMultiStaticMeshEntryDef>::operator=(const std::vector<CMultiStaticMeshEntryDef>& x)
{
    ++g_vcalls; g_vthis = this; g_vother = &x;
    return *this;
}

static unsigned long g_dst[13];
static unsigned long g_src[13];

int main()
{
    ((CMultiStaticMeshDef*)(g_dst))->CMultiStaticMeshDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src &&
        g_vcalls == 1 && g_vthis == (const void*)((char*)g_dst + 0x28) && g_vother == (const void*)((char*)g_src + 0x28)) {
        std::printf("COPY_OK\n");
        return 0;
    }
    std::printf("FAIL_004eb6c8 calls=%d vcalls=%d\n", g_calls, g_vcalls);
    return 1;
}
