// CSpecialAbilitiesMultiStrikeDef::operator=(const CSpecialAbilitiesMultiStrikeDef& other) -- memberwise copy: CParentDefClassBase part, then Sounds[4], Animations[4] (CDefString), HighlightWidth, HighlightColor, DelayTimeSecs, NumHits, PerHitDamagerMultiplier, AttachmentPoint (vector<CCharString>); returns *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045F716, length 147 bytes.
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<long>::operator= @ 0x00454886, vector<float>::operator= @ 0x0045EF2D, vector<CCharString>::operator= @ 0x00432EE9
// (catalog "CActiveFile::operator=" / "CCombatWheel::operator=" are propagated labels).
// Identity: slot 19 (Copy) of CSpecialAbilitiesMultiStrikeDef's vtable (0x0123412C, rtti-vtables.tsv) is the 5-byte `jmp` right before this body.
// Layout: Ego_r.pdb members shifted 4 bytes down (retail CParentDefClassBase is 0x39 bytes, vectors 12 bytes), arrays at +0x3c/+0x4c.
// TU: `optimize("y", off)` + `("s", on)` (ebp frame): the two 4-element arrays copy as explicit element `for` loops through ebx,
// the shared stride 4 in edx and the count homed in the dead `other` argument slot (`dec dword ptr [ebp+8]`).
// Leaf: operator=
#pragma optimize("y", off)
#pragma optimize("s", on)
#pragma pack(push, 1)
class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

class CDefString { public: long TablePos; };

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first; long* m_last; long* m_end;
    CLongVector& operator=(const CLongVector& other);     // 0x00454886
};
struct CFloatVector {                   // retail std::vector<float> (12 bytes)
    float* m_first; float* m_last; float* m_end;
    CFloatVector& operator=(const CFloatVector& other);   // 0x0045EF2D
};
class CCharString;
struct CStringVector {                  // retail std::vector<CCharString> (12 bytes)
    CCharString* m_first; CCharString* m_last; CCharString* m_end;
    CStringVector& operator=(const CStringVector& other); // 0x00432EE9
};

class CSpecialAbilitiesMultiStrikeDef : public CParentDefClassBase { public:   // vtable 0x0123412C
    CDefString    Sounds[4];                            // +0x3c
    CDefString    Animations[4];                        // +0x4c
    float         HighlightWidth;                       // +0x5c
    CLongVector   HighlightColor;                       // +0x60
    CFloatVector  DelayTimeSecs;                        // +0x6c
    CLongVector   NumHits;                              // +0x78
    CFloatVector  PerHitDamagerMultiplier;              // +0x84
    CStringVector AttachmentPoint;                      // +0x90
    CSpecialAbilitiesMultiStrikeDef& operator=(const CSpecialAbilitiesMultiStrikeDef& other);
};

CSpecialAbilitiesMultiStrikeDef& CSpecialAbilitiesMultiStrikeDef::operator=(const CSpecialAbilitiesMultiStrikeDef& other)
{
    CParentDefClassBase::operator=(other);
    for (int i = 0; i < 4; ++i)
        Sounds[i] = other.Sounds[i];
    for (int j = 0; j < 4; ++j)
        Animations[j] = other.Animations[j];
    HighlightWidth = other.HighlightWidth;
    HighlightColor = other.HighlightColor;
    DelayTimeSecs = other.DelayTimeSecs;
    NumHits = other.NumHits;
    PerHitDamagerMultiplier = other.PerHitDamagerMultiplier;
    AttachmentPoint = other.AttachmentPoint;
    return *this;
}