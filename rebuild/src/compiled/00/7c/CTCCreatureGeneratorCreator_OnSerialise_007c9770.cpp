// CTCCreatureGeneratorCreator::OnSerialise(CPersistContext& context, bool fullState) -- with full state: persist the regeneration frame and the generator's thing UID (saving takes it from a live generator and remaps it; loading remaps a non-zero saved UID back)
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C9770, 300 bytes (`ret 8`). Vtable 0x0126EB3C (CTCCreatureGeneratorCreator) slot 1 (catalog
// "vfunc_1" in the manifest; the catalog's OnSerialise name is the slot's).
// Callees: CPersistContext::Transfer<long> @ 0x00410BE0 and Transfer<unsigned __int64> @ 0x0044BB00 (manifest "Transfer<signed_char>",
// folded labels), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CThing::IsLevelLess @ 0x004C7930, CTCBase::GetThingManager
// @ 0x00686AE0, CThingManager::RemapUIDForSerialisation(bool, bool, long, unsigned __int64) @ 0x0051E210 (edx:eax result).
// Names: "FrameToRegenerate" @ 0x0126EBBC, "GeneratorUID" @ 0x0126EBAC.
// Layout: Ego_r.pdb CTCCreatureGeneratorCreator (as the landed IsGeneratorAlive 0x007C98A0): FrameToRegenerate +0x0C, PGenerator +0x11
// (unaligned CIntelligentPointer<CThing>, pack(1)), GeneratorUID +0x20; CThing UID +0x18, level short +0x8E, dead bit +0x91;
// CPersistContext mode +0x18 (1 or 3 = saving). Model as the landed CTCActivationTrigger::OnSerialise 0x0077B0D0.
// Shape: the saving arm transfers a local UID (zeroed, assigned from the live generator, then remapped in place); the loading arm
// transfers the member and remaps it when non-zero; ebx is the zero register for the bool test and every default. The UID comes
// through an inline by-value getter (or a named generator local): a direct `GetPItem()->UID` read copies the pointer to edx first.
class CThing { public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned char    _pad20[0x8e - 0x20];
    short            m_level;                   // +0x8e
    unsigned char    _pad90;
    unsigned char    Flags91;                   // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    unsigned __int64 GetUID() const { return UID; }
public:
    bool IsLevelLess() const;   // 0x004C7930
};

class CThingManager { public:
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210
};

class CPersistContext { public:
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: long 0x00410BE0; unsigned __int64 0x0044BB00
};

#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
protected:
    CThingManager& GetThingManager();   // 0x00686AE0
};

class CTCCreatureGeneratorCreator : public CTCBase {
public:
    long FrameToRegenerate;                     // +0x0c
    bool Generated;                             // +0x10
    CIntelligentPointer<CThing> PGenerator;         // +0x11 CIntelligentPointer<CThing>
    unsigned char _pad19[0x20 - 0x19];
    unsigned __int64 GeneratorUID;              // +0x20
    void OnSerialise(CPersistContext& context, bool fullState);
};
#pragma pack(pop)

void CTCCreatureGeneratorCreator::OnSerialise(CPersistContext& context, bool fullState)
{
    if (fullState) {
        context.Transfer<long>("FrameToRegenerate", FrameToRegenerate, 0);
        if (context.IsSaving()) {
            unsigned __int64 uid = 0;
            if (PGenerator.GetPItem() && !PGenerator.GetPItem()->IsDead()) {
                uid = PGenerator.GetPItem()->GetUID();
                uid = GetThingManager().RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, uid);
            }
            context.Transfer<unsigned __int64>("GeneratorUID", uid, 0);
        } else {
            context.Transfer<unsigned __int64>("GeneratorUID", GeneratorUID, 0);
            if (GeneratorUID != 0)
                GeneratorUID = GetThingManager().RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, GeneratorUID);
        }
    }
}