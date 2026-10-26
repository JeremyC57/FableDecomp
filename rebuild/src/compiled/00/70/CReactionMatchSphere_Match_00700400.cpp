// CReactionMatch_Sphere::Match(const COpinionSet& opinions) -- 1 - |opinions - Centre|^2 / SqRadius, clamped to [0, 1]
// Leaf: ?Match@CReactionMatch_Sphere@@UAEMABVCOpinionSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00700400, length 161 bytes (virtual, __fastcall, ret 4, float in st0).
// PDB: ?Match@CReactionMatch_Sphere@@UAEMABVCOpinionSet@@@Z. Callees: none (float pool 0x0122DED8 = 1.0f,
// 0x0122DEDC = 0.0f).
// Layout: retail offsets as in the landed TransferBinaryIn 0x00701990 / TransferBinaryOut 0x007003A0 (retail CArray<T>
// is three pointers, so COpinionSet Centre is +0x08..+0x13 and SqRadius +0x14; engine/CReactionMatch_Sphere.h has
// the PDB's +0x18, hence the local model).
// Shape: the squared distance over the five opinion axes is an inline COpinionSet helper with a plain
// `for (i < 5)` loop from a 0.0f sum: VC7.1 unrolls it completely and folds the leading `0 + d*d` (hence the
// `fmulp` first axis), the per-axis difference staying live until `fstp st(0)`. Written out by hand the last
// add is finished in place instead. The result is clamped through an inline reference Clamp(value, lo, hi):
// the literal bounds become frame temporaries (1.0f and 0.0f stored up front), the value is the expression
// itself (`fst` into the dead argument slot; a named result local gives `fstp ; fld`), and the returned
// reference is loaded with `lea eax ; fld [eax]`.
struct CArrayFloat {                                     // retail CArray<float>: begin/end/capacity
    float* m_begin; float* m_end; float* m_capacity;
};

class COpinionSet { public:
    CArrayFloat OpinionValues;                           // +0x00
    __forceinline float GetSqDistance(const COpinionSet& other) const
    {
        float sqDistance = 0.0f;
        for (long i = 0; i < 5; ++i) {
            float d = OpinionValues.m_begin[i] - other.OpinionValues.m_begin[i];
            sqDistance += d * d;
        }
        return sqDistance;
    }
};

class CReactionMatch { public:
    void* __vftable;                                     // +0x00
    long  ReactionType;                                  // +0x04 EOpinionReactionType
};

static __forceinline const float& Clamp(const float& value, const float& lo, const float& hi)
{
    if (value < lo) return lo;
    if (value > hi) return hi;
    return value;
}

struct CReactionMatch_Sphere : CReactionMatch {
    COpinionSet Centre;                                  // +0x08
    float       SqRadius;                                // +0x14
    float Match(const COpinionSet& opinions);
};

float CReactionMatch_Sphere::Match(const COpinionSet& opinions)
{
    return Clamp(1.0f - opinions.GetSqDistance(Centre) / SqRadius, 0.0f, 1.0f);
}