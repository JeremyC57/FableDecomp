// CAIStateGroup_OpinionReactionRoot::`dynamic initializer for 'Behaviours'' -- build the 79-entry opinion-reaction behaviour table: per reaction type its update handler, its name ("OPINION_REACTION_TYPE_<X>") and its is-runnable handler, then register the table's destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x01221D20, 7990 bytes: `push ecx` (the converted-name temporary) + 4 saves, 79 x (CCharString
//   (lit, -1) temporary, the two 16-byte pointers to member, Name copy-constructed 0x0099EC30, ~CCharString 0x0099EAE0), then
//   `push 0x0122A460 ; call _atexit` and `ret` at 0x01223C55 (extent proven; the next function starts after it).
// Identity (proven): Ego_r.pdb 0x01017780 and FableWin.pdb 0x040005B0 name `dynamic initializer for
//   'CAIStateGroup_OpinionReactionRoot::Behaviours''; the 40-byte entries at 0x013BB748 are Ego_r.pdb COpinionReactionBehaviour
//   (UpdateFunc, IsRunnableFunc, Name), 79 of them = the Anniversary PDB's const COpinionReactionBehaviour[79] (2844 bytes there at
//   36 bytes each). Handler names: each entry's name literal (rstr) is OPINION_REACTION_TYPE_<X> and its stored function words are
//   ?<X>Update / ?<X>IsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ (Ego_r.pdb + FableWin.pdb); the ten General / ToHerself
//   wife entries share WifeNotWhileJustMarriedIsRunnable 0x00985EF0 and the five Clothing ones WifeClothingIsRunnable 0x00985E90
//   (Ego_r donor order and sizes agree). Full address list in LABELS.tsv.
// Shape: the name argument is the bare literal, converted to the constructor's const CCharString& (FABLE_RETAIL_CCHARSTRING's
//   default length): retail then passes the temporary's own address (`lea eax,[esp+10h]`); an explicit CCharString("...")
//   passes the constructor's return value instead (7562 bytes). The constructor is the inline one of engine/
//   COpinionReactionBehaviour.h; the implicit destructor makes VC7.1 register the atexit thunk 0x0122A460.
#define FABLE_RETAIL_CCHARSTRING
#include "engine/COpinionReactionBehaviour.h"
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_BEHAVIOURS
#include "engine/CAIStateGroup_OpinionReactionRoot.h"

