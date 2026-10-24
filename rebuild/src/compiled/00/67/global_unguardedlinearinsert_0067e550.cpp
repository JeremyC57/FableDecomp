// _global::__unguarded_linear_insert(CDegradableInfo* last, CDegradableInfo value, CDegradableDef::CIsHealth1GTHealth2 comp) -- SGI insertion-sort step for CDegradableDef's degradation stages, sorted by descending HealthPercentage
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067E550, length 56 bytes (+ int3 padding), `ret 20h` (/Gr algorithm TU:
// `last` in ecx, the 28-byte value and the empty comparator on the stack).
// Callers: 0x0067EC5A and 0x0067EEDE, inside _Insertion_sort1<CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2,
// CDegradableInfo> @ 0x0067EE30 (catalog name) and its __linear_insert neighbour -- which names the element and the
// comparator.
// Shape: the 0x004CFD40 skeleton with the greater-than comparator: `fld value.HealthPercentage ; fcomp [next] ;
// test ah,41h ; jne` (value > next); the implicit CDegradableInfo assignment is a 7-dword `rep movsd`.
// Layout (Ego_r.pdb CDegradableInfo, 28 bytes): HealthPercentage +0, EngineGraphic (CEngineGraphic, 16 bytes) +4,
// SmashParticleEmitter +0x14, BlocksNavigation +0x18; CEngineGraphic is modelled by its size.
// Label correction: the catalog's `_Copy_backward<CEngineCamera::CDebugScriptElement*>` is propagated.
struct CEngineGraphic {                           // 16 bytes
    long m_words[4];
};
struct CDegradableInfo {                          // 28 bytes
    float          HealthPercentage;              // +0x00
    CEngineGraphic EngineGraphic;                 // +0x04
    long           SmashParticleEmitter;          // +0x14
    bool           BlocksNavigation;              // +0x18
};
struct CDegradableDef {
    struct CIsHealth1GTHealth2 {
        bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const { return a.HealthPercentage > b.HealthPercentage; }
    };
};

void __fastcall __unguarded_linear_insert(CDegradableInfo* last, CDegradableInfo value, CDegradableDef::CIsHealth1GTHealth2 comp)
{
    CDegradableInfo* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}