#ifndef FABLE_ENGINE_TRET_H
#define FABLE_ENGINE_TRET_H
#include "rebuild_abi.h"
// TRet<T>: the generated-script failure-return holder (twin of engine/TRetVoid.h, original script_language.hpp).
// Layout: Ego_r.pdb TPI `TRet<long>` 4 / `TRet<bool>` 1 / `TRet<CScriptThing>` 12 / `TRet<EMusicSetType>` 4, one member `Val`.
// Members (FableWin.pdb name set): `??0?$TRet@VCCharString@@@@QAE@ABVCCharString@@@Z` (ctor from const T&),
// `??1?$TRet@VCCharString@@@@QAE@XZ` (user dtor), `?Return@?$TRet@VCCharString@@@@QAE?AVCCharString@@XZ` (T by value);
// the same three for CScriptThing, bool, long, int, float, EMusicSetType. Retail Fable.exe has no out-of-line copy of the
// ctor / dtor (0x00F21590 GetEndRoundQuestion expands them: CCharString copy ctor 0x0099EC30, ~CCharString 0x0099EAE0).
// A generated script function that returns T opens with `TRet<T> _Fail_Val(<default>);` and leaves through
// `return _Fail_Val.Return();` when its thread is terminating.
template <class T>
class TRet {
public:
    T Val;
    TRet(const T& val) : Val(val) { }
    ~TRet() { }
    T Return() { return Val; }
};
#endif