const COpinionReactionBehaviour CAIStateGroup_OpinionReactionRoot::Behaviours[79] = {
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_NONE", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_RESPECT", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_AWE", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_DISDAIN", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_FEAR", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_FRIENDLINESS", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_WORSHIP", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_RIDICULOUS", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_OFFENSIVE", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_AGREEABLE", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_UGLY", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_ATTRACTED", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_LOVE", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_WIFE_LOVE", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_WIFE_LIKE", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_WIFE_NEUTRAL", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_WIFE_DISLIKE", NULL),
    COpinionReactionBehaviour(NULL, "OPINION_REACTION_TYPE_ATTITUDE_WIFE_HATE", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::AngryPointUpdate, "OPINION_REACTION_TYPE_ANGRY_POINT", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::BackAwayUpdate, "OPINION_REACTION_TYPE_BACK_AWAY", &CAIStateGroup_OpinionReactionRoot::BackAwayIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::BellyLaughUpdate, "OPINION_REACTION_TYPE_BELLY_LAUGH", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::BooUpdate, "OPINION_REACTION_TYPE_BOO", &CAIStateGroup_OpinionReactionRoot::BooIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::BowingLargeUpdate, "OPINION_REACTION_TYPE_BOWING_LARGE", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::BowingSmallUpdate, "OPINION_REACTION_TYPE_BOWING_SMALL", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CallingChildrenUpdate, "OPINION_REACTION_TYPE_CALLING_CHILDREN", &CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CheerLargeUpdate, "OPINION_REACTION_TYPE_CHEER_LARGE", &CAIStateGroup_OpinionReactionRoot::CheerLargeIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CheerSmallUpdate, "OPINION_REACTION_TYPE_CHEER_SMALL", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::ClapLargeUpdate, "OPINION_REACTION_TYPE_CLAP_LARGE", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::ClapSmallUpdate, "OPINION_REACTION_TYPE_CLAP_SMALL", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CommentAboutHeroUpdate, "OPINION_REACTION_TYPE_COMMENT_ABOUT_HERO", &CAIStateGroup_OpinionReactionRoot::CommentAboutHeroIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CommentAtHeroUpdate, "OPINION_REACTION_TYPE_COMMENT_AT_HERO", &CAIStateGroup_OpinionReactionRoot::CommentAtHeroIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CommentToSelfUpdate, "OPINION_REACTION_TYPE_COMMENT_TO_SELF", &CAIStateGroup_OpinionReactionRoot::CommentToSelfIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CowerUpdate, "OPINION_REACTION_TYPE_COWER", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::DismissUpdate, "OPINION_REACTION_TYPE_DISMISS", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::FleeUpdate, "OPINION_REACTION_TYPE_FLEE", &CAIStateGroup_OpinionReactionRoot::FleeIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::FollowCloseUpdate, "OPINION_REACTION_TYPE_FOLLOW_CLOSE", &CAIStateGroup_OpinionReactionRoot::FollowCloseIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::FollowFarUpdate, "OPINION_REACTION_TYPE_FOLLOW_FAR", &CAIStateGroup_OpinionReactionRoot::FollowFarIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::FriendlyGreetUpdate, "OPINION_REACTION_TYPE_FRIENDLY_GREET", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::GetOutUpdate, "OPINION_REACTION_TYPE_GET_OUT", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::GrovelLargeUpdate, "OPINION_REACTION_TYPE_GROVEL_LARGE", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::GrovelSmallUpdate, "OPINION_REACTION_TYPE_GROVEL_SMALL", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::HeroImitationPlayUpdate, "OPINION_REACTION_TYPE_HERO_IMITATION_PLAY", &CAIStateGroup_OpinionReactionRoot::HeroImitationPlayIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::HeroTitleAtHeroUpdate, "OPINION_REACTION_TYPE_HEROTITLE_AT_HERO", &CAIStateGroup_OpinionReactionRoot::HeroTitleAtHeroIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::HeroTitleToSelfUpdate, "OPINION_REACTION_TYPE_HEROTITLE_TO_SELF", &CAIStateGroup_OpinionReactionRoot::HeroTitleToSelfIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::HideUpdate, "OPINION_REACTION_TYPE_HIDE", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::MarriageCommentUpdate, "OPINION_REACTION_TYPE_MARRIAGE_COMMENT", &CAIStateGroup_OpinionReactionRoot::MarriageCommentIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::OfferGiftFriendlyUpdate, "OPINION_REACTION_TYPE_OFFER_GIFT_FRIENDLY", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::OfferGiftWorshipUpdate, "OPINION_REACTION_TYPE_OFFER_GIFT_WORSHIP", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::PickFightUpdate, "OPINION_REACTION_TYPE_PICK_FIGHT", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::PointUpdate, "OPINION_REACTION_TYPE_POINT", &CAIStateGroup_OpinionReactionRoot::PointIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::PointLaughUpdate, "OPINION_REACTION_TYPE_POINT_LAUGH", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::RespectfulGreetUpdate, "OPINION_REACTION_TYPE_RESPECTFUL_GREET", &CAIStateGroup_OpinionReactionRoot::RespectfulGreetIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::ShakeFistUpdate, "OPINION_REACTION_TYPE_SHAKE_FIST", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::ShoutAweUpdate, "OPINION_REACTION_TYPE_SHOUT_AWE", &CAIStateGroup_OpinionReactionRoot::ShoutAweIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::SniggerUpdate, "OPINION_REACTION_TYPE_SNIGGER", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::ThumbsDownUpdate, "OPINION_REACTION_TYPE_THUMBS_DOWN", &CAIStateGroup_OpinionReactionRoot::ThumbsDownIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WatchUpdate, "OPINION_REACTION_TYPE_WATCH", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeFeelingLoveUpdate, "OPINION_REACTION_TYPE_WIFE_FEELING_LOVE", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGeneralLoveUpdate, "OPINION_REACTION_TYPE_WIFE_GENERAL_LOVE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeToHerselfLoveUpdate, "OPINION_REACTION_TYPE_WIFE_TOHERSELF_LOVE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeClothingLoveUpdate, "OPINION_REACTION_TYPE_WIFE_CLOTHING_LOVE", &CAIStateGroup_OpinionReactionRoot::WifeClothingIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeFeelingLikeUpdate, "OPINION_REACTION_TYPE_WIFE_FEELING_LIKE", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGeneralLikeUpdate, "OPINION_REACTION_TYPE_WIFE_GENERAL_LIKE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeToHerselfLikeUpdate, "OPINION_REACTION_TYPE_WIFE_TOHERSELF_LIKE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeClothingLikeUpdate, "OPINION_REACTION_TYPE_WIFE_CLOTHING_LIKE", &CAIStateGroup_OpinionReactionRoot::WifeClothingIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeFeelingNeutralUpdate, "OPINION_REACTION_TYPE_WIFE_FEELING_NEUTRAL", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGeneralNeutralUpdate, "OPINION_REACTION_TYPE_WIFE_GENERAL_NEUTRAL", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeToHerselfNeutralUpdate, "OPINION_REACTION_TYPE_WIFE_TOHERSELF_NEUTRAL", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeClothingNeutralUpdate, "OPINION_REACTION_TYPE_WIFE_CLOTHING_NEUTRAL", &CAIStateGroup_OpinionReactionRoot::WifeClothingIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeFeelingDislikeUpdate, "OPINION_REACTION_TYPE_WIFE_FEELING_DISLIKE", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGeneralDislikeUpdate, "OPINION_REACTION_TYPE_WIFE_GENERAL_DISLIKE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeToHerselfDislikeUpdate, "OPINION_REACTION_TYPE_WIFE_TOHERSELF_DISLIKE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeClothingDislikeUpdate, "OPINION_REACTION_TYPE_WIFE_CLOTHING_DISLIKE", &CAIStateGroup_OpinionReactionRoot::WifeClothingIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeFeelingHateUpdate, "OPINION_REACTION_TYPE_WIFE_FEELING_HATE", NULL),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGeneralHateUpdate, "OPINION_REACTION_TYPE_WIFE_GENERAL_HATE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeToHerselfHateUpdate, "OPINION_REACTION_TYPE_WIFE_TOHERSELF_HATE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeClothingHateUpdate, "OPINION_REACTION_TYPE_WIFE_CLOTHING_HATE", &CAIStateGroup_OpinionReactionRoot::WifeClothingIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeJustMarriedUpdate, "OPINION_REACTION_TYPE_WIFE_JUSTMARRIED", &CAIStateGroup_OpinionReactionRoot::WifeJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGiftWantedUpdate, "OPINION_REACTION_TYPE_WIFE_GIFT_WANTED", &CAIStateGroup_OpinionReactionRoot::WifeGiftWantedIsRunnable),
};