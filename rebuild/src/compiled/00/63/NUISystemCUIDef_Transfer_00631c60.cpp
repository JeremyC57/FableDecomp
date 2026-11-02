// NUISystem::CUIDef::Transfer(CPersistContext& persist) -- persist every UI def field through the one-argument
// CPersistContext::Transfer<T> forms
// Leaf: ?Transfer@CUIDef@NUISystem@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00631C60, 1524 bytes (span ends 0x00632254, `ret 4`; next function 0x00632260), virtual.
// Identity: Ego_r.pdb ?Transfer@CUIDef@NUISystem@@UAEXAAVCPersistContext@@@Z (Ego 0x006758F0) and the FableWin procedure
//   record (argument name `persist`); vtable 0x01259F8C slot 18 (the def-class Transfer slot), RTTI .?AVCUIDef@NUISystem@@.
// Callees (one-argument member templates, the 0x00431C30 CUIIconsDef shape): Transfer<unsigned long> 0x00431020, <float>
//   0x00431061, <long> 0x00431102, <bool> 0x0043314A, <unsigned char> 0x00473593, <CDefString> 0x0045228F,
//   <CWideString> 0x004310A7; TransferVector<unsigned long> 0x00464841, <float> 0x00464CBF, <long> 0x00466A47 (both PDBs
//   instantiate vectors only through ??$TransferVector@K/M/J@CPersistContext, never Transfer<vector>);
//   enum instances <EType> 0x00632260, <ETableExpansionTypes> 0x00632340, <EEngineGraphicType> 0x00632420,
//   <EActionType> 0x00632500, <ETextAlignement> 0x006325E0, <EEngineSprite2DFlag> 0x006326C0; TransferMap<long,ETableSprites>
//   0x006327A0, TransferMap<CCharString,unsigned long> 0x00632AF0, TransferMap<unsigned long,unsigned long> 0x00431242,
//   TransferVectorOfSubComponents<CUIStateDef> 0x00632E00 (template names from the Ego_r keyed instantiations of the same
//   member types; the manifest labels "CPersistContext_TransferFloat" etc. are propagated). CWideString() 0x0099AED0,
//   ~CWideString 0x0099B510.
// Layout: engine/NUISystem_CUIDef.h (cons11 model, composites typed here) with FABLE_RETAIL_C2DVECTOR: PositionOffset is
//   read at +0x142/+0x146, so C2DVector is byte-packed (Ego_r CUIDef+0x15E, COldFileFormatThingBrushInfo+0x11; TPI packed bit).
// Shape: one Transfer per member in retail order (not declaration order: Children before MeshIndex, States after the
//   separations, the action slots in their own order, PositionOffset/AlphaOffset before Up). TextValue's default is the
//   only one with an out-of-line constructor: retail builds a CWideString in the dead `persist` slot ([esp+0Ch]) before
//   that transfer and destroys it right after, so it is a block-scoped default object; MovieFilename has none.
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_C2DVECTOR
#include "engine/NUISystem_CUIDef.h"
#include "fable_persist_context.h"

