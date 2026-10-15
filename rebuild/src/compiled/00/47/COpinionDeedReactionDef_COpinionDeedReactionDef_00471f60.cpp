// COpinionDeedReactionDef::COpinionDeedReactionDef(const COpinionDeedReactionDef& other) -- copy constructor: def base, own vtable, the attitude list, the animation name word and the dialogue tag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471F60, length 51 bytes (`ret 4`, eax = this). Callees:
// CParentDefClassBase::CParentDefClassBase(const CParentDefClassBase&) @ 0x0046EE98, the attitude vector's copy
// constructor @ 0x00471F97 (manifest "vector_insert_range_copy", propagated).
// Layout: the PDB COpinionDeedReactionDef (88 bytes) has Attitudes (vector<EOpinionAttitudeType>) at +0x40, Animation
// (CDefString) +0x50, DialogueTag +0x54 after a 0x40-byte def base; retail reads +0x3C / +0x48 / +0x4C (the def base is
// 0x3C bytes, the vector 12). Vtable 0x012361DC = COpinionDeedReactionDef (rtti-vtables.tsv).
// Shape: the vtable store is scheduled between the member's `lea ecx` and its copy-constructor call; a size-optimised
// module (#pragma optimize("s", on): both trailing words through eax).
// Leaf: ??0COpinionDeedReactionDef
#pragma optimize("s", on)
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE = 0 };

class CParentDefClassBase {                        // 0x3C bytes in retail (vptr + def base)
public:
    virtual void v000();
    unsigned char _body[0x38];
    CParentDefClassBase(const CParentDefClassBase& other);               // 0x0046EE98
    CParentDefClassBase& operator=(const CParentDefClassBase& other);    // 0x00431ED0
};

class CAttitudeVector {                            // retail std::vector<EOpinionAttitudeType>, 12 bytes
public:
    EOpinionAttitudeType* m_pFirst;
    EOpinionAttitudeType* m_pLast;
    EOpinionAttitudeType* m_pEnd;
    CAttitudeVector(const CAttitudeVector& other);                       // 0x00471F97
    CAttitudeVector& operator=(const CAttitudeVector& other);            // 0x00471E44
};

class COpinionDeedReactionDef : public CParentDefClassBase {             // vtable 0x012361DC
public:
    virtual void v000();
    CAttitudeVector Attitudes;                     // +0x3C (PDB +0x40)
    unsigned long   Animation;                     // +0x48 (CDefString: one word, copied as a word)
    unsigned long   DialogueTag;                   // +0x4C

    COpinionDeedReactionDef(const COpinionDeedReactionDef& other);
    COpinionDeedReactionDef& operator=(const COpinionDeedReactionDef& other);
};
COpinionDeedReactionDef::COpinionDeedReactionDef(const COpinionDeedReactionDef& other)
    : CParentDefClassBase(other),
      Attitudes(other.Attitudes),
      Animation(other.Animation),
      DialogueTag(other.DialogueTag)
{
}