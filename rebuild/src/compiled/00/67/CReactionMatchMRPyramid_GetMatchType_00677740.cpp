// Constant-return member: returns 0x3. __fastcall this=ecx.
// Leaf: ?GetMatchType@CReactionMatch_MRPyramid@@UBE?AW4EReactionMatchType@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CReactionMatch_MRPyramid.h (class-model switch FABLE_RETAIL_CREACTIONMATCH_MRPYRAMID_CLASS; was ?GetMatchType@CReactionMatch_MRPyramid_Methods@@QAEHXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CPersistTraitsstdlistCReactionMatch_TransferIn_00678760.cpp.
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_PEEKPSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_FINDSYMBOL
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#define FABLE_RETAIL_COPINIONSET_CTOR_INDEX
#define FABLE_RETAIL_CREACTIONMATCH_SPHERE_CLASS
#define FABLE_RETAIL_CREACTIONMATCH_ASYMMETRICPYRAMID_CLASS
#define FABLE_RETAIL_CREACTIONMATCH_MRPYRAMID_CLASS
#define FABLE_RETAIL_CREACTIONMATCH_SECONDARYAXISMRSHIFTED_CLASS
#define FABLE_RETAIL_CREACTIONMATCH_SECONDARYAXISCENTREDMRSHIFTED_CLASS
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CReactionMatch.h"
#include "engine/CReactionMatch_Sphere.h"
#include "engine/CReactionMatch_MoralityRenownBlob.h"
#include "engine/CReactionMatch_AsymmetricPyramid.h"
#include "engine/CReactionMatch_SecondaryAxisMRShifted.h"
#include "engine/CReactionMatch_SecondaryAxisCentredMRShifted.h"
#include "engine/CReactionMatch_MRPyramid.h"  // retyped onto the PDB layout; byte parity re-verified
EReactionMatchType CReactionMatch_MRPyramid::GetMatchType() const { return (EReactionMatchType)0x3; }
