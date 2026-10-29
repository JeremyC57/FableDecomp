// NUISystem::CComponent::RemoveObserverRecursive() -- unregister this component's observer interface from the front-end manager, notify itself, then recurse into every child
// Retail: Fable.exe (TLC, VC7.1) @ 0x005303F0, 101 bytes (112 with padding).
// Callees: NUISystem::CFrontEndManager::GetInstance 0x0041E5F2 (`__fastcall`, no arguments) and its
//          vtable slot 5 (`call [edx+14h]`, RemoveObserver(CObserver*)); this component's vtable slot
//          133 (`call [eax+214h]`); RemoveObserverRecursive itself on each child (direct call, non-virtual).
//
// Shape: the component's CObserver interface is a second base at +4 (Ego_r: CComponent derives from
// a primary polymorphic base and NUISystem::CObserver), so passing `this` as CObserver* is the
// null-tested `this ? this + 4 : 0` conversion, computed before the GetInstance call (argument
// evaluated first, kept in edi).
// Children is the retail 12-byte std::vector<CCountedPointer<CComponent>> at +0xb0 (Ego_r.pdb +0xb8:
// the States map before it is smaller in the shipped binary), 8-byte elements {object, count block};
// as in the landed CComponent rows (0x00530460, 0x005309E0) the element is reached through the
// retail iterator object (`*(begin() + i)`), so `first` is re-read after every virtual call and the
// size is recomputed (`(last - first) >> 3`) for each loop test; the index is unsigned (`jb`).
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
    T& operator[](unsigned long n) { return *(begin() + n); }    // STLport operator[] = *(begin() + n)
};

struct CComponent;
struct CObserver {
    virtual void Notify();
};

namespace NUISystem { class CManager { public:                                // vtable-slot view of NUISystem::CFrontEndManager
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void RemoveObserver(CObserver* observer);   // slot 5 (+0x14)
    static CManager* __fastcall GetInstance();  // 0x0041E5F2
}; }

struct CCountedComponentPointer {                               // CCountedPointer<NUISystem::CComponent>
    CComponent* m_pObject;                                      // +0
    void*       m_pCount;                                       // +4
};

struct CComponentBase {
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
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void OnObserversRemoved();                       // slot 133 (+0x214)
};

struct CComponent : CComponentBase, CObserver {                // retail NUISystem::CComponent view
    unsigned char _body[0xb0 - 0x08];                           // +0x08..+0xaf
    CRetailVector<CCountedComponentPointer> Children;           // +0xb0
    void RemoveObserverRecursive();
};

void CComponent::RemoveObserverRecursive()
{
    NUISystem::CManager::GetInstance()->RemoveObserver(this);
    OnObserversRemoved();
    for (unsigned long i = 0; i < Children.size(); ++i)
        Children[i].m_pObject->RemoveObserverRecursive();
}