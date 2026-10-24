// NUISystem::CComponent::CanScroll(bool horizontal, C2DBoxF& box) -- the child scroll amount of largest magnitude (0 without children)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00530690, 130 bytes (144 with padding).
// Callees: vtable slot 88 (+0x160) = CanScroll on each child (the recursion through the child's own override).
// Vtable 0x0122F5D4 (NUISystem::CSprite, inherited from CComponent) slot 88; the Ego_r PDB declares
// `virtual float CComponent::CanScroll(bool, C2DBoxF&)` (?CanScroll@CComponent@NUISystem@@UAEM_NAAVC2DBoxF@@@Z).
// Children is the retail std::vector<CCountedPointer<CComponent>> at +0xb0 (Ego_r +0xb8; the retail States map
// is 8 bytes smaller), 8-byte elements {object, count block}; the element is reached through the retail iterator
// OBJECT (`*(begin() + i)`, as the landed HasChild 0x005309E0) so first/last are re-read after every virtual call.
// The result is a frame float zeroed with an immediate store; `(float)fabs(v) > (float)fabs(result)` keeps the new
// value (`fcompp ; test ah,5 ; jp` skips the store): |v| is computed first and the two float narrowings leave the
// `fxch st(1) ; fxch st(1)` pair (the double compare has none). size() is unsigned (`jbe`/`jb`).
// Leaf: CanScroll
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)

template<class T> struct CRetailVector {                        // retail std::vector: {first, last, end} with an iterator OBJECT
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) { }
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct C2DBoxF;
struct CComponent;

struct CCountedComponentPointer {                               // CCountedPointer<NUISystem::CComponent>
    CComponent* m_pObject;                                      // +0
    void*       m_pCount;                                       // +4
    CComponent* operator->() const { return m_pObject; }
};

struct CComponent {                                             // retail NUISystem component view
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
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual float CanScroll(bool horizontal, C2DBoxF& box);     // slot 88 (+0x160)
    unsigned char _body[0xb0 - 0x04];                           // +0x04..+0xaf
    CRetailVector<CCountedComponentPointer> Children;           // +0xb0
};

float CComponent::CanScroll(bool horizontal, C2DBoxF& box)
{
    float result = 0.0f;
    for (unsigned long i = 0; i < Children.size(); ++i) {
        float amount = Children[i]->CanScroll(horizontal, box);
        if ((float)fabs(amount) > (float)fabs(result))
            result = amount;
    }
    return result;
}