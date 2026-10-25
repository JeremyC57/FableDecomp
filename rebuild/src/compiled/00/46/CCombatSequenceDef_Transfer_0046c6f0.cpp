// CCombatSequenceDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (ValidCombatZones, Type, InterruptionType, IsValidType, OnStartType, OnStopType, ActionList, SecondsToPredictTargetPosition, PreferIfValid)
// Leaf: ?Transfer@CCombatSequenceDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046C6F0, 112 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CCombatSequenceDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CCombatSequenceDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the nine transferred offsets and their callee
//   overloads match Ego_r.pdb CCombatSequenceDef's members and types exactly (retail offset = Ego_r offset - 4 up to ActionList, - 8 after it: STLport vectors are 12 bytes). Family G057 (seven def Transfers of nine fields).
// Callees: CPersistContext::Transfer(unsigned long&) 0x00431020, Transfer(long&) 0x00431102, Transfer(float&) 0x00431061,
//   Transfer(bool&) 0x0043314A (each bound under that name by landed defs); member-template instances Transfer<ECombatSequenceType> 0x0046C760, Transfer<ECombatSequenceInterruptionType> 0x0046C804, Transfer<ECombatSequenceIsValidType> 0x0046C8A8, Transfer<ECombatSequenceOnStartModuleType> 0x0046C94C, Transfer<ECombatSequenceOnStopModuleType> 0x0046C9F0, Transfer<vector<EActionRegisteredType>> 0x0046CA94.
// Shape: context in edi and this in esi for the whole body; the last field's address is `add esi, N` in place.
#pragma optimize("s", on)
enum ECombatSequenceInterruptionType { ECombatSequenceInterruptionType_Force32 = 0x7fffffff };
enum ECombatSequenceIsValidType { ECombatSequenceIsValidType_Force32 = 0x7fffffff };
enum ECombatSequenceOnStartModuleType { ECombatSequenceOnStartModuleType_Force32 = 0x7fffffff };
enum ECombatSequenceOnStopModuleType { ECombatSequenceOnStopModuleType_Force32 = 0x7fffffff };
enum ECombatSequenceType { ECombatSequenceType_Force32 = 0x7fffffff };
enum EActionRegisteredType { EActionRegisteredType_Force32 = 0x7fffffff };
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport: three pointers
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CPersistContext {
public:
    void Transfer(unsigned long& value);                   // 0x00431020
    void Transfer(long& value);                            // 0x00431102
    void Transfer(float& value);                           // 0x00431061
    void Transfer(bool& value);                            // 0x0043314A
    template <class T> void Transfer(T& value);            // enum / container instances in this TU
};
class CCombatSequenceDef {                                              // retail fields from +0x3c
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                    // +0x04 def base body
    long ValidCombatZones;                   // +0x3c
    ECombatSequenceType Type;                // +0x40
    ECombatSequenceInterruptionType InterruptionType; // +0x44
    ECombatSequenceIsValidType IsValidType;  // +0x48
    ECombatSequenceOnStartModuleType OnStartType; // +0x4c
    ECombatSequenceOnStopModuleType OnStopType; // +0x50
    std::vector<EActionRegisteredType> ActionList; // +0x54
    float SecondsToPredictTargetPosition;    // +0x60
    bool PreferIfValid;                      // +0x64
};
void CCombatSequenceDef::Transfer(CPersistContext& context)
{
    context.Transfer(ValidCombatZones);
    context.Transfer(Type);
    context.Transfer(InterruptionType);
    context.Transfer(IsValidType);
    context.Transfer(OnStartType);
    context.Transfer(OnStopType);
    context.Transfer(ActionList);
    context.Transfer(SecondsToPredictTargetPosition);
    context.Transfer(PreferIfValid);
}