namespace NUISystem {

void CUIDef::Transfer(CPersistContext& persist)
{
    persist.Transfer(Type);
    persist.TransferVector(Children);
    persist.Transfer(MeshIndex);
    { CWideString defaultTextValue; persist.Transfer(TextValue); }
    persist.Transfer(Font);
    persist.Transfer(Height);
    persist.Transfer(Width);
    persist.Transfer(ExpansionType);
    persist.TransferMap(Sprites);
    persist.TransferVector(HorizontalSeparations);
    persist.TransferVector(VerticalSeparations);
    persist.TransferVectorOfSubComponents(States);
    persist.Transfer(TextLineBreak);
    persist.Transfer(ScaleText);
    persist.Transfer(Independant);
    persist.Transfer(MeshType);
    persist.TransferVector(NonScrollingChildren);
    persist.Transfer(TextWindow.TLX);
    persist.Transfer(TextWindow.TLY);
    persist.Transfer(TextWindow.BRX);
    persist.Transfer(TextWindow.BRY);
    persist.Transfer(Layer);
    persist.Transfer(Angle);
    persist.Transfer(PositionIsCenter);
    persist.Transfer(ScrollingSpeed);
    persist.Transfer(Wrapping);
    persist.Transfer(Inverted);
    persist.Transfer(PositionOffset.X);
    persist.Transfer(PositionOffset.Y);
    persist.Transfer(AlphaOffset);
    persist.Transfer(Up.X);
    persist.Transfer(Up.Y);
    persist.Transfer(Up.Z);
    persist.Transfer(Forward.X);
    persist.Transfer(Forward.Y);
    persist.Transfer(Forward.Z);
    persist.Transfer(RotationAxis.X);
    persist.Transfer(RotationAxis.Y);
    persist.Transfer(RotationAxis.Z);
    persist.Transfer(RotationSpeed);
    persist.Transfer(AnimationIndex);
    persist.Transfer(DownArrow);
    persist.Transfer(UpArrow);
    persist.Transfer(UpLimit);
    persist.Transfer(DownLimit);
    persist.Transfer(Scrolling);
    persist.Transfer(ComputeOffsetsOnActivate);
    persist.Transfer(Min.X);
    persist.Transfer(Min.Y);
    persist.Transfer(Max.X);
    persist.Transfer(Max.Y);
    persist.Transfer(Step.X);
    persist.Transfer(Step.Y);
    persist.Transfer(Dimensions.X);
    persist.Transfer(Dimensions.Y);
    persist.Transfer(SliderLeft);
    persist.Transfer(SliderRight);
    persist.Transfer(Action);
    persist.Transfer(ActionOnBack);
    persist.Transfer(ActionOnSelected);
    persist.Transfer(ActionOnUnselected);
    persist.Transfer(ActionOnDestruction);
    persist.Transfer(ActionOnLeftClicked);
    persist.Transfer(ActionOnLeftUnclicked);
    persist.Transfer(ActionOnLeftHeld);
    persist.Transfer(ActionOnRightClicked);
    persist.Transfer(ActionOnDropped);
    persist.Transfer(ActionOnDroppedNowhere);
    persist.Transfer(PreAction);
    persist.Transfer(ActionOnDraggedUp);
    persist.Transfer(ActionOnDraggedDown);
    persist.Transfer(ActionOnLeftClickedAbove);
    persist.Transfer(ActionOnLeftClickedUnder);
    persist.Transfer(InputDelay);
    persist.Transfer(DrawFromViewport);
    persist.Transfer(TextBankIndex);
    persist.Transfer(ActionText);
    persist.Transfer(KeyText);
    persist.Transfer(Redefiner);
    persist.Transfer(UndefinedWarning);
    persist.TransferMap(ActionMap);
    persist.TransferMap(ActionMapAliases);
    persist.TransferVector(ActionOrder);
    persist.Transfer(EditBoxParentIsButton);
    persist.Transfer(PasswordBox);
    persist.Transfer(EditBoxCharLimit);
    persist.Transfer(EditBoxUsesIME);
    persist.Transfer(MovieFilename);
    persist.Transfer(DisallowSpaceAsFirstChar);
    persist.Transfer(LayerIndependant);
    persist.TransferVector(SwappingStates);
    persist.TransferVector(SwappingTimes);
    persist.Transfer(BastardChild);
    persist.Transfer(Alignement);
    persist.Transfer(RandomSwap);
    persist.Transfer(UseRelativeZoom);
    persist.Transfer(UseRelativePosition);
    persist.Transfer(HoveredState);
    persist.Transfer(LeftClickedState);
    persist.Transfer(RightClickedState);
    persist.TransferVector(ShapeChildren);
    persist.Transfer(ViewArea.TLX);
    persist.Transfer(ViewArea.TLY);
    persist.Transfer(ViewArea.BRX);
    persist.Transfer(ViewArea.BRY);
    persist.Transfer(UseViewArea);
    persist.Transfer(PartOfListTree);
    persist.Transfer(PCStyle);
    persist.Transfer(Sprite2DFlag);
}

}