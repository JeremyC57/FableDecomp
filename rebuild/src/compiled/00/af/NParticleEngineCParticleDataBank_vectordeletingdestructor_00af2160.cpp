// NParticleEngine::CParticleDataBank::`vector deleting destructor'(unsigned int) `adjustor{356}' -- CResourceBank-subobject vtable entry: this -= 0x164, jump to the deleting destructor 0x00AF21E0
// Leaf: ??_ECParticleDataBank@NParticleEngine@@WBGE@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2160, 6+5 bytes: `sub ecx,0x164 ; jmp 0x00AF21E0`. Compiler-generated adjustor thunk.
// Identity (two sources): (1) slot 0 of the CResourceBank (+0x164) vtable 0x0129F380 (RTTI complete-object locator offset 0x164, class
//   .?AVCParticleDataBank@NParticleEngine@@; the catalog's "vfunc_0"); (2) its jump target 0x00AF21E0 is slot 0 of the primary vtable 0x0129F38C, the class's
//   ??_G. VC7.1 names the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the adjustor in the
//   thunk name (??_ECParticleDataBank@NParticleEngine@@WBGE@AEPAXI@Z); objdump -t of this object shows the leaf.
// TU: the thunk is emitted with the vtable in any TU that defines the class's constructor or destructor (chunk 765 shape). This
//   source reuses the landed definer NParticleEngine::CParticleDataBank::~CParticleDataBank 0x00AF1600 (landed destructor, verbatim) as that TU;
//   only the thunk is graded here. Landed definer: rebuild/src/compiled/00/af/NParticleEngineCParticleDataBank_CParticleDataBank_00af1600.cpp
// Callees: ??_GCParticleDataBank@NParticleEngine@@UAEPAXI@Z 0x00AF21E0 (bound as its ??_E alias).
// ---- definer TU (verbatim from the landed row, minus its Leaf/Flags lines) ----
// NParticleEngine::CParticleDataBank::~CParticleDataBank() -- destroys the mesh and graphic symbol maps and the particle-emitter array, then the secondary base and the CBankFile base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF1600, length 104 bytes (+ int3 padding).
// Identity: the dtor stores vtable 0x0129F38C (NParticleEngine::CParticleDataBank, rtti-vtables.tsv) at +0 and the secondary base's
// vtable 0x0129F380 at +0x164; the catalog label "CTCInventoryStats::~CTCInventoryStats" is propagated.
// Callees: CSymbolMap::~CSymbolMap @ 0x006FD040 (manifest "~CPatchTesselationEdgeStrip", propagated) twice, the LTCG `__fastcall`
// element-range destroy @ 0x00AF1D90 (ecx = first, edx = last, the type-tag temporary by reference), the array storage freed by
// 0x00BFEA14, the secondary base's dtor @ 0x009FC690 and CBankFile's dtor @ 0x009D6490.
// Shape: members destroy in reverse order (MeshSymbols +0x210, GraphicSymbols +0x1EC, ParticleEmitters +0x1E0) before the bases
// (secondary at +0x164, then CBankFile); the emitter array's inline dtor passes an empty tag temporary (`lea eax,[esp+0Bh]`).
// Layout: Ego_r.pdb CParticleDataBank ParticleEmitters / GraphicSymbols / MeshSymbols (+596/+612/+656) sit at retail
// +0x1E0/+0x1EC/+0x210 (retail CBankFile 0x164 bytes, 12-byte array, 36-byte symbol maps).
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct __false_type { __false_type() {} };
namespace NParticleEngine {
class CParticleEmitterInfo;
}
void __fastcall DestroyRange(NParticleEngine::CParticleEmitterInfo* first, NParticleEngine::CParticleEmitterInfo* last,
                             const __false_type& tag);         // 0x00AF1D90
template<class T> class CArray { public:                              // retail 12 bytes: {first, last, end}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CArray()
    {
        DestroyRange(_M_start, _M_finish, __false_type());
        if (_M_start)
            free(_M_start);
    }
};
class CSymbolMap { public:                                            // retail 36 bytes
    unsigned char _body[0x24];
    ~CSymbolMap();                                             // 0x006FD040
};
namespace N_009d6490 { struct CBankFile {
    virtual ~CBankFile();                                      // 0x009D6490
    unsigned char _pad04[0x164 - 0x04];
}; } using N_009d6490::CBankFile;
struct CBankSecondaryBase {                                    // vtable 0x0129F380 in the derived object
    virtual ~CBankSecondaryBase();                             // 0x009FC690
    unsigned char _pad04[0x7c - 0x04];
};

namespace NParticleEngine {
class CParticleDataBank : public CBankFile, public CBankSecondaryBase {
public:
    CArray<CParticleEmitterInfo> ParticleEmitters;             // +0x1e0
    CSymbolMap GraphicSymbols;                                 // +0x1ec
    CSymbolMap MeshSymbols;                                    // +0x210
    virtual ~CParticleDataBank();
};

CParticleDataBank::~CParticleDataBank()
{
}
}