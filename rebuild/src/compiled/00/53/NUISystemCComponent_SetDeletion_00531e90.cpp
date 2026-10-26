// NUISystem::CComponent::SetDeletion(CDeletion deletion) -- replace the component's deletion record (method and associated parents) with a copy of the argument
// Leaf: ?SetDeletion@CComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00531E90, 38 bytes (48 with padding), virtual, `ret 8`.
// Vtable 0x0122F5D4 (NUISystem::CSprite, inherited from CComponent) slot 62; Ego_r.pdb declares
// `virtual void CComponent::SetDeletion(CDeletion)` -- the record is passed BY VALUE (8 bytes in retail: the
// EDeletionMethod plus a 4-byte STLport list<CComponent*>), hence `ret 8`.
// Callees: list<CComponent*>::operator= 0x00535800 (manifest "std::list::splice"; the body is the element-wise
//   assign of an STLport list), ~list<CComponent*> 0x0042ABCA (the by-value parameter is destroyed by the callee).
// Shape: the implicit CDeletion::operator= copies Method as a dword then calls the list's operator=; the by-value
//   parameter's implicit destructor then runs on its own argument slot (`lea ecx,[esp+8]`).
// Layout: retail CComponent Deletion at +0xd4 (Ego_r.pdb +0xe4; the retail list is 4 bytes where the PDB has 12).
namespace NUISystem {
class CComponent;

enum EDeletionMethod { DELETION_METHOD_DEFAULT };

struct CComponentList {                                       // std::list<CComponent*> (STLport, 4 bytes: the head node)
    void* _M_node;
    CComponentList& operator=(const CComponentList& other);   // 0x00535800
    ~CComponentList();                                        // 0x0042ABCA
};

class CDeletion { public:
    EDeletionMethod Method;                                   // +0x00
    CComponentList  AssociatedParents;                        // +0x04
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
    virtual void v061();
    virtual void SetDeletion(CDeletion deletion);             // slot 62 (+0xf8)
    unsigned char _pad_0x04[0xd4 - 0x04];
    CDeletion Deletion;                                       // +0xd4
};

void CComponent::SetDeletion(CDeletion deletion)
{
    Deletion = deletion;
}
}