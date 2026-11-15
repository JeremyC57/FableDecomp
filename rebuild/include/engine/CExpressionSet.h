#ifndef FABLE_ENGINE_CEXPRESSIONSET_H
#define FABLE_ENGINE_CEXPRESSIONSET_H
#include "rebuild_abi.h"
#include "engine/CVectorMap.h"
// Original enum values independently recovered from Ego_r and FableWin raw TPI.
enum EExpressionType {
    EXPRESSION_NULL = 0,
    EXPRESSION_DEFAULT = 1,
    EXPRESSION_FEAR = 2,
    EXPRESSION_HAPPY = 3,
    EXPRESSION_CONFUSED = 4,
    EXPRESSION_FURTIVE = 5,
    EXPRESSION_SERIOUS = 6,
    EXPRESSION_SQUINT = 7,
    EXPRESSION_EVIL = 8,
    EXPRESSION_CONTENT = 9
};
class CMemoryDataInputStream;
class CDataOutputStream;
class CExpressionSet {
public:
    class CEntry {
    public:
        EExpressionType Type;
        long AnimIndex;
    };
    void TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryOut(CDataOutputStream&) const;
#ifdef FABLE_RETAIL_CPERSIST_ORIGINAL_CLASS_TYPES_PASS22
    // Both full PDB classes prove the original alias and protected field. LF_NESTTYPE does not separately encode alias access.
    typedef CVectorMap<EExpressionType,CEntry> TExpressionMap;
protected:
    TExpressionMap Expressions;
#else
private:
    CVectorMap<EExpressionType,CEntry> Expressions;
#endif
};
FABLE_STATIC_ASSERT_AT(32, sizeof(CExpressionSet)==0x10);
#endif
