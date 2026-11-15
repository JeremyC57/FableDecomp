// Behaviour test for 0x01221D20 CAIStateGroup_OpinionReactionRoot::`dynamic initializer for 'Behaviours'': the 79-entry reaction table.
// Each entry's name must be OPINION_REACTION_TYPE_<X> (the expected list is independent of the table), its update / is-runnable
// pointers must call the expected handler on a reaction root (the handler returns its own code), the absent ones must be null in
// VC7.1's 16-byte form ({0,0,0,-1}, the words retail stores), and the entry is 40 bytes (8-aligned pointers to member).
#include <cstdio>
#include <cstring>
#include <cstdlib>
class CCharString { public: char* Text;
  CCharString(const char* text, long length = -1) { (void)length; Text = (char*)std::malloc(std::strlen(text) + 1); std::strcpy(Text, text); }
  CCharString(const CCharString& o) { Text = (char*)std::malloc(std::strlen(o.Text) + 1); std::strcpy(Text, o.Text); }
  ~CCharString() { std::free(Text); } };
class CAIStateGroup_OpinionReactionRoot;
class COpinionReactionBehaviour { public:
  COpinionReactionBehaviour(bool (CAIStateGroup_OpinionReactionRoot::*updateFunc)(), const CCharString& name, bool (CAIStateGroup_OpinionReactionRoot::*isRunnableFunc)())
    : UpdateFunc(updateFunc), IsRunnableFunc(isRunnableFunc), Name(name) {}
  bool (CAIStateGroup_OpinionReactionRoot::*UpdateFunc)();
  bool (CAIStateGroup_OpinionReactionRoot::*IsRunnableFunc)();
  CCharString Name; };
