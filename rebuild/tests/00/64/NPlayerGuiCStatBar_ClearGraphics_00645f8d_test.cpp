// behaviour test for 0x00645F8D NPlayerGui::CStatBar::ClearGraphics (body verbatim, callees stubbed)
#include <stdio.h>
// NPlayerGui::CStatBar::ClearGraphics() -- clears every primitive handle (for_each + mem_fun_ref(&CEnginePrimitiveHandle::Clear)) and kills and forgets every live particle effect
// Grade: RELOCATION_MATCH -- the null pointer assigned to each killed effect is a NAMED local (`CIntelligentPointer<CParticleEmitter> none(0); *(Effects.begin() + i) = none;`): retail builds it before reloading begin and passes its frame address, and the shared element offset stays in edi (`mov edi,ebx ; shl edi,3`) across the calls instead of being folded into each lea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00645F8D, length 131 bytes.
// Leaf: ClearGraphics
// Label: the manifest's "C3DPrimitive2::GetUsedBones" is propagated; the body is virtual slot 1 of
// NPlayerGui::CStatBar (vtable 0x0125A738, rtti-vtables.tsv) and of three sibling stat-bar vtables
// (0x0125A748.., absolute references at 0x0125A73C/4C/5C/6C). The method name is descriptive.
// Callees: 0x0043F360 std::for_each<CEnginePrimitiveHandle*, mem_fun_ref_t> (/Gr `__fastcall`: hidden result
// in ecx, first in edx, last and the 4-byte functor on the stack), 0x0041BE70 CEnginePrimitiveHandle::Clear
// (the member-function pointer pushed as the functor), 0x00A01B50 CBaseIntelligentPointer::GetPItem,
// 0x00AEE8C0 CParticleEmitter::Kill, 0x00655318 CIntelligentPointer<T>(T*), 0x0065533C CIntelligentPointer<T>::operator=,
// 0x00A01C10 ~CBaseIntelligentPointer.
// Layout: PDB CStatBase with retail 12-byte VC6 vectors (landed ctor 0x0064CC11): PrimitiveHandles +0x30,
// Effects +0x3c (8-byte CIntelligentPointer<CParticleEmitter>: vptr + item).
// Shape: frame-pointer size TU (`leave`): `optimize("y",off)` + `("s",on)`. The element is reached as
// `*(begin() + i)` (the scaled index kept in edi, begin() re-read for every use); the reset is an assignment
// from a named null local whose 8-byte slot overlaps the dead for_each result.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CBaseObject;
class CEnginePrimitiveHandle { public:
    void Clear();                                        // 0x0041BE70
    unsigned long m_handle;
};

struct CMemFunRefClear {                                 // std::mem_fun_ref_t<void, CEnginePrimitiveHandle>
    void (CEnginePrimitiveHandle::*m_pmf)();
    explicit CMemFunRefClear(void (CEnginePrimitiveHandle::*pmf)()) : m_pmf(pmf) { }
};
CMemFunRefClear __fastcall for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CMemFunRefClear f);   // 0x0043F360

namespace NParticleEngine {
class CParticleEmitter { public:
    void Kill();                                         // 0x00AEE8C0
};
}

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                // +0x04
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    ~CBaseIntelligentPointer();                          // 0x00A01C10
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                        // 0x00655318
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x0065533C
};

template<class T> struct CRetailVector {                 // VC6 Dinkumware vector: first/last/end
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T* begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
};

namespace NPlayerGui {

class CStatBar { public:
    virtual void v00();
    virtual void ClearGraphics();                        // slot 1
    unsigned char                                                       _pad04[0x2c];
    CRetailVector<CEnginePrimitiveHandle>                               PrimitiveHandles;   // +0x30
    CRetailVector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;         // +0x3c
};

void CStatBar::ClearGraphics()
{
    for_each(PrimitiveHandles.m_pFirst, PrimitiveHandles.m_pLast, CMemFunRefClear(&CEnginePrimitiveHandle::Clear));
    for (unsigned long i = 0; i < Effects.size(); ++i) {
        if ((*(Effects.begin() + i)).GetPItem()) {
            ((NParticleEngine::CParticleEmitter*)(*(Effects.begin() + i)).GetPItem())->Kill();
            CIntelligentPointer<NParticleEngine::CParticleEmitter> none(0);
            *(Effects.begin() + i) = none;
        }
    }
}

}

// ---- stubs ----
static int g_cleared;
void CEnginePrimitiveHandle::Clear() { m_handle = 0; g_cleared++; }
CMemFunRefClear __fastcall for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CMemFunRefClear f)
{
    for (; first != last; ++first) (first->*f.m_pmf)();
    return f;
}
static int g_killed[8]; static int g_nk;
static NParticleEngine::CParticleEmitter g_em[4];
void NParticleEngine::CParticleEmitter::Kill() { g_killed[g_nk++] = (int)(this - g_em); }
void CBaseIntelligentPointer::v00() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static int g_dtors;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { g_dtors++; }
template<> CIntelligentPointer<NParticleEngine::CParticleEmitter>::CIntelligentPointer(NParticleEngine::CParticleEmitter* item) { m_pItem = (CBaseObject*)item; }
template<> CIntelligentPointer<NParticleEngine::CParticleEmitter>& CIntelligentPointer<NParticleEngine::CParticleEmitter>::operator=(const CIntelligentPointer& o) { m_pItem = o.m_pItem; return *this; }
void NPlayerGui::CStatBar::v00() { }

static int fails;
static void check(int c, const char* w) { if (!c) { fails++; printf("FAIL %s\n", w); } }

int main()
{
    NPlayerGui::CStatBar bar;
    CEnginePrimitiveHandle h[3] = { {1}, {2}, {3} };
    bar.PrimitiveHandles.m_pFirst = h; bar.PrimitiveHandles.m_pLast = h + 3; bar.PrimitiveHandles.m_pEnd = h + 3;
    CIntelligentPointer<NParticleEngine::CParticleEmitter> e[4] = { &g_em[0], 0, &g_em[2], &g_em[3] };
    bar.Effects.m_pFirst = e; bar.Effects.m_pLast = e + 4; bar.Effects.m_pEnd = e + 4;
    int d0 = g_dtors;
    bar.ClearGraphics();
    check(g_cleared == 3 && h[0].m_handle == 0 && h[2].m_handle == 0, "handles cleared");
    check(g_nk == 3 && g_killed[0] == 0 && g_killed[1] == 2 && g_killed[2] == 3, "live effects killed");
    check(e[0].m_pItem == 0 && e[2].m_pItem == 0 && e[3].m_pItem == 0 && g_dtors - d0 == 3, "effects reset");
    // empty vectors
    bar.PrimitiveHandles.m_pLast = h; bar.Effects.m_pLast = e; g_nk = 0; g_cleared = 0;
    bar.ClearGraphics();
    check(g_cleared == 0 && g_nk == 0, "empty");
    if (!fails) printf("00645f8d_TEST PASS\n");
    return fails;
}