// NUISystem::CComponent::Die() -- switches the component into its dead state (2), then detaches it from its observers
// Leaf: Die@CComponent
// Retail: Fable.exe @ 0x00530720, 21 bytes; vtable 0x0124608C slot 81 (RTTI CComponent@NUISystem, inherited by CSprite etc.).
// Calls its own slot 48 (+0xc0, 0x0052E710) with 2 -- Ego_r.pdb ChangeState(unsigned long) is the one (K) overload -- then
// tail-jumps to the protected non-virtual RemoveObserverRecursive() @ 0x005303F0.
namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent();                               // slot 0
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void _v06();
    virtual void _v07();
    virtual void _v08();
    virtual void _v09();
    virtual void _v10();
    virtual void _v11();
    virtual void _v12();
    virtual void _v13();
    virtual void _v14();
    virtual void _v15();
    virtual void _v16();
    virtual void _v17();
    virtual void _v18();
    virtual void _v19();
    virtual void _v20();
    virtual void _v21();
    virtual void _v22();
    virtual void _v23();
    virtual void _v24();
    virtual void _v25();
    virtual void _v26();
    virtual void _v27();
    virtual void _v28();
    virtual void _v29();
    virtual void _v30();
    virtual void _v31();
    virtual void _v32();
    virtual void _v33();
    virtual void _v34();
    virtual void _v35();
    virtual void _v36();
    virtual void _v37();
    virtual void _v38();
    virtual void _v39();
    virtual void _v40();
    virtual void _v41();
    virtual void _v42();
    virtual void _v43();
    virtual void _v44();
    virtual void _v45();
    virtual void _v46();
    virtual void _v47();
    virtual void ChangeState(unsigned long state);       // slot 48 (+0xc0)
    virtual void Die();
protected:
    void RemoveObserverRecursive();                      // 0x005303F0
};

void CComponent::Die()
{
    ChangeState(2);
    RemoveObserverRecursive();
}
}