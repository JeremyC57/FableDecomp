// COpinionReactionManagerDef::Transfer(CPersistContext&) -- persist the opinion-reaction def: attitude blur vector, the reaction->attitude / targeting / delay maps, the tolerance map, the four per-reaction bool maps, the match list and the frequency traits array
// Leaf: ?Transfer@COpinionReactionManagerDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x00679740, length 447 bytes, virtual, `ret 4`.
// Callees: CPersistContext::TransferVectorOfSubComponents<CBlurredAttitudesDef> 0x0067D6B0; CPersistContext::CheckCRC 0x00404500
//   (x8, the empty key "" @ 0x0122D70E); `__fastcall` (stream, map*) map serialisers -- out/in pairs:
//   map<EOpinionReactionType,EOpinionAttitudeType> 0x0067B8B0 / 0x0067C020, map<..,EOpinionTargetingConditionType> 0x0067B930 /
//   0x0067C110, map<EOpinionReactionType,float> 0x0067B9B0 / 0x0067C200, map<EOpinionAttitudeType,float> 0x004ECD6B /
//   0x004ECDAC, map<EOpinionReactionType,bool> 0x0045DF1E / 0x0045DF5E (manifest labels on these are propagated);
//   the match-list transfer 0x0067BFB0 and CPersistContext_TransferableComponent<CReactionFrequencyTraitsArray> 0x0067B820.
// Layout: Ego_r.pdb COpinionReactionManagerDef members from +0x40 sit at retail +0x3c (12-byte STLport vectors/maps, a 4-byte
//   STLport list at +0xa8): AttitudeBlur +0x3c, AttitudeCondition +0x48, TargetingCondition +0x54, PreReactionDelay +0x60,
//   ToleranceToBeingHit +0x6c, BlockFurtherReactions +0x78, AllowSpeechOnNonPureAISpeaker +0x84, AllowWhileCarrying +0x90,
//   AllowWhileFollowingPlayer +0x9c, Matches +0xa8, FrequencyTraitsArray +0xac.
// Shape: the inline CPersistContext::Transfer template of the landed CCreatureGeneratorDef::Transfer 0x0074C5F0 -- CRC check,
//   then `switch (m_mode)` with mode 2 loading from +0x24 and mode 3 saving to +0x28, each map type with its own pair.
class CMemoryDataInputStream;
class CDataOutputStream;

#define FABLE_RETAIL_MAP(Name, OutVa, InVa) \
    struct Name { void* m_header[3]; }; \
    void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream* stream, Name* value); \
    void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream* stream, Name* value)

FABLE_RETAIL_MAP(CReactionAttitudeMap, 0x0067B8B0, 0x0067C020);    // map<EOpinionReactionType, EOpinionAttitudeType>
FABLE_RETAIL_MAP(CReactionTargetingMap, 0x0067B930, 0x0067C110);   // map<EOpinionReactionType, EOpinionTargetingConditionType>
FABLE_RETAIL_MAP(CReactionFloatMap, 0x0067B9B0, 0x0067C200);       // map<EOpinionReactionType, float>
FABLE_RETAIL_MAP(CAttitudeFloatMap, 0x004ECD6B, 0x004ECDAC);       // map<EOpinionAttitudeType, float>
FABLE_RETAIL_MAP(CReactionBoolMap, 0x0045DF1E, 0x0045DF5E);        // map<EOpinionReactionType, bool>

struct CBlurredAttitudesDefVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; };
struct CReactionMatchList { void* _M_node; };                       // STLport list<CReactionMatch*>
class CReactionFrequencyTraitsArray { public: unsigned char _body[0x278]; };

class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);                    // 0x00404500
    void TransferVectorOfSubComponents(CBlurredAttitudesDefVector& value);     // 0x0067D6B0
    void TransferMatches(CReactionMatchList& value);                           // 0x0067BFB0
    void TransferComponent(CReactionFrequencyTraitsArray& value);              // 0x0067B820
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            GFSerialiseMapBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            GFSerialiseMapBinaryOut(m_pSaveStream, &value);
            break;
        }
    }
};

class COpinionReactionManagerDef { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char                 _defBase[0x3c - 4];             // CParentDefClassBase (retail 0x39 bytes + padding)
    CBlurredAttitudesDefVector    AttitudeBlur;                   // +0x3c
    CReactionAttitudeMap          AttitudeCondition;              // +0x48
    CReactionTargetingMap         TargetingCondition;             // +0x54
    CReactionFloatMap             PreReactionDelay;               // +0x60
    CAttitudeFloatMap             ToleranceToBeingHit;            // +0x6c
    CReactionBoolMap              BlockFurtherReactions;          // +0x78
    CReactionBoolMap              AllowSpeechOnNonPureAISpeaker;  // +0x84
    CReactionBoolMap              AllowWhileCarrying;             // +0x90
    CReactionBoolMap              AllowWhileFollowingPlayer;      // +0x9c
    CReactionMatchList            Matches;                        // +0xa8
    CReactionFrequencyTraitsArray FrequencyTraitsArray;           // +0xac
};

void COpinionReactionManagerDef::Transfer(CPersistContext& context)
{
    context.TransferVectorOfSubComponents(AttitudeBlur);
    context.Transfer("", AttitudeCondition);
    context.Transfer("", TargetingCondition);
    context.Transfer("", PreReactionDelay);
    context.Transfer("", ToleranceToBeingHit);
    context.Transfer("", BlockFurtherReactions);
    context.Transfer("", AllowSpeechOnNonPureAISpeaker);
    context.Transfer("", AllowWhileCarrying);
    context.Transfer("", AllowWhileFollowingPlayer);
    context.TransferMatches(Matches);
    context.TransferComponent(FrequencyTraitsArray);
}