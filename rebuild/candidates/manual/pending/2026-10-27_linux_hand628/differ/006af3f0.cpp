// CMessageEventFilter_IsMyDeadLeader::operator()(const CMessageEvent& event) const -- a death message (type 9) whose target thing (alive, typed CThing) has the definition my creature's def names as its leader (LeaderTypeDef)
// Grade: DIFFER(172v160) -- the compiled-out typed-pointer check is now reproduced (GetPItem homed in the dead event slot + `xor al,al ; jne`, via CIntelligentPointer<CThing>::Get() with IsThingOfType<T>(const CThing* const&) and a mask local); left: (1) the CDefPointer zero store sits before `movzx/lea/push` (retail after the two pushes), (2) the true exit calls the def-pointer dtor out of line (`lea ecx ; call`), retail inlines `dec [ecx+4] ; jne ; call [eax+4]`, (3) the false exit re-reads Object after `dec [eax+4]` (retail keeps it in ecx). __forceinline ctor/dtor (with or without a local pointee) and the shared engine/CDefPointer.h all give a 192-byte `mov esi,[eax+4] ; add eax,4 ; dec esi ; mov [eax],esi` release.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF3F0, 160 bytes, `ret 4`. Identity: both PDB name sets hold
//   ??RCMessageEventFilter_IsMyDeadLeader@@QBE_NABVCMessageEvent@@@Z (member Creature +0, CThingCreatureBase*).
// Callees: CBaseIntelligentPointer::GetPItem const 0x00A01B50 (three times: CIntelligentPointer<CThing>::Get inline);
//   CThing::PeekDefinitionManager const 0x004C7990 (returns const CGameDefinitionManager&, called on my creature);
//   CDefinitionManager::GetDef<CThingCreatureDef>(long, CDefPointer<const CThingCreatureDef>*) const 0x00513160;
//   the def's virtual slot 1 (Delete) at count zero.
// Layout: CMessageEvent Type +0x08, PTarget +0x50 (Ego_r.pdb); retail CThing ThingTypeFlags +0x6C, DefGlobalIndex
//   +0x8C (Ego_r +0x9C); CThingCreatureDef::LeaderTypeDef retail +0x13C (Ego_r +0x144).
// Leaf: ??RCMessageEventFilter_IsMyDeadLeader@@QBE_NABVCMessageEvent@@@Z
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable long IVRefCount;                                // +0x04
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object) {
            if (--Object->IVRefCount == 0)
                Object->Delete();
        }
    }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
};
class CThingCreatureDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x13c - 0x08];
    long          LeaderTypeDef;                            // +0x13c (Ego_r.pdb +0x144)
};
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* def) const;   // 0x00513160 (T = CThingCreatureDef)
};
class CGameDefinitionManager : public CDefinitionManager { };
class CGameDefinitionManager;
class CThing { public:
    enum { ThingTypeMask = 0 };
    unsigned char  _pad00[0x6c];
    unsigned long  ThingTypeFlags;                          // +0x6c
    unsigned char  _pad70[0x8c - 0x70];
    unsigned short DefGlobalIndex;                          // +0x8c (Ego_r.pdb +0x9c)
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x004C7990
};
class CThingCreatureBase : public CThing { };
template <class T> inline bool IsThingOfType(const CThing* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
class CBaseIntelligentPointer { public:
    void* __vftable;
    void* PData;
    CThing* GetPItem() const;                               // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (GetPItem() != 0 && IsThingOfType<T>((T*)GetPItem())) ? (T*)GetPItem() : 0; }
};
enum EMessageEventType { MESSAGE_EVENT_TYPE_9 = 9 };
class CMessageEvent { public:
    unsigned char _pad00[0x08];
    EMessageEventType Type;                                 // +0x08
    unsigned char _pad0c[0x50 - 0x0c];
    CIntelligentPointer<CThing> PTarget;                    // +0x50
};
class CMessageEventFilter_IsMyDeadLeader { public:
    CThingCreatureBase* Creature;                           // +0x00
    bool operator()(const CMessageEvent& event) const;
};

bool CMessageEventFilter_IsMyDeadLeader::operator()(const CMessageEvent& event) const
{
    if (event.Type == MESSAGE_EVENT_TYPE_9) {
        CThing* dead = event.PTarget.Get();
        if (dead) {
            CDefPointer<const CThingCreatureDef> def;
            if (Creature->PeekDefinitionManager().GetDef(Creature->DefGlobalIndex, &def)) {
                if (def.Get() && def->LeaderTypeDef == dead->DefGlobalIndex)
                    return true;
            }
        }
    }
    return false;
}
