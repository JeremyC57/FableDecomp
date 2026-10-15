// CMessageEventFilter_IsMyDeadLeader::operator()(CMessageEvent*) const -- a death message (type 9) about a thing whose definition is the leader definition of my creature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF3F0, 160 bytes (padding included).
// Grade: DIFFER(160v160) -- (1) retail stores the second GetPItem() result into the dead argument slot followed by a never-taken `xor al,al ; jne` (a compiled-out inline check; a volatile by-value parameter does not reproduce it); (2) retail zeroes the CDefPointer slot AFTER pushing &def and the index (ours zeroes first); (3) the def release keeps the pointer in ecx (ours eax, or an out-of-line ~CDefPointer without __forceinline). Tried: plain flow, volatile-param helper, __forceinline release + member GetDefinitionManager
// Shape: type test (+8 == 9); the event's thing pointer (CIntelligentPointer at +0x50) is fetched with
// GetPItem (0x00A01B50) for a null test, again into the dead argument slot for a compiled-out check
// (the `xor al,al ; jne` that never jumps), and a third time as the dead thing; my creature's
// definition (word index +0x8c) is looked up through the definition manager (0x004C7990, fetched
// after the argument pushes) with GetDef (0x00513160) into a CDefPointer that reuses the argument slot;
// on success and a non-null def, the def's leader index (+0x13c) is compared with the dead thing's
// def index (+0x8c). The def pointer's inline release (count +4, vtable slot 1 at zero) runs on each
// exit that holds one.
struct CCreatureDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long          m_refCount;                      // +0x04
    unsigned char _pad08[0x13c - 8];
    unsigned long m_leaderDefIndex;                // +0x13c
};

template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p) {
            if (--m_p->m_refCount == 0)
                m_p->Destroy();
        }
    }
    T* operator->() const { return m_p; }
};

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CCreatureDef>* def);   // 0x00513160
};
CDefinitionManager* GetDefinitionManager();        // 0x004C7990

struct CThing {
    unsigned char  _pad[0x8c];
    unsigned short m_defIndex;                     // +0x8c
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                 // +4
    CThing* GetPItem() const;                      // 0x00A01B50
};

inline bool IsBeingDestroyed(CThing* volatile thing) { return false; }   // compiled-out check

struct CMessageEvent {
    unsigned char _pad00[8];
    long          Type;                            // +0x08
    unsigned char _pad0c[0x50 - 0xc];
    CBaseIntelligentPointer PThing;                // +0x50
};

struct CThingCreatureBase : CThing { };

struct CMessageEventFilter_IsMyDeadLeader {
    CThingCreatureBase* Creature;                  // +0
    bool operator()(CMessageEvent* event) const;
};

bool CMessageEventFilter_IsMyDeadLeader::operator()(CMessageEvent* event) const
{
    if (event->Type == 9) {
        CBaseIntelligentPointer& p = event->PThing;
        if (p.GetPItem() != 0 && !IsBeingDestroyed(p.GetPItem())) {
            CThing* dead = p.GetPItem();
            if (dead) {
                CDefPointer<CCreatureDef> def;
                if (GetDefinitionManager()->GetDef(Creature->m_defIndex, &def)) {
                    if (def.m_p && def->m_leaderDefIndex == dead->m_defIndex)
                        return true;
                }
            }
        }
    }
    return false;
}
