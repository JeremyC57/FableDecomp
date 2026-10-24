// CThingAICreature::SerialiseGameState(CPersistContext& context) -- after the base creature serialise, persist the relationship block (home/work buildings, father, mother, spouse as saved UIDs), the initial position, the overriding brain name and the ten AI flag bits
// Leaf: ?SerialiseGameState@CThingAICreature
// Grade: DIFFER(ours v retail) -- 1505v1504: two residues. (1) The empty CPersistTraits<float> temporary of the three
//   InitialPos transfers sits at [esp+0x1b] in ours, retail places it on hasRelationships' byte (the context arg slot,
//   [esp+0x20]); (2) the bit-7 EnableCreatureAutoPlacing write-back loads the flag before the member byte (retail loads the
//   member byte first: mov al,[esi+1c8h]; mov cl,flag; and al,7fh; shl cl,7). Tried: named traits local in the template
//   (slot 0x14), early `return false` form, two-arg float Transfer, an inline/__forceinline C3DVector::Transfer.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830FE0, 1504 bytes, `ret 4`; virtual SerialiseGameState of CThingAICreature.
// Callees: CThingCreatureBase::SerialiseGameState 0x006628E0, CPersistContext::Transfer<bool> 0x004045C0 /
//   Transfer<unsigned __int64> 0x0044BB00 / Transfer<CCharString> 0x00411010 / CheckCRC 0x00404500 (pooled "" 0x0122D70E),
//   CPersistTraits<float>::TransferBinaryIn 0x0040EFB0, CDataOutputStream::WriteFloat 0x00993EE0, operator new 0x00BFEA1A,
//   CCountedPointer<CRelationshipsInit>::Reset(T*) 0x00834D30 (landed), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   operator delete 0x00BFE9BC.
// Layout: retail CThingAICreature (Ego_r.pdb less 0x28 here, CThingCreatureBase less 0x1c): InitialPos +0xe8,
//   RelationshipsInit +0x160, HomeBuilding .. SpouseCreature +0x168..+0x188, OverridingBrainName +0x190, AI flag bits +0x1c8/+0x1c9.
class CCharString;
class CMemoryDataInputStream;
void __cdecl operator delete(void* block);              // 0x00BFE9BC (MSVCR71)
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A (MSVCR71)

class CDataOutputStream {
public:
    void WriteFloat(float value);                       // 0x00993EE0
};
template<class T> struct CPersistTraits;
template<> struct CPersistTraits<float> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, float* value);   // 0x0040EFB0
    static void TransferBinaryOut(CDataOutputStream* stream, const float& value) { stream->WriteFloat(value); }
};

class CPersistContext {
public:
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (1/3 = save, 2 = load)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);                    // 0x00404500
    void Transfer(const char* name, bool& value, const bool& defaultValue);                           // 0x004045C0
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
    void Transfer(const char* name, CCharString* value);                                              // 0x00411010
    template<class T> void Transfer(const char* name, T& value, const T& defaultValue)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
};

