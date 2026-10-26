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
// Leaf: ??1CParticleDataBank
void __cdecl FreeStorage(void* p);                             // 0x00BFEA14

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
            FreeStorage(_M_start);
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