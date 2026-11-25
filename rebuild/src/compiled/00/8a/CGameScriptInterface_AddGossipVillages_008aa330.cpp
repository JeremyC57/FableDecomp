// Leaf: ?AddGossipVillages@CGameScriptInterface@@UAEXVCCharString@@@Z
// Byte-exact reconstruction of CGameScriptInterface::AddGossipVillages @ 0x008aa330
// impl 0x8ae890 takes the CCharString BY VALUE; the caller copy-constructs the temp,
// the callee owns/destroys it. The thunk then destroys its own 'val' parameter.

class CCharString { public:
    void* data;
    CCharString(const CCharString& o);
    ~CCharString();
};

class CGameScriptInterface { public:
    virtual void AddGossipVillages(class CCharString);
};

// By-value param => caller copy-constructs into the arg slot, CALLEE destroys it,
// so no destructor for the copy is emitted in this thunk (only 'val' is destroyed).

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x008AE890 ?AddGossipVillages@CTCAIScratchpad@@SIXVCCharString@@@Z (was ?Impl_AddGossipVillages@@YIXVCCharString@@@Z)
class CTCAIScratchpad { public: static void __fastcall AddGossipVillages(CCharString); };
void CGameScriptInterface::AddGossipVillages(class CCharString val)
{
    CTCAIScratchpad::AddGossipVillages(val);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_AddGossipVillages -> CGameScriptInterface::AddGossipVillages, the RTTI-placed PDB member; class model: the row's local class model.
