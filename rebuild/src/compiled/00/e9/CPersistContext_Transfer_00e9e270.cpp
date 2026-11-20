// CPersistContext::Transfer<NScript::CV_IntroductionToTrophiesScript::BriarRoseOpinion>(const char* name, BriarRoseOpinion& value, const BriarRoseOpinion& defaultValue) -- persist one enum member ("OpinionCurrent"): CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save as a long, or reset to the default
// Leaf: ??$Transfer@W4BriarRoseOpinion@CV_IntroductionToTrophiesScript@NScript@@@CPersistContext@@QAEXPBDAAW4BriarRoseOpinion@CV_IntroductionToTrophiesScript@NScript@@ABW4123@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9E270, `ret 0Ch`. Called from the quest-script persist function at 0x00E9E110 with the key "OpinionCurrent".
// NF162 (2026-11-20, q11758): was CPersistContext::Transfer<EOpinionCurrent> on a descriptive global enum and row-local CPersistContext / CPersistTraits /
//   CPersistContext_TransferableComponent / CDataOutputStream models (old file CPersistContext_TransferEOpinionCurrent_00e9e270.cpp). Identity: Ego_r.pdb
//   0x00F05040 + FableWin.pdb 0x03B52750 ??$Transfer@W4BriarRoseOpinion@CV_IntroductionToTrophiesScript@NScript@@@CPersistContext@@... (this exact symbol);
//   the landed caller 0x00E9E110 binds it (exe scan: its only referencer); the field is CV_IntroductionToTrophiesScript::OpinionCurrent (+0x58, BriarRoseOpinion,
//   engine/CV_IntroductionToTrophiesScript.h); the text arms store the component vtable 0x012ED7D4 whose slots are the FableWin.pdb TransferOut / TransferIn of
//   this T (0x00E9E400 / 0x00E9E3E0).
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0 (the traits' inline TransferBinaryOut),
//   CPersistTraits<T>::TransferBinaryIn 0x00E9E350 (FableWin.pdb ?TransferBinaryIn@?$CPersistTraits@W4BriarRoseOpinion@...@@QAEXAAVCMemoryDataInputStream@@AAW4...@Z),
//   CPersistContext::TransferObjectSaveText 0x009BAD60 and TransferObjectLoadText 0x009BBA40.
// Shape: the speed-optimised sibling of the landed size-TU CDefIndex instantiation 0x005CC1E8. The persist mode (CPersistContext +0x18) is a closed enum, so
//   the switch ends in an unreachable default and VC7.1 emits the bare `jmp [eax*4+table]`; the case bodies are laid out in source order and retail has
//   them 4,3,2,1,0. The traits object (DefaultVal zeroed) is declared at function scope: it is built once before the jump, in the dead key-argument slot,
//   and the binary-load arm passes its address. The 8-byte frame is the {vtable, value*} wrapper of the text arms.
// Inferred: the unreachable default (`__assume(0)`, guarded as engine/CPersistContext.h writes its switch defaults) is the landed row's form at HEAD:
//   retail has no range check before the jump table at 0x00E9E270 (the same jump-table shape as the landed three-argument Transfer<long> 0x00684610 family).
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_DEFAULTVAL_COPYINIT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_NSCRIPT
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
#include "engine/CV_IntroductionToTrophiesScript.h"

template <class T>
void CPersistContext::Transfer(const char* name, T& value, const T& defaultValue)
{
    CheckCRC(name);
    CPersistTraits<T> traits;
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
#ifdef _MSC_VER
        __assume(0);
#elif defined(__GNUC__)
        __builtin_unreachable();
#endif
        break;
    }
}

template void CPersistContext::Transfer<NScript::CV_IntroductionToTrophiesScript::BriarRoseOpinion>(const char* name, NScript::CV_IntroductionToTrophiesScript::BriarRoseOpinion& value, const NScript::CV_IntroductionToTrophiesScript::BriarRoseOpinion& defaultValue);
