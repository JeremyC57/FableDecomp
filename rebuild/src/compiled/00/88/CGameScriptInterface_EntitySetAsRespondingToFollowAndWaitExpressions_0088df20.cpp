// CGameScriptInterface::EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing& thing, bool flag) const -- marks the creature as responding to follow-and-wait expressions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088DF20, length 64 bytes.
// Callees: CScriptThing virtual slot 75 (+0x12c, IsValid) and slot 11 (+0x2c, GetThing); then the thing's byte at +0x10 must be 1
// (a creature) and bit 2 of the flag byte at +0x1c9 is assigned (`shl cl,2 ; xor ; and 4 ; xor`).
// Oracle name: ?EntitySetAsRespondingToFollowAndWaitExpressions@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z (`UBE` = virtual const; `this` is unused, so the
// method is written on the PDB-header wrapper and selected by its leaf).
// Leaf: EntitySetAsRespondingToFollowAndWaitExpressions
#include "engine/CGameScriptInterface.h"

struct CThing {
    unsigned char _pad[0x10];
    unsigned char m_thingKind;                      // +0x10 (1 = creature)
    unsigned char _pad11[0x1b8];
    unsigned char m_followBit0 : 1;                 // +0x1c9 allowed to follow hero
    unsigned char m_followBit1 : 1;                 //        NOT allowed to change region following state
    unsigned char m_followBit2 : 1;                 //        responds to follow-and-wait expressions
};

struct CScriptThing {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual CThing* GetThing() const;                // slot 11 (+0x2c)
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual bool IsValid() const;                   // slot 75 (+0x12c)
    void* m_pImpData;                               // +4 CCountedPointer<CScriptThing>::Data
    void* m_pImpInfo;                               // +8
};

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing& thing, bool flag) const;
};

void CGameScriptInterface_Methods::EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing& thing, bool flag) const
{
    if (thing.IsValid()) {
        CThing* object = thing.GetThing();
        if (object->m_thingKind == 1)
            object->m_followBit2 = flag;
    }
}