// NUISystem::CRedefinerList::OnActivate() -- on first activation set the active flag, refresh the key list and hide the attached component at its current position, then run the scrollable list's own activation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005570F0, length 72 bytes. Vtable slot 140 of NUISystem::CRedefinerList
// (0x0124A994). Callees: Refresh @ 0x00557000, the component's virtual slot 16 (+0x40, returns a C2DVector through the
// hidden pointer), CScrollableList::HideComponent(CComponent*, float, float, C2DVector, unsigned char) @ 0x00555440,
// CScrollableList::OnActivate @ 0x00556200 (tail jump).
// Layout: descriptive (no engine header): the active flag +0x17c (as OnDeactivate @ 0x00556580), the attached component
// pointer +0x18c.
// Shape: the by-value C2DVector argument is the virtual call's result built straight into its argument slot (`push 60h ;
// sub esp,8 ; push esp ; call [eax+40h]`): C2DVector has a user constructor (so it returns through the hidden pointer)
// AND a user copy constructor (without it the result lands in a frame temporary and is copied with two moves), the
// two 0.0f arguments are immediates, the component is re-read for the HideComponent call, and the base OnActivate is the
// shared tail of both paths (`jmp`).
// Leaf: OnActivate@CRedefinerList
namespace NUISystem {

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
};

class CComponent { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual C2DVector GetPosition() const;              // slot 16 (+0x40)
};

struct CListRoot_005570f0 {                             // primary base (vptr at +0)
    virtual void v000();
};
struct CGuiControl_005570f0 {                           // the control base (vptr at +4)
    virtual void v000();
};

class CScrollableList : public CListRoot_005570f0, public CGuiControl_005570f0 { public:
    unsigned char _body[0x174];                         // +0x008 .. +0x17b
    void HideComponent(CComponent* component, float startTime, float endTime, C2DVector position, unsigned char alpha);   // 0x00555440
    void OnActivate();                                  // 0x00556200
};

class CRedefinerList : public CScrollableList { public:
    bool          m_bActive;                            // +0x17c
    bool          m_bSelecting;                         // +0x17d
    float         m_fadeTime;                           // +0x180
    unsigned char _pad184[0x08];                        // +0x184
    CComponent*   m_pComponent;                         // +0x18c
    void Refresh();                                     // 0x00557000
    void OnActivate();
};

void CRedefinerList::OnActivate()
{
    if (!m_bActive) {
        m_bActive = true;
        Refresh();
        if (m_pComponent)
            HideComponent(m_pComponent, 0.0f, 0.0f, m_pComponent->GetPosition(), 0x60);
    }
    CScrollableList::OnActivate();
}

}