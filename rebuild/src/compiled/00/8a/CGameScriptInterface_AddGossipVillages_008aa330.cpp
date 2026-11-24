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
void __fastcall Impl_AddGossipVillages(CCharString byVal);

void CGameScriptInterface::AddGossipVillages(class CCharString val)
{
    Impl_AddGossipVillages(val);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_AddGossipVillages -> CGameScriptInterface::AddGossipVillages, the RTTI-placed PDB member; class model: the row's local class model.
