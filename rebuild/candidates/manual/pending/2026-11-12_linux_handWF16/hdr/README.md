## 0x007E4E30

- engine/CTCBetting.h: FABLE_RETAIL_CTCBETTING_FRAMEUPDATE declares virtual FrameUpdate (slot 9, 0x007E4E30).

## 0x007E8590

engine/CThing.h: FABLE_RETAIL_CTHING_SENDMESSAGEW -- declares SendMessageW(const NTCMessages::CMessageBase&) 0x004C7820.
engine/CTCThingOwner.h: FABLE_RETAIL_CTCTHINGOWNER_OWNEDOBJECTS -- private AddOwnedObject/RemoveOwnedObject(CThing&) + friend CTCOwnedEntity.
fable_string.h: FABLE_RETAIL_CCHARSTRING_STARTSWITHCASEINSENSITIVE -- inline StartsWithCaseInsensitive via _strnicmp.
engine/CThing.h: FABLE_RETAIL_CTHING_GETSCRIPTDATA -- inline GetScriptData() (ScriptData +0x78).

## 0x00922010

(no overlay needed: the matching source uses only repo headers; experimental opt-ins tried during authoring were removed)

## 0x00AF5090

engine/CParticle.h: FABLE_RETAIL_CPARTICLE_CLEARPARTICLE -- inline FableWin.pdb accessors GetParentHandle / GetPrimID (when SPRITE is off) / ClearParentHandle (FABLE_FORCEINLINE) / ClearAll (row 0x00AF5090), the CDecal twins of FABLE_RETAIL_CDECAL_CLEAR.

## 0x009F3B00

- engine/CSurface.h: FABLE_RETAIL_CSURFACE_LOCK -- Lock decl + inline Unlock + out-of-line GetByteLength decl (row 0x009F3B00).

## 0x009735D0

engine/CCreatureAction_CastAssassinRush.h: FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH -- retail class on CCombatActionBase, #pragma pack(1) as retail (Override +0x114, Dir +0x115, pad +0x121, Distance, Level, FinalAction, UseCollision, IsZTargetting; sizeof 0x138) for row 0x009735D0
engine/CAIStateGroup_JackOfBladesBase.h: FABLE_RETAIL_JACKOFBLADESBASE_RUSHTOPOS -- declares RushToPos without FABLE_RETAIL_JACK_STAGE1_UPDATE (row 0x009735D0)
engine/CTCSolidToThing.h: FABLE_RETAIL_CTCSOLIDTOTHING_REMOVETHINGNOTSOLIDTO -- declares RemoveThingNotSolidTo 0x00770F20 (row 0x009735D0)
engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX -- Normalise sums (Z*Z)+(Y*Y)+(X*X) (row 0x009735D0). The Normalise #if/#elif chain is re-headed: the new `#if defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX)` branch goes first and the repo's former head line `#if defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ)` (line 355) becomes `#elif`; that is the only repo line changed, and the default (no opt-in) path is unchanged.
- (coordinator, at landing) The ctor's first parameter is CThingCreatureBase& as both PDBs spell it (was a pointer); the row passes GetCreature(). Regraded: see the landing README. NAME_FIXES hands back the landed 0x008CE1B0 leaf.
- (coordinator, at landing) engine/C3DVector.h: the Normalise chain's first line `#if defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ)` becomes `#if defined(..._PAREN_SQUARES_ZYX) ... #elif defined(..._LOCALS_XYZ)`: one rewritten #if line, needed to put the new arm first; default and every existing opt-in path are token-identical (hdrcheck2).

## 0x00803D60

FABLE_RETAIL_NCAMERAMODES_CBASE_LOOKATPHASE_INLINE (engine/NCameraModes_CBase.h): GetLookAtPhase() const/non-const inline returning LookAt (row 0x00803D60 reads mode+0x10 in place).
FABLE_RETAIL_CGAMECAMERAMANAGER_GETCAMERA (engine/CGameCameraManager.h, LOOKAROUND class): inline GetCamera() / const (FableWin ?GetCamera@CGameCameraManager), expanded in row 0x00803D60.
FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_PROJECTILE (engine/CTCTargetingPlayer.h): declares protected UpdateProjectileTargeting() (Ego_r 0x007FF010 / FableWin 0x024CF520).

## 0x006D8F10 (not landed)

# Header overlay for row 0x006D8F10 (full copies of the current repo headers plus opt-ins; default paths unchanged)
- engine/CHeroExperienceDef.h: FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER -- real STLport member types (vector<long> x3, map<long,CDefString> x5, map<long,CComboMultiplierData>, map<EDamageAttribute,CHeroStatIncreaseData>, vector<CAttackHistoryCombo>) and the slot-18 `virtual void Transfer(CPersistContext&)` (Ego_r + FableWin ?Transfer@CHeroExperienceDef@@UAEXAAVCPersistContext@@@Z). Needs FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CATTACKHISTORYCOMBO, FABLE_RETAIL_CDEFSTRINGTABLE, FABLE_RETAIL_CCHARSTRING.
- engine/CPersistContext.h: FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERMAP_BINARY_BODY -- generic body of the one-argument TransferMap<V,K>(map&) (CheckCRC(""), switch 2 In / 3 Out), the shape of the landed out-of-line copies (0x0046FA22 etc.).
- engine/CPersistTraits.h: FABLE_RETAIL_CPERSISTTRAITS_FLOAT_BINARYOUT -- inline CPersistTraits<float>::TransferBinaryOut -> CDataOutputStream::WriteFloat 0x00993EE0 (retail 0x006D9002).