#pragma pack(push, 4)
class CThing {
public:
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                               // +0x18
    unsigned char _pad20[0x91 - 0x20];
    unsigned char Flags91;                              // +0x91 bit 0 Dead
    unsigned char _pad92[2];
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CThing* PItem;                                      // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};

struct C3DVector { float X, Y, Z; };

class CThingCreatureBase : public CThing {
public:
    unsigned char _pad94[0xe8 - 0x94];
    C3DVector InitialPos;                               // +0xe8
    bool SerialiseGameState(CPersistContext& context);  // 0x006628E0
};

class CThingAICreature : public CThingCreatureBase {
public:
    struct CRelationshipsInit {
        unsigned __int64 HomeBuildingUID;               // +0x00
        unsigned __int64 WorkBuildingUID;               // +0x08
        unsigned __int64 FatherCreatureUID;             // +0x10
        unsigned __int64 MotherCreatureUID;             // +0x18
        unsigned __int64 SpouseCreatureUID;             // +0x20
        CRelationshipsInit() : HomeBuildingUID(0), WorkBuildingUID(0), FatherCreatureUID(0), MotherCreatureUID(0), SpouseCreatureUID(0) { }
    };
    struct CCPPointerInfo {
        long RefCount;
        void (__fastcall* DeleteFunc)(void* data);
        void* Data;
    };
    struct CRelationshipsInitPointer {                  // CCountedPointer<CRelationshipsInit>
        CRelationshipsInit* Data;
        CCPPointerInfo*     Info;
        void Reset(CRelationshipsInit* data);           // 0x00834D30
        void Reset()
        {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Info = 0;
            Data = 0;
        }
    };
    unsigned char _padf4[0x160 - 0xf4];
    CRelationshipsInitPointer RelationshipsInit;        // +0x160
    CBaseIntelligentPointer   HomeBuilding;             // +0x168
    CBaseIntelligentPointer   WorkBuilding;             // +0x170
    CBaseIntelligentPointer   FatherCreature;           // +0x178
    CBaseIntelligentPointer   MotherCreature;           // +0x180
    CBaseIntelligentPointer   SpouseCreature;           // +0x188
    CCharString*              OverridingBrainNameData;  // +0x190 (CCharString)
    unsigned char _pad194[0x1c8 - 0x194];
    bool PerformingVillageTaskFlag : 1;                 // +0x1c8
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;
    bool EnableCreatureAutoPlacing : 1;                 // bit 7
    bool AllowedToFollowHero : 1;                       // +0x1c9
    bool RegionFollowingOverriddenFromScript : 1;
    bool RespondingToFollowAndWait : 1;
    bool CanBeCourted : 1;
    bool CanBeMarried : 1;
    bool SerialiseGameState(CPersistContext& context);
};
#pragma pack(pop)

bool CThingAICreature::SerialiseGameState(CPersistContext& context)
{
    if (CThingCreatureBase::SerialiseGameState(context)) {
        bool hasRelationships = true;
        context.Transfer("HasRelationships", hasRelationships, false);
        RelationshipsInit.Reset(new CRelationshipsInit);
        if (context.m_mode == 1 || context.m_mode == 3) {
            if (HomeBuilding.GetPItem() && !HomeBuilding.GetPItem()->IsDead())
                RelationshipsInit.Data->HomeBuildingUID = HomeBuilding.GetPItem()->UID;
            if (WorkBuilding.GetPItem() && !WorkBuilding.GetPItem()->IsDead())
                RelationshipsInit.Data->WorkBuildingUID = WorkBuilding.GetPItem()->UID;
            if (FatherCreature.GetPItem() && !FatherCreature.GetPItem()->IsDead())
                RelationshipsInit.Data->FatherCreatureUID = FatherCreature.GetPItem()->UID;
            if (MotherCreature.GetPItem() && !MotherCreature.GetPItem()->IsDead())
                RelationshipsInit.Data->MotherCreatureUID = MotherCreature.GetPItem()->UID;
            if (SpouseCreature.GetPItem() && !SpouseCreature.GetPItem()->IsDead())
                RelationshipsInit.Data->SpouseCreatureUID = SpouseCreature.GetPItem()->UID;
        }
        context.Transfer("", InitialPos.X, 0.0f);
        context.Transfer("", InitialPos.Y, 0.0f);
        context.Transfer("", InitialPos.Z, 0.0f);
        if (hasRelationships) {
            context.Transfer("HomeBuildingUID", RelationshipsInit.Data->HomeBuildingUID, (unsigned __int64)0);
            context.Transfer("WorkBuildingUID", RelationshipsInit.Data->WorkBuildingUID, (unsigned __int64)0);
            context.Transfer("FatherCreatureUID", RelationshipsInit.Data->FatherCreatureUID, (unsigned __int64)0);
            context.Transfer("MotherCreatureUID", RelationshipsInit.Data->MotherCreatureUID, (unsigned __int64)0);
            context.Transfer("SpouseCreatureUID", RelationshipsInit.Data->SpouseCreatureUID, (unsigned __int64)0);
        }
        if (context.m_mode == 1 || context.m_mode == 3)
            RelationshipsInit.Reset();
        context.Transfer("OverridingBrainName", (CCharString*)&OverridingBrainNameData);
        {
            bool flag = EnableCreatureAutoPlacing;
            context.Transfer("EnableCreatureAutoPlacing", flag, false);
            EnableCreatureAutoPlacing = flag;
        }
        {
            bool flag = HasInformation;
            context.Transfer("HasInformation", flag, false);
            HasInformation = flag;
        }
        {
            bool flag = WanderWithInformation;
            context.Transfer("WanderWithInformation", flag, false);
            WanderWithInformation = flag;
        }
        {
            bool flag = WaveWithInformation;
            context.Transfer("WaveWithInformation", flag, false);
            WaveWithInformation = flag;
        }
        {
            bool flag = ContinueAIWithInformation;
            context.Transfer("ContinueAIWithInformation", flag, false);
            ContinueAIWithInformation = flag;
        }
        {
            bool flag = AllowedToFollowHero;
            context.Transfer("AllowedToFollowHero", flag, false);
            AllowedToFollowHero = flag;
        }
        {
            bool flag = RegionFollowingOverriddenFromScript;
            context.Transfer("RegionFollowingOverriddenFromScript", flag, false);
            RegionFollowingOverriddenFromScript = flag;
        }
        {
            bool flag = RespondingToFollowAndWait;
            context.Transfer("RespondingToFollowAndWait", flag, true);
            RespondingToFollowAndWait = flag;
        }
        {
            bool flag = CanBeCourted;
            context.Transfer("CanBeCourted", flag, false);
            CanBeCourted = flag;
        }
        {
            bool flag = CanBeMarried;
            context.Transfer("CanBeMarried", flag, false);
            CanBeMarried = flag;
        }
        return true;
    }
    return false;
}
