// CEventScript::CEventScript(CScriptOwner* owner) -- script constructor: the script root keeps the owner (+0x04), the event container at +0x08 (a typed container over an embedded red-black tree, built with 0), the flag override at +0x38; then the script registers itself with the owner's script registry (+0x10 of the owner, virtual slot 9)
// Grade: DIFFER(132v132) -- scheduling only: retail loads the owner argument after `push esi ; mov esi,ecx ; mov [esp+4],esi` (`mov eax,[esp+18h]`), ours before `push esi` (`mov eax,[esp+14h]`); every other instruction matches
// Leaf: ??0CEventScript
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2CBD0, length 144 bytes (132 + int3 padding), __thiscall, `ret 4`, returns this.
// Counterpart: the destructor @ 0x00C2CC60 (landed as "CCombatScript_00c2cc60": own vtable 0x012B46FC, drains the container,
//   `m_pOwner->m_registry` slot 8 removes the script, the +0x38 member's base vtable 0x012B2828, the container vtable
//   0x012B46B8 with the tree destructor @ 0x00C19250 and the container root vtable 0x012B1AB0, the root vtable 0x012B463C).
// Callees: PKCTRRedblackTree::PKCTRRedblackTree @ 0x00C194F0 (vtable 0x012B2F0C, chunk 433's model; argument 0), the flag
//   override's constructor @ 0x00C2C6B0 (vtable 0x012B4630, four zero words; its ApplyTo is 0x00C2C670), the owner
//   registry's virtual slot 9 (+0x24).
// Name: no PDB covers this object (the catalog's `CQ_GuildTrainingPreMeleeScript_Initialize` is propagated and the landed
//   0x00C2C6B0 was retyped onto that game class by label alone); the names are descriptive.
// Shape: /EHsc, `this` homed in the `push ecx` frame word. The owner store precedes the script's vtable store: it belongs to
//   the inlined root constructor (EH state 0, empty inline root destructor; the root's own vtable store is dead). The
//   container's root likewise takes the address of the container's tree before the container's vtable store (state 1),
//   the container's `this` homed in the dead argument slot; state 2 after the container, 3 after the flag override. The
//   registration reads the owner back from the member (`mov eax,[esi+4]`).
class CEventScript;

struct CScriptRegistry {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CEventScript* script);   // slot 8 (+0x20)
    virtual void Add(CEventScript* script);      // slot 9 (+0x24)
};

struct CScriptOwner {
    unsigned char   _pad_0x00[0x10];
    CScriptRegistry m_registry;             // +0x10
};

class PKCTRRedblackTree {                   // 0x28 bytes (vtable 0x012B2F0C)
public:
    PKCTRRedblackTree(void* allocator);     // 0x00C194F0
    ~PKCTRRedblackTree();                   // 0x00C19250
private:
    void* m_words[10];
};

class CScriptRoot {                         // vtable 0x012B463C
public:
    virtual void v00();
    CScriptRoot(CScriptOwner* owner) : m_pOwner(owner) { }
    ~CScriptRoot() { }
    CScriptOwner* m_pOwner;                 // +0x04
};

class CContainerRoot {                      // vtable 0x012B1AB0
public:
    virtual void v00();
    CContainerRoot(PKCTRRedblackTree* tree) : m_pTree(tree) { }
    ~CContainerRoot() { }
    PKCTRRedblackTree* m_pTree;             // +0x04
};

class CEventContainer : public CContainerRoot {   // vtable 0x012B46B8, 0x30 bytes
public:
    virtual void v00();
    CEventContainer() : CContainerRoot(&m_tree), m_tree(0) { }
    ~CEventContainer() { }
    PKCTRRedblackTree m_tree;               // +0x08
};

class CFlagOverrideBase {                   // vtable 0x012B2828
public:
    virtual void v00();
    ~CFlagOverrideBase() { }
};

class CFlagOverride : public CFlagOverrideBase {  // vtable 0x012B4630, 12 bytes
public:
    CFlagOverride();                        // 0x00C2C6B0
    ~CFlagOverride() { }
private:
    unsigned short m_words[4];              // +0x04 .. +0x0a
};

class CEventScript : public CScriptRoot {   // vtable 0x012B46FC
public:
    virtual void v00();
    CEventScript(CScriptOwner* owner);
    ~CEventScript();                        // 0x00C2CC60
    CEventContainer m_events;               // +0x08
    CFlagOverride   m_flags;                // +0x38
};

CEventScript::CEventScript(CScriptOwner* owner)
    : CScriptRoot(owner)
{
    m_pOwner->m_registry.Add(this);
}
