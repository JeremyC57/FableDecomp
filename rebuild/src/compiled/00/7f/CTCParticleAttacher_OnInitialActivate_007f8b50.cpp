// CTCParticleAttacher::OnInitialActivate() -- attach the def's particles when none are attached yet
// Leaf: ?OnInitialActivate@CTCParticleAttacher@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8B50, 14 bytes, tail jump.
// Identity: CTCParticleAttacher vtable 0x0124176C slot 6 (RTTI .?AVCTCParticleAttacher@@); FableWin.pdb
//   `?OnInitialActivate@CTCParticleAttacher@@UAEXXZ` is in the class's name set.
// CTCBase virtual slots (Ego_r.pdb donor order, matched to the CTCBase vtable 0x0125BE0C): 2 OnRemove, 3 ConstructFromParams,
//   4 OnCreate, 5 InitialActivate, 6 OnInitialActivate, 7 RenderUpdate, 8 HasFrameUpdate, 9 FrameUpdate,
//   10 BulletTimeFrameUpdate, 11 DrawPreparePrimitivesForRendering, 12 Copy, 13 OnDie, 14 OnKill (the editor-only
//   EditFrameUpdate / EditorDrawPreparePrimitivesForRendering of the debug build are absent from retail).
// Callees: CTCParticleAttacher::AttachParticles() @ 0x007F8890 (landed).
// Layout: as the landed AttachParticles: PDef +0x0c, ParticleList +0x10 (STLport vector<CIntelligentPointer<CThing> >,
//   12 bytes: _M_start +0x10, _M_finish +0x14).
// Shape: `ParticleList.empty()` = `_M_start == _M_finish` (`mov eax,[ecx+10h] ; cmp eax,[ecx+14h] ; jne`), then the sibling
//   tail jump.
class CThing;
template <class T> class CIntelligentPointer;
class CParticleAttacherDef;

namespace std {
template <class T> class allocator { public: };
template <class T, class Alloc = allocator<T> > class vector {      // STLport 4.6 layout (_Vector_base members)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
};
}

class CTCParticleAttacher {
public:
    virtual void OnInitialActivate();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    const CParticleAttacherDef* PDef;           // +0x0c CDefPointer<const CParticleAttacherDef>
    std::vector<CIntelligentPointer<CThing> > ParticleList;   // +0x10
    void AttachParticles();                     // 0x007F8890
};

void CTCParticleAttacher::OnInitialActivate()
{
    if (ParticleList.empty())
        AttachParticles();
}