class CAIStateGroup_OpinionReactionRoot { friend int main();
  int LastCode, Calls; bool Result;
  CAIStateGroup_OpinionReactionRoot() : LastCode(0), Calls(0), Result(true) {}
  bool Hit(int code) { LastCode = code; ++Calls; Result = !Result; return Result; }
  bool AngryPointUpdate() { return Hit(1); }
  bool BackAwayIsRunnable() { return Hit(2); }
  bool BackAwayUpdate() { return Hit(3); }
  bool BellyLaughUpdate() { return Hit(4); }
  bool BooIsRunnable() { return Hit(5); }
  bool BooUpdate() { return Hit(6); }
  bool BowingLargeUpdate() { return Hit(7); }
  bool BowingSmallUpdate() { return Hit(8); }
  bool CallingChildrenIsRunnable() { return Hit(9); }
  bool CallingChildrenUpdate() { return Hit(10); }
  bool CheerLargeIsRunnable() { return Hit(11); }
  bool CheerLargeUpdate() { return Hit(12); }
  bool CheerSmallUpdate() { return Hit(13); }
  bool ClapLargeUpdate() { return Hit(14); }
  bool ClapSmallUpdate() { return Hit(15); }
  bool CommentAboutHeroIsRunnable() { return Hit(16); }
  bool CommentAboutHeroUpdate() { return Hit(17); }
  bool CommentAtHeroIsRunnable() { return Hit(18); }
  bool CommentAtHeroUpdate() { return Hit(19); }
  bool CommentToSelfIsRunnable() { return Hit(20); }
  bool CommentToSelfUpdate() { return Hit(21); }
  bool CowerUpdate() { return Hit(22); }
  bool DismissUpdate() { return Hit(23); }
  bool FleeIsRunnable() { return Hit(24); }
  bool FleeUpdate() { return Hit(25); }
  bool FollowCloseIsRunnable() { return Hit(26); }
  bool FollowCloseUpdate() { return Hit(27); }
  bool FollowFarIsRunnable() { return Hit(28); }
  bool FollowFarUpdate() { return Hit(29); }
  bool FriendlyGreetUpdate() { return Hit(30); }
  bool GetOutUpdate() { return Hit(31); }
  bool GrovelLargeUpdate() { return Hit(32); }
  bool GrovelSmallUpdate() { return Hit(33); }
  bool HeroImitationPlayIsRunnable() { return Hit(34); }
  bool HeroImitationPlayUpdate() { return Hit(35); }
  bool HeroTitleAtHeroIsRunnable() { return Hit(36); }
  bool HeroTitleAtHeroUpdate() { return Hit(37); }
  bool HeroTitleToSelfIsRunnable() { return Hit(38); }
  bool HeroTitleToSelfUpdate() { return Hit(39); }
  bool HideUpdate() { return Hit(40); }
  bool MarriageCommentIsRunnable() { return Hit(41); }
  bool MarriageCommentUpdate() { return Hit(42); }
  bool OfferGiftFriendlyUpdate() { return Hit(43); }
  bool OfferGiftWorshipUpdate() { return Hit(44); }
  bool PickFightUpdate() { return Hit(45); }
  bool PointIsRunnable() { return Hit(46); }
  bool PointLaughUpdate() { return Hit(47); }
  bool PointUpdate() { return Hit(48); }
  bool RespectfulGreetIsRunnable() { return Hit(49); }
  bool RespectfulGreetUpdate() { return Hit(50); }
  bool ShakeFistUpdate() { return Hit(51); }
  bool ShoutAweIsRunnable() { return Hit(52); }
  bool ShoutAweUpdate() { return Hit(53); }
  bool SniggerUpdate() { return Hit(54); }
  bool ThumbsDownIsRunnable() { return Hit(55); }
  bool ThumbsDownUpdate() { return Hit(56); }
  bool WatchUpdate() { return Hit(57); }
  bool WifeClothingDislikeUpdate() { return Hit(58); }
  bool WifeClothingHateUpdate() { return Hit(59); }
  bool WifeClothingIsRunnable() { return Hit(60); }
  bool WifeClothingLikeUpdate() { return Hit(61); }
  bool WifeClothingLoveUpdate() { return Hit(62); }
  bool WifeClothingNeutralUpdate() { return Hit(63); }
  bool WifeFeelingDislikeUpdate() { return Hit(64); }
  bool WifeFeelingHateUpdate() { return Hit(65); }
  bool WifeFeelingLikeUpdate() { return Hit(66); }
  bool WifeFeelingLoveUpdate() { return Hit(67); }
  bool WifeFeelingNeutralUpdate() { return Hit(68); }
  bool WifeGeneralDislikeUpdate() { return Hit(69); }
  bool WifeGeneralHateUpdate() { return Hit(70); }
  bool WifeGeneralLikeUpdate() { return Hit(71); }
  bool WifeGeneralLoveUpdate() { return Hit(72); }
  bool WifeGeneralNeutralUpdate() { return Hit(73); }
  bool WifeGiftWantedIsRunnable() { return Hit(74); }
  bool WifeGiftWantedUpdate() { return Hit(75); }
  bool WifeJustMarriedIsRunnable() { return Hit(76); }
  bool WifeJustMarriedUpdate() { return Hit(77); }
  bool WifeNotWhileJustMarriedIsRunnable() { return Hit(78); }
  bool WifeToHerselfDislikeUpdate() { return Hit(79); }
  bool WifeToHerselfHateUpdate() { return Hit(80); }
  bool WifeToHerselfLikeUpdate() { return Hit(81); }
  bool WifeToHerselfLoveUpdate() { return Hit(82); }
  bool WifeToHerselfNeutralUpdate() { return Hit(83); }
  static const COpinionReactionBehaviour Behaviours[79];
};
const COpinionReactionBehaviour CAIStateGroup_OpinionReactionRoot::Behaviours[79] = {
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_NONE", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_RESPECT", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_AWE", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_DISDAIN", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_FEAR", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_FRIENDLINESS", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_WORSHIP", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_RIDICULOUS", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_OFFENSIVE", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_AGREEABLE", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_UGLY", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_ATTRACTED", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_LOVE", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_WIFE_LOVE", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_WIFE_LIKE", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_WIFE_NEUTRAL", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_WIFE_DISLIKE", 0),
    COpinionReactionBehaviour(0, "OPINION_REACTION_TYPE_ATTITUDE_WIFE_HATE", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::AngryPointUpdate, "OPINION_REACTION_TYPE_ANGRY_POINT", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::BackAwayUpdate, "OPINION_REACTION_TYPE_BACK_AWAY", &CAIStateGroup_OpinionReactionRoot::BackAwayIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::BellyLaughUpdate, "OPINION_REACTION_TYPE_BELLY_LAUGH", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::BooUpdate, "OPINION_REACTION_TYPE_BOO", &CAIStateGroup_OpinionReactionRoot::BooIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::BowingLargeUpdate, "OPINION_REACTION_TYPE_BOWING_LARGE", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::BowingSmallUpdate, "OPINION_REACTION_TYPE_BOWING_SMALL", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CallingChildrenUpdate, "OPINION_REACTION_TYPE_CALLING_CHILDREN", &CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CheerLargeUpdate, "OPINION_REACTION_TYPE_CHEER_LARGE", &CAIStateGroup_OpinionReactionRoot::CheerLargeIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CheerSmallUpdate, "OPINION_REACTION_TYPE_CHEER_SMALL", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::ClapLargeUpdate, "OPINION_REACTION_TYPE_CLAP_LARGE", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::ClapSmallUpdate, "OPINION_REACTION_TYPE_CLAP_SMALL", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CommentAboutHeroUpdate, "OPINION_REACTION_TYPE_COMMENT_ABOUT_HERO", &CAIStateGroup_OpinionReactionRoot::CommentAboutHeroIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CommentAtHeroUpdate, "OPINION_REACTION_TYPE_COMMENT_AT_HERO", &CAIStateGroup_OpinionReactionRoot::CommentAtHeroIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CommentToSelfUpdate, "OPINION_REACTION_TYPE_COMMENT_TO_SELF", &CAIStateGroup_OpinionReactionRoot::CommentToSelfIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::CowerUpdate, "OPINION_REACTION_TYPE_COWER", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::DismissUpdate, "OPINION_REACTION_TYPE_DISMISS", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::FleeUpdate, "OPINION_REACTION_TYPE_FLEE", &CAIStateGroup_OpinionReactionRoot::FleeIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::FollowCloseUpdate, "OPINION_REACTION_TYPE_FOLLOW_CLOSE", &CAIStateGroup_OpinionReactionRoot::FollowCloseIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::FollowFarUpdate, "OPINION_REACTION_TYPE_FOLLOW_FAR", &CAIStateGroup_OpinionReactionRoot::FollowFarIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::FriendlyGreetUpdate, "OPINION_REACTION_TYPE_FRIENDLY_GREET", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::GetOutUpdate, "OPINION_REACTION_TYPE_GET_OUT", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::GrovelLargeUpdate, "OPINION_REACTION_TYPE_GROVEL_LARGE", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::GrovelSmallUpdate, "OPINION_REACTION_TYPE_GROVEL_SMALL", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::HeroImitationPlayUpdate, "OPINION_REACTION_TYPE_HERO_IMITATION_PLAY", &CAIStateGroup_OpinionReactionRoot::HeroImitationPlayIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::HeroTitleAtHeroUpdate, "OPINION_REACTION_TYPE_HEROTITLE_AT_HERO", &CAIStateGroup_OpinionReactionRoot::HeroTitleAtHeroIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::HeroTitleToSelfUpdate, "OPINION_REACTION_TYPE_HEROTITLE_TO_SELF", &CAIStateGroup_OpinionReactionRoot::HeroTitleToSelfIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::HideUpdate, "OPINION_REACTION_TYPE_HIDE", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::MarriageCommentUpdate, "OPINION_REACTION_TYPE_MARRIAGE_COMMENT", &CAIStateGroup_OpinionReactionRoot::MarriageCommentIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::OfferGiftFriendlyUpdate, "OPINION_REACTION_TYPE_OFFER_GIFT_FRIENDLY", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::OfferGiftWorshipUpdate, "OPINION_REACTION_TYPE_OFFER_GIFT_WORSHIP", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::PickFightUpdate, "OPINION_REACTION_TYPE_PICK_FIGHT", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::PointUpdate, "OPINION_REACTION_TYPE_POINT", &CAIStateGroup_OpinionReactionRoot::PointIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::PointLaughUpdate, "OPINION_REACTION_TYPE_POINT_LAUGH", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::RespectfulGreetUpdate, "OPINION_REACTION_TYPE_RESPECTFUL_GREET", &CAIStateGroup_OpinionReactionRoot::RespectfulGreetIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::ShakeFistUpdate, "OPINION_REACTION_TYPE_SHAKE_FIST", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::ShoutAweUpdate, "OPINION_REACTION_TYPE_SHOUT_AWE", &CAIStateGroup_OpinionReactionRoot::ShoutAweIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::SniggerUpdate, "OPINION_REACTION_TYPE_SNIGGER", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::ThumbsDownUpdate, "OPINION_REACTION_TYPE_THUMBS_DOWN", &CAIStateGroup_OpinionReactionRoot::ThumbsDownIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WatchUpdate, "OPINION_REACTION_TYPE_WATCH", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeFeelingLoveUpdate, "OPINION_REACTION_TYPE_WIFE_FEELING_LOVE", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGeneralLoveUpdate, "OPINION_REACTION_TYPE_WIFE_GENERAL_LOVE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeToHerselfLoveUpdate, "OPINION_REACTION_TYPE_WIFE_TOHERSELF_LOVE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeClothingLoveUpdate, "OPINION_REACTION_TYPE_WIFE_CLOTHING_LOVE", &CAIStateGroup_OpinionReactionRoot::WifeClothingIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeFeelingLikeUpdate, "OPINION_REACTION_TYPE_WIFE_FEELING_LIKE", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGeneralLikeUpdate, "OPINION_REACTION_TYPE_WIFE_GENERAL_LIKE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeToHerselfLikeUpdate, "OPINION_REACTION_TYPE_WIFE_TOHERSELF_LIKE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeClothingLikeUpdate, "OPINION_REACTION_TYPE_WIFE_CLOTHING_LIKE", &CAIStateGroup_OpinionReactionRoot::WifeClothingIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeFeelingNeutralUpdate, "OPINION_REACTION_TYPE_WIFE_FEELING_NEUTRAL", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGeneralNeutralUpdate, "OPINION_REACTION_TYPE_WIFE_GENERAL_NEUTRAL", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeToHerselfNeutralUpdate, "OPINION_REACTION_TYPE_WIFE_TOHERSELF_NEUTRAL", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeClothingNeutralUpdate, "OPINION_REACTION_TYPE_WIFE_CLOTHING_NEUTRAL", &CAIStateGroup_OpinionReactionRoot::WifeClothingIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeFeelingDislikeUpdate, "OPINION_REACTION_TYPE_WIFE_FEELING_DISLIKE", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGeneralDislikeUpdate, "OPINION_REACTION_TYPE_WIFE_GENERAL_DISLIKE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeToHerselfDislikeUpdate, "OPINION_REACTION_TYPE_WIFE_TOHERSELF_DISLIKE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeClothingDislikeUpdate, "OPINION_REACTION_TYPE_WIFE_CLOTHING_DISLIKE", &CAIStateGroup_OpinionReactionRoot::WifeClothingIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeFeelingHateUpdate, "OPINION_REACTION_TYPE_WIFE_FEELING_HATE", 0),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGeneralHateUpdate, "OPINION_REACTION_TYPE_WIFE_GENERAL_HATE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeToHerselfHateUpdate, "OPINION_REACTION_TYPE_WIFE_TOHERSELF_HATE", &CAIStateGroup_OpinionReactionRoot::WifeNotWhileJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeClothingHateUpdate, "OPINION_REACTION_TYPE_WIFE_CLOTHING_HATE", &CAIStateGroup_OpinionReactionRoot::WifeClothingIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeJustMarriedUpdate, "OPINION_REACTION_TYPE_WIFE_JUSTMARRIED", &CAIStateGroup_OpinionReactionRoot::WifeJustMarriedIsRunnable),
    COpinionReactionBehaviour(&CAIStateGroup_OpinionReactionRoot::WifeGiftWantedUpdate, "OPINION_REACTION_TYPE_WIFE_GIFT_WANTED", &CAIStateGroup_OpinionReactionRoot::WifeGiftWantedIsRunnable),
};
static const char* const ExpectedName[79] = { "NONE", "ATTITUDE_RESPECT", "ATTITUDE_AWE", "ATTITUDE_DISDAIN", "ATTITUDE_FEAR", "ATTITUDE_FRIENDLINESS", "ATTITUDE_WORSHIP", "ATTITUDE_RIDICULOUS", "ATTITUDE_OFFENSIVE", "ATTITUDE_AGREEABLE", "ATTITUDE_UGLY", "ATTITUDE_ATTRACTED", "ATTITUDE_LOVE", "ATTITUDE_WIFE_LOVE", "ATTITUDE_WIFE_LIKE", "ATTITUDE_WIFE_NEUTRAL", "ATTITUDE_WIFE_DISLIKE", "ATTITUDE_WIFE_HATE", "ANGRY_POINT", "BACK_AWAY", "BELLY_LAUGH", "BOO", "BOWING_LARGE", "BOWING_SMALL", "CALLING_CHILDREN", "CHEER_LARGE", "CHEER_SMALL", "CLAP_LARGE", "CLAP_SMALL", "COMMENT_ABOUT_HERO", "COMMENT_AT_HERO", "COMMENT_TO_SELF", "COWER", "DISMISS", "FLEE", "FOLLOW_CLOSE", "FOLLOW_FAR", "FRIENDLY_GREET", "GET_OUT", "GROVEL_LARGE", "GROVEL_SMALL", "HERO_IMITATION_PLAY", "HEROTITLE_AT_HERO", "HEROTITLE_TO_SELF", "HIDE", "MARRIAGE_COMMENT", "OFFER_GIFT_FRIENDLY", "OFFER_GIFT_WORSHIP", "PICK_FIGHT", "POINT", "POINT_LAUGH", "RESPECTFUL_GREET", "SHAKE_FIST", "SHOUT_AWE", "SNIGGER", "THUMBS_DOWN", "WATCH", "WIFE_FEELING_LOVE", "WIFE_GENERAL_LOVE", "WIFE_TOHERSELF_LOVE", "WIFE_CLOTHING_LOVE", "WIFE_FEELING_LIKE", "WIFE_GENERAL_LIKE", "WIFE_TOHERSELF_LIKE", "WIFE_CLOTHING_LIKE", "WIFE_FEELING_NEUTRAL", "WIFE_GENERAL_NEUTRAL", "WIFE_TOHERSELF_NEUTRAL", "WIFE_CLOTHING_NEUTRAL", "WIFE_FEELING_DISLIKE", "WIFE_GENERAL_DISLIKE", "WIFE_TOHERSELF_DISLIKE", "WIFE_CLOTHING_DISLIKE", "WIFE_FEELING_HATE", "WIFE_GENERAL_HATE", "WIFE_TOHERSELF_HATE", "WIFE_CLOTHING_HATE", "WIFE_JUSTMARRIED", "WIFE_GIFT_WANTED" };
static const int ExpectedUpdate[79] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 4, 6, 7, 8, 10, 12, 13, 14, 15, 17, 19, 21, 22, 23, 25, 27, 29, 30, 31, 32, 33, 35, 37, 39, 40, 42, 43, 44, 45, 48, 47, 50, 51, 53, 54, 56, 57, 67, 72, 82, 62, 66, 71, 81, 61, 68, 73, 83, 63, 64, 69, 79, 58, 65, 70, 80, 59, 77, 75 };
static const int ExpectedRunnable[79] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 5, 0, 0, 9, 11, 0, 0, 0, 16, 18, 20, 0, 0, 24, 26, 28, 0, 0, 0, 0, 34, 36, 38, 0, 41, 0, 0, 0, 46, 0, 49, 0, 52, 0, 55, 0, 0, 78, 78, 60, 0, 78, 78, 60, 0, 78, 78, 60, 0, 78, 78, 60, 0, 78, 78, 60, 76, 74 };
int main() {
    if (sizeof(COpinionReactionBehaviour) != 40) { std::puts("01221d20_TEST FAIL size"); return 1; }
    CAIStateGroup_OpinionReactionRoot root; int updates = 0, runnables = 0;
    for (int i = 0; i < 79; ++i) {
        const COpinionReactionBehaviour& b = CAIStateGroup_OpinionReactionRoot::Behaviours[i];
        char want[96]; std::sprintf(want, "OPINION_REACTION_TYPE_%s", ExpectedName[i]);
        if (std::strcmp(b.Name.Text, want)) { std::printf("01221d20_TEST FAIL name %d\n", i); return 1; }
        const unsigned long* w = (const unsigned long*)&b;
        for (int k = 0; k < 2; ++k) {
            bool (CAIStateGroup_OpinionReactionRoot::*f)() = k ? b.IsRunnableFunc : b.UpdateFunc;
            int expected = k ? ExpectedRunnable[i] : ExpectedUpdate[i]; const unsigned long* pw = w + 4 * k;
            if (!expected) {
                if (f || pw[0] || pw[1] || pw[2] || pw[3] != 0xFFFFFFFFul) { std::printf("01221d20_TEST FAIL null %d/%d\n", i, k); return 1; }
                continue;
            }
            if (!f || pw[1] || pw[2] || pw[3]) { std::printf("01221d20_TEST FAIL set %d/%d\n", i, k); return 1; }
            int calls = root.Calls; bool r = (root.*f)();
            if (root.LastCode != expected || root.Calls != calls + 1 || r != root.Result) { std::printf("01221d20_TEST FAIL call %d/%d\n", i, k); return 1; }
            if (k) ++runnables; else ++updates;
        }
    }
    if (updates != 61 || runnables != 35) { std::printf("01221d20_TEST FAIL count %d %d\n", updates, runnables); return 1; }
    std::puts("01221d20_TEST PASS");
    return 0;
}