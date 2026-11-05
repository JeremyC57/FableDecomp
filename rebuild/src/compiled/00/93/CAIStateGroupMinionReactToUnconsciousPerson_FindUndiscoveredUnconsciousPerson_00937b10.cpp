// CAIStateGroup_MinionReactToUnconsciousPerson::FindUndiscoveredUnconsciousPerson() -- once a second (staggered by the
// Leaf: ?FindUndiscoveredUnconsciousPerson@CAIStateGroup_MinionReactToUnconsciousPerson@@QAEPAVCThingCreatureBase@@XZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// creature's UID), find the nearest not-yet-discovered unconscious ally the creature is aware of within a 7-unit box.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00937B10, length 261 bytes (+ int3 padding).
// Callees: GFGetCurrentFrame @ 0x0049D870, __allrem @ 0x00BFEC30 (signed 64-bit `(frame + UID) % fps`), CThing::
// GetPosition @ 0x004C73D0 (x2), __ftol2 @ 0x00BFEA70 (x2), CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind4<CThingFilter_IsAICreatureAwareOf, CThingFilter_IsUnconscious,
// CThingFilter_IsAnAlly, std::unary_negate<CThingFilter_HasBeenDiscovered>>, CThingCompare_Nearest, CThingCreatureBase>
// @ 0x00937420. Global: the frame-rate long
// @ 0x01375550 (read first).
// Shape: the landed filter-chain model (`a && b && c` binding pointers to frame temporaries; the empty filter is one
// byte); the result is zeroed up front and returned through its out slot.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/C3DVector.h"

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CThingSearchTools;
class CAIStateGroupBase {
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000 (Ego_r.pdb ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ, protected)
private: public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

struct CThingWithUID {                          // a thing seen through its 64-bit UID (+0x18)
    unsigned char _pad[0x18];
    __int64 UID;                                // +0x18 (read signed: the frame stagger uses __allrem)
};
class CThingCreatureBase;
struct CThingPos : CThing {
};
class C2DBoxI { public:
    long x0, y0, x1, y1;
    C2DBoxI(long ax0, long ay0, long ax1, long ay1) : x0(ax0), y0(ay0), x1(ax1), y1(ay1) { }
};
class CThingFilter_IsAICreatureAwareOf { public:
    const CThing* m_pCreature;
    CThingFilter_IsAICreatureAwareOf(const CThing* creature) : m_pCreature(creature) { }
};
class CThingFilter_IsUnconscious { public:
    CThingFilter_IsUnconscious() { }
};
class CThingFilter_IsAnAlly { public:
    const CThing* m_pCreature;
    CThingFilter_IsAnAlly(const CThing* creature) : m_pCreature(creature) { }
};
class CThingFilter_HasBeenDiscovered { public:
    CThingFilter_HasBeenDiscovered() { }
};
template<class F> struct unary_negate_ {        // std::unary_negate<F>: holds the predicate (one empty byte)
    F m_pred;
    explicit unary_negate_(const F& f) : m_pred(f) { }
};
template<class F> inline unary_negate_<F> not1_(const F& f) { return unary_negate_<F>(f); }
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};
template<class A, class B, class C, class D> class CFilter_Bind4 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    const D* m_pFourth;
    CFilter_Bind4(const CFilter_Bind3<A, B, C>& abc, const D& d)
        : m_pFirst(abc.m_pFirst), m_pSecond(abc.m_pSecond), m_pThird(abc.m_pThird), m_pFourth(&d) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }
template<class A, class B, class C, class D> inline CFilter_Bind4<A, B, C, D> operator&&(const CFilter_Bind3<A, B, C>& abc, const D& d) { return CFilter_Bind4<A, B, C, D>(abc, d); }
class CThingCompare_Nearest { public:
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
typedef CFilter_Bind3<CThingFilter_IsAICreatureAwareOf, CThingFilter_IsUnconscious, CThingFilter_IsAnAlly> TUnconsciousFilter;
typedef CFilter_Bind4<CThingFilter_IsAICreatureAwareOf, CThingFilter_IsUnconscious, CThingFilter_IsAnAlly,
    unary_negate_<CThingFilter_HasBeenDiscovered> > TUndiscoveredFilter;
bool ThingInBox(const CThing* thing, const C2DBoxI& box);
bool IsAwareOf(const CThing* creature, const CThing* thing);
bool IsUnconscious(const CThing* thing);
bool IsAlly(const CThing* creature, const CThing* thing);
bool HasBeenDiscovered(const CThing* thing);
const C3DVector& PositionOf(const CThing* thing);
class CThingSearchTools { public:
    CThing** m_things;                          // candidate list (stand-in model)
    CThing** m_thingsEnd;
    bool GetPBestThingInRoughArea(const C2DBoxI& box, const TUnconsciousFilter& filter,
        const CThingCompare_Nearest& compare, CThingCreatureBase** out) const;    // 0x00937690
    bool GetPBestThingInRoughArea(const C2DBoxI& box, const TUndiscoveredFilter& filter,
        const CThingCompare_Nearest& compare, CThingCreatureBase** out) const;    // 0x00937420
};
struct CAIStateGroup_MinionReactToUnconsciousPerson : CAIStateGroupBase {
    CThingCreatureBase* FindUnconsciousPerson();
    CThingCreatureBase* FindUndiscoveredUnconsciousPerson();
};
// GetPBestThingInRoughArea stand-in: the nearest candidate in the box passing every bound filter.
#pragma auto_inline(off)
bool CThingSearchTools::GetPBestThingInRoughArea(const C2DBoxI& box, const TUndiscoveredFilter& filter,
    const CThingCompare_Nearest& compare, CThingCreatureBase** out) const
{
    CThing* best = 0;
    float bestDistance = 0.0f;
    for (CThing** it = m_things; it != m_thingsEnd; ++it) {
        CThing* thing = *it;
        if (!ThingInBox(thing, box) || !IsAwareOf(filter.m_pFirst->m_pCreature, thing) || !IsUnconscious(thing)
                || !IsAlly(filter.m_pThird->m_pCreature, thing) || HasBeenDiscovered(thing))
            continue;
        const C3DVector& pos = PositionOf(thing);
        float dx = pos.X - compare.m_pos.X;
        float dy = pos.Y - compare.m_pos.Y;
        float dz = pos.Z - compare.m_pos.Z;
        float distance = dx * dx + dy * dy + dz * dz;
        if (!best || distance < bestDistance) {
            best = thing;
            bestDistance = distance;
        }
    }
    *out = (CThingCreatureBase*)best;
    return best != 0;
}
#pragma auto_inline(on)

CThingCreatureBase* CAIStateGroup_MinionReactToUnconsciousPerson::FindUndiscoveredUnconsciousPerson()
{
    __int64 uid = ((CThingWithUID*)PBrain->PCreature)->UID;
    long framesPerSecond = CMainGameComponent::GetConstantFPS();
    CThingCreatureBase* found = 0;
    if ((CWorld::GetFrame() + uid) % framesPerSecond == 0) {
        const C3DVector& pos = ((CThingPos*)PBrain->PCreature)->GetPos();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        C2DBoxI box(x - 7, y - 7, x + 7, y + 7);
        PeekThingSearchTools().GetPBestThingInRoughArea(box,
            CThingFilter_IsAICreatureAwareOf(PBrain->PCreature) && CThingFilter_IsUnconscious()
                && CThingFilter_IsAnAlly(PBrain->PCreature) && not1_(CThingFilter_HasBeenDiscovered()),
            CThingCompare_Nearest(((CThingPos*)PBrain->PCreature)->GetPos()), &found);
    }
    return found;
}