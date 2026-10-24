// NUISystem::CComponent::AddChildFront(CCountedPointer<CComponent>& child) -- insert a child at the front of the children vector
// Leaf: ?AddChildFront@CComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00533BA0, 26 bytes (32 with padding), virtual, `ret 4`.
// Vtable 0x0122F5D4 (NUISystem::CSprite, inherited from CComponent) slot 61; Ego_r.pdb declares
// `virtual void CComponent::AddChildFront(CCountedPointer<NUISystem::CComponent>&)`.
// Callees: STLport vector<CCountedPointer<CComponent>>::insert(iterator, const T&) 0x00535B10 (the iterator is a
//   plain element pointer, so `begin()` is the first word pushed by value; the returned iterator is dropped).
// Shape: a one-call forwarder that is not tail-called (the member address `add ecx,0B0h` is formed after the
//   begin load, the argument pushed first).
// Layout: retail CComponent Children (STLport vector, 12 bytes) at +0xb0 (Ego_r.pdb +0xb8).
namespace NUISystem {
class CComponent;

template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
};

template <class T> struct CRetailVector {                     // STLport vector: {start, finish, end of storage}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* insert(T* position, const T& x);                       // 0x00535B10
};

class CComponent {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void AddChildFront(CCountedPointer<CComponent>& child);   // slot 61 (+0xf4)
    unsigned char _pad_0x04[0xb0 - 0x04];
    CRetailVector<CCountedPointer<CComponent> > Children;             // +0xb0
};

void CComponent::AddChildFront(CCountedPointer<CComponent>& child)
{
    Children.insert(Children.begin(), child);
}
}