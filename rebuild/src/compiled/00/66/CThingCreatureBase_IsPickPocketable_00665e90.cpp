// CThingCreatureBase::IsPickPocketable() -- true when the creature def allows pick-pocketing, the hero is alive, the creature is not watching the hero, and the hero is within 2 units
// Leaf: ?IsPickPocketable@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00665E90, 241 bytes (+15 int3), `ret`, bool in al.
// Callees: CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*) @ 0x00485815 (manifest names a propagated
//   CAbilityDef instantiation), CThing::GetPlayerManager @ 0x004C79F0 (`[g_0x013B8A1C]+0x30`), CPlayerManager::GetMainPlayer
//   @ 0x00449960, CPlayer::GetPControlledCreature @ 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0
//   (the inlined GetInterface for CTCPerceiveThing, type 0x34 = bit 20 of the interface bitset word at +0x24),
//   CTCPerceiveThing::GetPerceivedThing @ 0x007446A0, CTCPerceiveThing::CanSeeOrSmellPerceivedThing @ 0x00661600 (landed),
//   CThing::GetPos @ 0x004C73D0, GFWorld3DDistanceSmallerThan(const C3DVector&, const C3DVector&, float) @ 0x006614F0
//   (`__fastcall`), the def's virtual slot 1 (+4) at a zero count.
// Layout: retail CThing PBaseDef +0x70, dead flag +0x91 bit 0, interface bitset +0x20, interface map +0x44; CCreatureDef
//   PickPocketable +0xCA (PDB +0xDA).
// Shape: the sub-def out pointer is the zeroed `push ecx` word (a same-TU stand-in GetSubDef keeps its address from escaping,
//   so the def lives in ebp and the lookup key reuses the word); a null def skips to the single shared `return false` exit (no release); the success
//   path releases inline and returns `mov al,1`, every failure joins one release + `xor al,al`.
class C3DVector { public: float X, Y, Z; };
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);   // 0x006614F0
class CDefPointeeBase { public:                        // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
    long m_refCount;                            // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> class CDefPointer { public:          // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
};
class CCreatureDef : public CDefPointeeBase { public:
    unsigned char _pad08[0xca - 0x08];
    bool PickPocketable;                        // +0xca
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CCreatureDef>* out);         // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing;
class CThingPlayerCreature;
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;            // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;               // 0x00449960
};
class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;            // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PBaseDef;                // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_statusFlags;           // +0x91 (bit 0: dead)
    const C3DVector& GetPos() const;   // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x004C79F0
};
class CThingPlayerCreature : public CThing { };
class CTCPerceiveThing { public:
    CThing* GetPerceivedThing() const;          // 0x007446A0
    bool CanSeeOrSmellPerceivedThing() const;   // 0x00661600
};
class CThingCreatureBase : public CThing { public:
    bool IsPickPocketable();
};

bool CThingCreatureBase::IsPickPocketable()
{
    CDefPointer<CCreatureDef> out;
    PBaseDef->GetSubDef(&out);
    CCreatureDef* def = out.m_pObject;
    if (def) {
        if (def->PickPocketable) {
            CThing* hero = PeekPlayerManager().GetMainPlayer()->GetPControlledCreature();
            CTCPerceiveThing* perceive;
            if (hero && !(hero->m_statusFlags & 1) && GetInterface(&perceive, 0x34)) {
                if (perceive->GetPerceivedThing() != hero || !perceive->CanSeeOrSmellPerceivedThing()) {
                    if (GFWorld3DDistanceSmallerThan(GetPos(), hero->GetPos(), 2.0f)) {
                        def->ReleaseDef();
                        return true;
                    }
                }
            }
        }
        def->ReleaseDef();
    }
    return false;
}