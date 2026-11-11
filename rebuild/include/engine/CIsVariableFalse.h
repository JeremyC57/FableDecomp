#ifndef FABLE_ENGINE_CISVARIABLEFALSE_H
#define FABLE_ENGINE_CISVARIABLEFALSE_H
#include "engine/CPredicateBase.h"
// Retail model (Codex pass 21): Ego_r and FableWin TPI prove an 8-byte class,
// private const bool& Variable at +4, public const virtual operator()/Clone.
// Native vtable012EA43C, operator00E7DDD0, Clone00E7DDE0 and deleting destructor
// 00E7DE10 independently establish layout and slots. Reference binding is original.
namespace NScript {
class CIsVariableFalse : public CPredicateBase {
public:
    CIsVariableFalse(const bool& variable) : Variable(variable) {}
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    virtual ~CIsVariableFalse() {}
private:
    const bool& Variable;
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CIsVariableFalse) == 8);
#endif
