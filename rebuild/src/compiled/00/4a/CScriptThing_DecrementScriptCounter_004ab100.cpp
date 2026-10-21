// CScriptThing::DecrementScriptCounter() -- script-handle proxy: forwards to the same virtual on the implementation object (PImp) when there is one
// Retail: Fable.exe @ 0x004AB100, vtable 0x01238C8C slot 73 (+0x124); a null test then a tail jump through the
// implementation's vtable (`ret` when PImp is null). Same proxy shape as landed 0x004AAF40.
// Layout: engine/CScriptThing.h (+0x04 PImp, a CCountedPointer<CScriptThing>); the class is modelled with its virtuals
// so the slot offset comes from the declaration order (CScriptThing.h is a data-only header).
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              Data;
    T* operator->() const { return Data; }              // inline accessors: a direct `.Data->` read loads into eax
    bool IsValid() const { return Data != 0; }
    CCPPointerInfo* Info;
};
class CScriptThing {                                     // vtable 0x01238C8C (79 slots)
public:
    CCountedPointer<CScriptThing> PImp;                  // +0x04 the implementation (a derived CScriptThing)
    virtual ~CScriptThing();                             // slot 0
    // slots 1..66: GetName .. SetAsUsable (rtti-vtables.tsv); bodies elsewhere
    virtual void _slot01();
    virtual void _slot02();
    virtual void _slot03();
    virtual void _slot04();
    virtual void _slot05();
    virtual void _slot06();
    virtual void _slot07();
    virtual void _slot08();
    virtual void _slot09();
    virtual void _slot10();
    virtual void _slot11();
    virtual void _slot12();
    virtual void _slot13();
    virtual void _slot14();
    virtual void _slot15();
    virtual void _slot16();
    virtual void _slot17();
    virtual void _slot18();
    virtual void _slot19();
    virtual void _slot20();
    virtual void _slot21();
    virtual void _slot22();
    virtual void _slot23();
    virtual void _slot24();
    virtual void _slot25();
    virtual void _slot26();
    virtual void _slot27();
    virtual void _slot28();
    virtual void _slot29();
    virtual void _slot30();
    virtual void _slot31();
    virtual void _slot32();
    virtual void _slot33();
    virtual void _slot34();
    virtual void _slot35();
    virtual void _slot36();
    virtual void _slot37();
    virtual void _slot38();
    virtual void _slot39();
    virtual void _slot40();
    virtual void _slot41();
    virtual void _slot42();
    virtual void _slot43();
    virtual void _slot44();
    virtual void _slot45();
    virtual void _slot46();
    virtual void _slot47();
    virtual void _slot48();
    virtual void _slot49();
    virtual void _slot50();
    virtual void _slot51();
    virtual void _slot52();
    virtual void _slot53();
    virtual void _slot54();
    virtual void _slot55();
    virtual void _slot56();
    virtual void _slot57();
    virtual void _slot58();
    virtual void _slot59();
    virtual void _slot60();
    virtual void _slot61();
    virtual void _slot62();
    virtual void _slot63();
    virtual void _slot64();
    virtual void _slot65();
    virtual void _slot66();
    virtual void SetFriendsWithEverythingFlag(bool flag);    // slot 67 (+0x10c)
    virtual bool GetActivationTriggerStatus() const;         // slot 68
    virtual void SetActivationTriggerStatus(bool status);    // slot 69 (+0x114)
    virtual void SetToKillOnLevelUnload(bool kill);          // slot 70
    virtual void UpdateThingAttachment();                    // slot 71 (+0x11c)
    virtual void IncrementScriptCounter();                   // slot 72 (+0x120)
    virtual void DecrementScriptCounter();                   // slot 73 (+0x124)
};

void CScriptThing::DecrementScriptCounter()
{
    if (PImp.IsValid())
        PImp->DecrementScriptCounter();
}