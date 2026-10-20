// CThingCreatureBase::GetPrefferedCombatDistance() -- the combat type def's PreferredCombatDistance, or when that is unset a fifth of the way from the front-middle limit to the front-far limit
// Leaf: ?GetPrefferedCombatDistance@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00667C00, 298 bytes (+6 int3), `ret`, float in st(0).
// Callees: CThingCreatureBase::GetPCombatTypeDef() const @ 0x00664D40 (CDefPointer by value, hidden result pointer; five
//   calls), the def's virtual slot 1 (+4) at a zero count. Globals: 1e-4f 0x0129BA3C, 0.2f 0x012305A0.
// Layout: CCombatTypeDef (PDB 0xC0; retail head 0x3C and three 12-byte STLport vectors, so 0x10 below the PDB from
//   PreferOuterRing on): FrontFarLimit +0x6C, FrontMiddleLimit +0x70, PreferredCombatDistance +0x80.
// Shape: every access goes through an inline raw-pointer helper `GetCombatTypeDef() { return GetPCombatTypeDef(); }`: the
//   by-value temporary is converted (`mov edi,[eax]`), destroyed inside the helper, and the field is read afterwards; the
//   destructor's release is the ref-count base's two inline accessors (`add eax,4 ; dec ; cmp ; mov [eax]`) and it clears
//   the pointer (`mov [slot],ebx`, dropped where the slot is reused). The unset branch reads the near limit into an
//   unused local first (call and release stay, the load goes), then the middle limit into a named local (homed in the
//   temporary's dead slot) and the far limit inside the return expression.
struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()                // inlined into ~CDefPointer
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CCombatTypeDef : CDefBase {
    unsigned char _pad08[0x6c - 0x08];
    float FrontFarLimit;                   // +0x6c
    float FrontMiddleLimit;                // +0x70
    float FrontNearLimit;                  // +0x74
    float SideLimit;                       // +0x78
    float RearLimit;                       // +0x7c
    float PreferredCombatDistance;         // +0x80
};
template<class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
    T* operator->() const { return m_p; }
    operator T*() const { return m_p; }
};
struct CThingCreatureBase {
    CDefPointer<CCombatTypeDef> GetPCombatTypeDef() const;     // 0x00664D40
    const CCombatTypeDef* GetCombatTypeDef() const { return GetPCombatTypeDef(); }
    float GetPrefferedCombatDistance();
};

float CThingCreatureBase::GetPrefferedCombatDistance()
{
    if (GetCombatTypeDef()->PreferredCombatDistance < 1e-4f) {
        float nearLimit = GetCombatTypeDef()->FrontNearLimit;
        float middleLimit = GetCombatTypeDef()->FrontMiddleLimit;
        return (GetCombatTypeDef()->FrontFarLimit - middleLimit) * 0.2f + middleLimit;
    }
    return GetCombatTypeDef()->PreferredCombatDistance;
}
