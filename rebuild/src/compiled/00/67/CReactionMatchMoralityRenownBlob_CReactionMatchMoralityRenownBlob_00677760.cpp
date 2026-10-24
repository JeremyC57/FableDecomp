// CReactionMatch_MoralityRenownBlob::CReactionMatch_MoralityRenownBlob(EOpinionReactionType type, float yMin, float yMax, float xMin, float xMax, float scarinessShift, float agreeablenessShift, float attractivenessShift) -- build the blob as an MR pyramid centred midway in both ranges (axes 0 and 1), keeping the three opinion shifts
// Leaf: ??0CReactionMatch_MoralityRenownBlob
//
// Retail 0x00677760, 116 bytes (+ int3 padding to 128), thiscall `ret 20h` (type + seven floats), returns `this`.
// Callee: CReactionMatch_AsymmetricPyramid::CReactionMatch_AsymmetricPyramid @ 0x00677620 (landed; `ret 24h`).
// Constant: 0.5f @ 0x0122F59C.
// Identity: vtable 0x0125B748, RTTI .?AVCReactionMatch_MoralityRenownBlob@@ with bases CReactionMatch_MRPyramid,
//   CReactionMatch_AsymmetricPyramid, CReactionMatch (slots 1-4 are MRPyramid's Match 0x00700320 / GetMatchType
//   0x00677740 / TransferBinaryIn 0x006FF810 / TransferBinaryOut 0x006FECA0).  Ego_r.pdb names the ctor
//   ??0CReactionMatch_MoralityRenownBlob@@QAE@W4EOpinionReactionType@@MMMMMMM@Z.  Its one caller is the reaction-match
//   parser at 0x00678E13 (after `push 34h ; call operator new`).
// Layout: engine/CReactionMatch_MRPyramid.h (0x34: the AsymmetricPyramid body +0x00..+0x24, ScarinessShift +0x28,
//   AgreeablenessShift +0x2c, AttractivenessShift +0x30); the blob adds no members (PDB size 0x34).
// Shape: the MRPyramid ctor (out of line at 0x006776D0: `AsymmetricPyramid(type, 0, 1, x.., y..)`, then the three
//   shifts) is inlined here; its own vtable store is dead under the blob's.  Centres are `(max - min) * 0.5f + min`.
#include "engine/CReactionMatch_AsymmetricPyramid.h"
#include "engine/CReactionMatch_MRPyramid.h"

enum EOpinionReactionType { OPINION_REACTION_TYPE_0 = 0 };

class CReactionMatch {
public:
    virtual ~CReactionMatch();
    long ReactionType;                                      // +0x04 EOpinionReactionType
};

class CReactionMatch_AsymmetricPyramidRetail : public CReactionMatch {    // polymorphic model of engine/CReactionMatch_AsymmetricPyramid.h
public:
    long  XAxisOpinionType;                                 // +0x08 EOpinion
    long  YAxisOpinionType;                                 // +0x0c EOpinion
    float XCentre;                                          // +0x10
    float NegInvXRadius;                                    // +0x14
    float PosInvXRadius;                                    // +0x18
    float YCentre;                                          // +0x1c
    float NegInvYRadius;                                    // +0x20
    float PosInvYRadius;                                    // +0x24
    CReactionMatch_AsymmetricPyramidRetail(EOpinionReactionType type, long xAxis, long yAxis, float xMin, float xCentre,
                                     float xMax, float yMin, float yCentre, float yMax);   // 0x00677620
};

class CReactionMatch_MRPyramidRetail : public CReactionMatch_AsymmetricPyramidRetail {   // polymorphic model of engine/CReactionMatch_MRPyramid.h
public:
    float ScarinessShift;                                   // +0x28
    float AgreeablenessShift;                               // +0x2c
    float AttractivenessShift;                              // +0x30
    CReactionMatch_MRPyramidRetail(EOpinionReactionType type, float xMin, float xCentre, float xMax, float yMin, float yCentre,
                             float yMax, float scarinessShift, float agreeablenessShift, float attractivenessShift)
        : CReactionMatch_AsymmetricPyramidRetail(type, 0, 1, xMin, xCentre, xMax, yMin, yCentre, yMax),
          ScarinessShift(scarinessShift), AgreeablenessShift(agreeablenessShift), AttractivenessShift(attractivenessShift)
    {
    }
    virtual ~CReactionMatch_MRPyramidRetail();
};

class CReactionMatch_MoralityRenownBlob : public CReactionMatch_MRPyramidRetail {
public:
    CReactionMatch_MoralityRenownBlob(EOpinionReactionType type, float yMin, float yMax, float xMin, float xMax,
                                      float scarinessShift, float agreeablenessShift, float attractivenessShift);
    virtual ~CReactionMatch_MoralityRenownBlob();
};
FABLE_STATIC_ASSERT(sizeof(CReactionMatch_AsymmetricPyramidRetail) == sizeof(CReactionMatch_AsymmetricPyramid));
FABLE_STATIC_ASSERT(sizeof(CReactionMatch_MoralityRenownBlob) == sizeof(CReactionMatch_MRPyramid));

CReactionMatch_MoralityRenownBlob::CReactionMatch_MoralityRenownBlob(EOpinionReactionType type, float yMin, float yMax,
        float xMin, float xMax, float scarinessShift, float agreeablenessShift, float attractivenessShift)
    : CReactionMatch_MRPyramidRetail(type, xMin, (xMax - xMin) * 0.5f + xMin, xMax, yMin, (yMax - yMin) * 0.5f + yMin, yMax,
                               scarinessShift, agreeablenessShift, attractivenessShift)
{
}