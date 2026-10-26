// NUISystem::CTextContainer::AddChild(CCountedPointer<CComponent>& child) -- accept only text children: hand each the container's text window, then add it as a normal child
// Leaf: ?AddChild@CTextContainer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546C90, 53 bytes (64 with padding), virtual, `ret 4`.
// Vtable 0x01248064 (NUISystem::CTextContainer) slot 59 = AddChild (Ego_r.pdb
// `virtual void CComponent::AddChild(CCountedPointer<NUISystem::CComponent>&)`, overridden here).
// Callees: the child's GetType (slot 65, +0x104; 6 = the text component type), CText::SetTextWindow (slot 147,
//   +0x24c, landed row 0x0054FFB0 of this chunk) with the container's Window, and the base CComponent::AddChild
//   0x00533B50 (slot 59 of the CSprite vtable; a qualified, non-virtual call). Non-text children are ignored.
// Shape: the child pointer is re-read from the counted pointer for each call (`mov ecx,[edi]`); the text cast is a
//   plain static_cast (single inheritance, no adjustment).
// Layout: retail CTextContainer Window at +0x15c (Ego_r.pdb +0x18c), as the landed GetDataFromDef 0x00546CD0.
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
};

namespace NUISystem {
enum EType { TYPE_TEXT = 6 };
class CComponent;

template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
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
    virtual void AddChild(CCountedPointer<CComponent>& child);   // slot 59 (+0xec)
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual EType GetType() const;                               // slot 65 (+0x104)
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
    virtual void v133();
    virtual void v134();
    virtual void v135();
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual void v142();
    virtual void v143();
    virtual void v144();
    virtual void v145();
    virtual void v146();
};

class CText : public CComponent {
public:
    virtual void SetTextWindow(C2DBoxF& window);                 // slot 147 (+0x24c)
};

class CTextContainer : public CComponent {
public:
    virtual void AddChild(CCountedPointer<CComponent>& child);
    unsigned char _pad_0x04[0x15c - 0x04];
    C2DBoxF Window;                                              // +0x15c
};

void CTextContainer::AddChild(CCountedPointer<CComponent>& child)
{
    if (child->GetType() == TYPE_TEXT) {
        static_cast<CText*>(child.Data)->SetTextWindow(Window);
        CComponent::AddChild(child);
    }
}
}