// CTCCreatureOpinionOfHero::SerialiseFlagMask(CPersistContext& context, const CCharString& name, std::vector<bool>& mask) -- save the
//   flag mask as one "TF.." string under `name`; on load read it back and, when its length equals the mask's, set each flag from it
// Flags: /O2 /Oy /W3
// Leaf: ?SerialiseFlagMask@CTCCreatureOpinionOfHero@@AAEXAAVCPersistContext@@ABVCCharString@@AAV?$vector@_NV?$allocator@_N@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00681BA0. Called three times from CTCCreatureOpinionOfHero::OnSerialise 0x00684610 with
//   "AttitudeMask" / "ReactionMask" / "DeedMask" (that landed row binds this spelling).
// Names (NF201, 2026-11-20): Ego_r.pdb private ?SerialiseFlagMask@CTCCreatureOpinionOfHero@@AAEXAAVCPersistContext@@ABVCCharString@@AAV?$
//   vector@_NV?$allocator@_N@std@@@std@@@Z (donor 0x0058FBD0), declared in engine/CTCCreatureOpinionOfHero.h (SERIALISE opt-in);
//   callees CCharString(const char*, long) 0x0099EBF0, operator+=(char) 0x0099F180 (Ego_r / FableWin ??YCCharString@@QAEABV0@D@Z, was a
//   stand-in AppendCharacter), CPersistContext::Transfer<CCharString>(name, value, default) 0x004109A0 and Transfer<CCharString>(name,
//   value) 0x00411010. Was the stand-in CPersistTraits::Transfer over local CCharString / CPersistContext / vector models.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_APPEND_CHAR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CPersistContext.h"
#include "engine/CStringParser.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"

void CTCCreatureOpinionOfHero::SerialiseFlagMask(CPersistContext& context, const CCharString& name, std::vector<bool>& mask)
{
    if (context.IsSaving()) {
        CCharString mask_string("");
        for (std::vector<bool>::iterator it = mask.begin(), end = mask.end(); it != end; ++it)
            mask_string += *it ? 'T' : 'F';
        context.Transfer(name.GetAsCharArray(), mask_string);
        return;
    }
    {
        CCharString mask_string("");
        context.Transfer(name.GetAsCharArray(), mask_string);
        if (mask_string.GetLength() > 0 && mask_string.GetLength() == (long)mask.size()) {
            const char* text = mask_string.GetAsCharArray();
            long count = mask.size();
            for (long i = 0; i < count; ++i)
                mask[i] = text[i] == 'T';
        }
    }
}
