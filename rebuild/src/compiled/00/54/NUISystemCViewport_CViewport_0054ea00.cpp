// NUISystem::CViewport::CViewport(long owner) -- CChangingStateComponent(owner), the three vtables of this class, the primitive handle at +0x15c, the empty fake-child vector and the cleared word at +0x170, then InitialiseFromDef()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054EA00, length 96 bytes.
// Callees: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50 (the landed
// CParticleEffect constructor's base), CBaseClass::CBaseClass @ 0x0099A2D0 (the handle's base), the body call
// @ 0x0054E5D0 (manifest "CParticleEffect::GetDataFromDef" is propagated; the same position as the landed
// 0x00544010's GetDataFromDef -- reads the def through `this`).
// Vtables 0x01249A5C / 0x01249A38 / 0x01249A30 = NUISystem::CViewport (primary + the interface bases at +4
// and +0x18, as the landed 0x00544010 model), 0x0122F598 = CEnginePrimitiveHandle (rtti-vtables.tsv).
// Shape: after the base call the three vtable stores, then the member objects in declaration order: the
// handle (out-of-line CBase constructor on `lea edi,[esi+15Ch]`, its vtable, its pointer zeroed), the
// 12-byte vector's inline constructor (three zero stores), the plain word at +0x170, then the body call.
namespace NUISystem {

struct IComponentA {                           // the interface base at +0x04 (vtable, 0x10 bytes of state)
    virtual void va00();
    unsigned char _state[0x10];
};

struct IComponentB {                           // the interface base at +0x18 (vtable only)
    virtual void vb00();
};

struct CComponentRoot {                        // the primary base: vptr at +0
    virtual void v00();
};

class CChangingStateComponent : public CComponentRoot, public IComponentA, public IComponentB { public:
    unsigned char _body[0x15c - 0x1c];         // +0x1c .. +0x15c: the base's state
    CChangingStateComponent(long owner);       // 0x0052CC50
};

}   // the retail root is file scope
class CBaseClass { public:
    virtual ~CBaseClass();                     // slot 0 (0x0099A2E0)
    CBaseClass();                              // 0x0099A2D0 ??0CBaseClass@@QAE@XZ
};
namespace NUISystem {

struct CEnginePrimitiveHandle : CBaseClass {   // vtable 0x0122F598
    void* m_pPrimitive;                        // +4
    CEnginePrimitiveHandle() : CBaseClass(), m_pPrimitive(0) { }
};

struct CComponentVector {                      // 12-byte retail vector {first, last, end}
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CComponentVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

class CViewport : public CChangingStateComponent { public:   // vtables 0x01249A5C / 0x01249A38 / 0x01249A30
    virtual void v00();
    virtual void va00();
    virtual void vb00();
    CEnginePrimitiveHandle m_primitive;        // +0x15c
    CComponentVector       m_fakeChildren;     // +0x164
    void*                  m_pView;            // +0x170
    void InitialiseFromDef();                  // 0x0054E5D0
    CViewport(long owner);
};

CViewport::CViewport(long owner)
    : CChangingStateComponent(owner), m_primitive(), m_fakeChildren(), m_pView(0)
{
    InitialiseFromDef();
}

}