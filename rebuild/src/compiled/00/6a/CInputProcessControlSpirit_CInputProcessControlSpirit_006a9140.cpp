// CInputProcessControlSpirit::CInputProcessControlSpirit(const CInputProcessGameBaseInit& init) -- the game-base constructor, this vtable, then the PCreature intelligent pointer (+0x28) default-constructed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A9140, length 43 bytes (+ int3 padding). Vtable 0x0125D110 (CInputProcessControlSpirit).
// Callees: CInputProcessGameBase::CInputProcessGameBase(const CInputProcessGameBaseInit&) @ 0x00687A30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10
// (then the CIntelligentPointer vtable 0x01237F14 stored by the inlined derived constructor).
// Layout: engine/CInputProcessControlSpirit.h (PCreature +0x28, 0x30 bytes); the base is modelled locally for its constructor.
// Shape: the member's address is taken into edi before the vtable store, the base pointer constructor is called out of line and
// the derived vtable stored through edi; `this` returns in eax, `ret 4`.
// Leaf: ??0CInputProcessControlSpirit
class CInputProcessGameBaseInit;
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
class CThingPlayerCreature;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F14
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CInputProcessGameBase {                      // vtable 0x0125BE98, 0x28 bytes
    virtual void v00();
    unsigned char _pad04[0x10];                     // +0x04 CAInputProcess
    void*         World;                            // +0x14
    void*         MyPlayer;                         // +0x18
    void*         DefinitionManager;                // +0x1c
    void*         Component;                        // +0x20
    void*         PGamePlayerInterface;             // +0x24
    CInputProcessGameBase(const CInputProcessGameBaseInit& init);   // 0x00687A30
};
class CInputProcessControlSpirit : public CInputProcessGameBase { public:   // vtable 0x0125D110
    virtual void v00();
    CIntelligentPointer<const CThingPlayerCreature> PCreature;                  // +0x28
    CInputProcessControlSpirit(const CInputProcessGameBaseInit& init);
};

CInputProcessControlSpirit::CInputProcessControlSpirit(const CInputProcessGameBaseInit& init)
    : CInputProcessGameBase(init), PCreature()
{
}