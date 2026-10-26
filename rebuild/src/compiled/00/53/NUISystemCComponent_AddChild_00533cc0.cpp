// NUISystem::CComponent::AddChild(CCountedPointer<CComponent>& child) -- append the child's counted pointer to Children, make this component its parent, then initialise it
// Leaf: ?AddChild@CComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00533CC0, 98 bytes (112 with padding), `ret 4` (thiscall; Ego_r.pdb
//   `?AddChild@CComponent@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z`, CComponent vtable 0x0124608C
//   slot 60, rtti-vtables.tsv).
// Callees: vector<CCountedPointer<CComponent> >::_M_insert_overflow(pos, x, const __false_type&, 1, true) 0x005359D0
//   (as declared by the landed CSpellContainer::Initialise 0x00545E60), the child's virtual SetParent (slot 51, +0xcc;
//   Ego_r.pdb ?SetParent@CComponent@NUISystem@@UAEXPAV12@@Z) and Initialise (slot 43, +0xac; CComponent's own is the
//   landed 0x005339B0).
// Callers: the vtable entry (0x0124617C) and direct calls 0x0052CAB9, 0x00556DA4, 0x00556EB6.
// Label correction: catalogued as "Vector_PushBack_Or_Reallocate"; it is CComponent::AddChild.
// Layout: Children (STLport vector of 8-byte CCountedPointer<CComponent> { Data, Info }) at retail +0xb0 (landed
//   CSpellContainer::Initialise). Local polymorphic view.
// Shape: STLport push_back inlined (room left: the null-tested placement copy, whose copy constructor bumps the count;
//   full: _M_insert_overflow with the __false_type temporary homed in the dead `child` argument slot), then the two
//   virtual calls through child->Data.
inline void* operator new(unsigned int, void* where) { return where; }

class CCPPointerInfo { public:
    long Count;                                 // +0x00
};
template <class T> class CCountedPointer {      // 8 bytes: the object, then its shared count record
public:
    T*              Data;                       // +0x00
    CCPPointerInfo* Info;                       // +0x04
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info)
    {
        if (Info)
            ++Info->Count;
    }
    T* operator->() const { return Data; }
};
struct __false_type {
    __false_type() {}
};
template <class T> class CCountedVector {       // STLport vector<CCountedPointer<T> > (12 bytes)
public:
    CCountedPointer<T>* _M_start;
    CCountedPointer<T>* _M_finish;
    CCountedPointer<T>* _M_end_of_storage;
    void _M_insert_overflow(CCountedPointer<T>* position, const CCountedPointer<T>& x, const __false_type&,
                            unsigned long fillLength, bool atEnd);   // 0x005359D0
    void push_back(const CCountedPointer<T>& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCountedPointer<T>(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

namespace NUISystem {
class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042();
    virtual void Initialise();                                          // slot 43 (+0xac)
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050();
    virtual void SetParent(CComponent* parent);                         // slot 51 (+0xcc)
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059();
    virtual void AddChild(CCountedPointer<CComponent>& child);          // slot 60 (+0xf0)
    unsigned char _pad04[0xb0 - 4];
    CCountedVector<CComponent> Children;                                // +0xb0
};

void CComponent::AddChild(CCountedPointer<CComponent>& child)
{
    Children.push_back(child);
    child->SetParent(this);
    child->Initialise();
}
}