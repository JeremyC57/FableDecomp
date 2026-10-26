// NPlayerGui::CDrawMeleeActionButtonEffects::~CDrawMeleeActionButtonEffects() -- compiler-generated: release the melee ability def, destroy the particle-emitter vector, then the base's primitive-handle vector
// Leaf: ??1CDrawMeleeActionButtonEffects
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004420D0, length 115 bytes, plain `ret`. Callers: the CCountedPointer
// per-type deleter 0x00443AA0 (`if (p) { ~T; operator delete }`; manifest "CTCMapwho::OnDie", propagated), stored by
// CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects>::Reset 0x00444B40, which CPlayerGui's setup calls at 0x0043C25A
// on `new` (0x48 bytes) + NPlayerGui::CDrawMeleeActionButtonEffects::CDrawMeleeActionButtonEffects 0x00654258 (landed) into
// CPlayerGui::PDrawMeleeActionButtonEffects (Ego_r.pdb +0x228, retail +0x220); also the unreferenced ??_G 0x00443010.
// The manifest label (_Dest_val<allocator<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase>>>>) is propagated and wrong.
// Layout (Ego_r.pdb, retail 4/8 lower from the vector on as the landed ctor notes): CDrawBase {vptr; PlayerGui +4; PlayerManager
// +8; World +0xc; Component +0x10; Bank +0x14; Drawing +0x18; vector<CEnginePrimitiveHandle> PrimitiveHandles +0x1c}, then
// Effects (vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >) +0x28, the four timers/counters +0x34..+0x40 and
// PMeleeAbilityDef (CDefPointer<const CMeleeCombatAbilityDef>) +0x44.
// Callees: CDefPointeeBase slot 1 (DeleteThis) through the def, each element's virtual destructor through its vtable
// (`push 0 ; call [vtbl]`: CEnginePrimitiveHandle and CIntelligentPointer both have a virtual dtor at slot 0), free @ 0x00BFEA14.
// Shape: no vtable stores -- CDrawBase's vtable (0x0125A52C) holds only ClearPrimitivesIfNecessary and PrepareForLoad, so neither
// class declares a destructor; the vectors' inline destructors run STLport's `_Destroy(first, last)` inline
// (the bounds held in registers across the virtual calls), then `if (start) free`.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

class CPlayerGui;
class CPlayerManager;
class CWorld;
class CMainGameComponent;
class CGraphicDataBank;
class CMeleeCombatAbilityDef;

class CDefPointeeBase { public:
    virtual ~CDefPointeeBase();
    virtual void DeleteThis();                               // slot 1 (+4)
    unsigned long RefCount;                                  // +0x04
};

template<class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->DeleteThis();
            Object = 0;
        }
    }
};

class CEnginePersistentPrimitiveBase;
template<class T> class CIVCountedPointer { public:
    T* Object;
};

class CEnginePrimitiveHandle { public:                              // 8 bytes, polymorphic
    virtual ~CEnginePrimitiveHandle();
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;  // +0x04
};

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10 (slot 0 = ??_G)
    void* PItem;                                             // +0x04
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };

namespace NParticleEngine { class CParticleEmitter; }

namespace std {
template<class T> inline void _Destroy(T* first, T* last)    // STLport: explicit destructor call per element
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> struct vector {                            // STLport: three words
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
}

namespace NPlayerGui {

class CDrawBase { public:                                           // vtable 0x0125A52C
    virtual void ClearPrimitivesIfNecessary();               // slot 0 (0x00644627)
    virtual void PrepareForLoad();                           // slot 1 (0x00442760)
    CPlayerGui*                              PlayerGui;      // +0x04
    CPlayerManager*                          PlayerManager;  // +0x08
    CWorld*                                  World;          // +0x0c
    CMainGameComponent*                      Component;      // +0x10
    CGraphicDataBank*                        Bank;           // +0x14
    bool                                     Drawing;        // +0x18
    std::vector<CEnginePrimitiveHandle>      PrimitiveHandles;   // +0x1c
};

class CDrawMeleeActionButtonEffects : public CDrawBase { public:           // vtable 0x0125A714
    virtual void ClearPrimitivesIfNecessary();               // slot 0 (0x00645829)
    std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;   // +0x28
    long                                     FlashTimer;     // +0x34
    long                                     ComboLoadingIconTimer;   // +0x38
    long                                     FlourishCounter;         // +0x3c
    long                                     FlourishEffect;          // +0x40
    CDefPointer<const CMeleeCombatAbilityDef> PMeleeAbilityDef;      // +0x44
};

}

void DestroyMeleeActionButtonEffects(NPlayerGui::CDrawMeleeActionButtonEffects* draw)   // emits the implicit destructor; not part of the retail body
{
    draw->NPlayerGui::CDrawMeleeActionButtonEffects::~CDrawMeleeActionButtonEffects();
}