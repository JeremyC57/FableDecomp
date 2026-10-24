// CConstraintRepeat_00c38470::CConstraintRepeat_00c38470(CProgram_00c38470* program, void* target, float value) -- EH-framed constructor: stores the program and target, clears the interpolated value's angle flag, builds the +0x40 slot, resets the value, then registers with the program's update registry (+8) and its owner's constraint registry (+0x40), both virtual slot 9
// Leaf: ??0CConstraintRepeat_00c38470
// Flags: /EHsc
// Label: manifest "Matrix_Rotation::Constructor" is propagated; the vtable 0x012B53B0 and the unwind map (state 0 -> base dtor 0x00C383A0, state 1 -> +0x40 slot dtor 0x00C147D0) tie it to the landed destructor 0x00C38470 (same class model). Sole caller 0x00C2C895 allocates 0x4c bytes.
struct CConstraintRepeat_00c38470;
struct CRegistry_00c38470 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CConstraintRepeat_00c38470* constraint);   // slot 8
    virtual void Add(CConstraintRepeat_00c38470* constraint);      // slot 9
};

struct CProgramOwner_00c38470 {
    unsigned char _pad[0x40];
    CRegistry_00c38470 m_constraints;      // +0x40
};

struct CProgram_00c38470 {
    unsigned char _pad[4];
    CProgramOwner_00c38470* m_pOwner;      // +4
    CRegistry_00c38470 m_updates;          // +8
};

struct CInterpolatedFloat {
    float m_current;                    // +0x00
    float m_target;                     // +0x04
    unsigned char _pad_0x08[0x28];
    bool m_isAngle;                     // +0x30
    CInterpolatedFloat() : m_isAngle(false) { }
    void Reset(float value);            // 0x00C0B510
};

struct CConstraintBase_00c38470 {          // vtable 0x012B53A0
    CConstraintBase_00c38470() { }
    virtual void v00();
    virtual void v01();
    virtual void* v02();
    virtual ~CConstraintBase_00c38470();   // 0x00C383A0
};

struct CConstraintSlot_00c38470 {          // +0x40
    CConstraintSlot_00c38470();            // 0x00C2C6B0
    ~CConstraintSlot_00c38470();           // 0x00C147D0
    virtual void v00();
};

struct CConstraintRepeat_00c38470 : CConstraintBase_00c38470 {   // vtable 0x012B53B0
    CProgram_00c38470* m_pProgram;         // +4
    void* m_pTarget;                       // +8
    CInterpolatedFloat m_value;            // +0xc
    CConstraintSlot_00c38470 m_slot;       // +0x40
    CConstraintRepeat_00c38470(CProgram_00c38470* program, void* target, float value);
    virtual void v00();
    virtual void v01();
    virtual void* v02();
    virtual ~CConstraintRepeat_00c38470();
};

CConstraintRepeat_00c38470::CConstraintRepeat_00c38470(CProgram_00c38470* program, void* target, float value)
    : m_pProgram(program), m_pTarget(target)
{
    m_value.Reset(value);
    m_pProgram->m_updates.Add(this);
    m_pProgram->m_pOwner->m_constraints.Add(this);
}