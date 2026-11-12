// CTCTalk::PlaySpeech(CASoundBank& bank, long sample, bool b, const CCharString& narrator, NSpeechGainManager::EDialogueLayer layer, bool in2D, bool c) -- play a speech sample through the owner's sound player (FAILED_FINAL when it has none): stop a speech still running (inline IsTalking), drop IsPureAISpeaker when the narrator is not a permitted AI narrator (inline SetAsUsingNarrator), remember the narrator, play the sample with gain-fader bits 0x8040, tell the speech gain manager a dialogue started, apply the entity sound def's voice pitch override, set up lip movement, restart the speech frame window
// Shape: inline IsTalking / SetAsUsingNarrator (header opt-in) and the inline STLport find; the real bodies of __find 0x004B8FF0 and CBasicString<T>::Compare 0x00411570 sit in the TU out of line, so VC7.1 knows the find call writes no memory and loads the find's `first` (PermittedAINarrators.begin()) late, straight into ecx, as retail.
// Retail: Fable.exe @ 0x006E3580, 502 bytes, thiscall `ret 1Ch`, no EH frame.
// Callees: CVectorMap::LowerBound 0x0040F020, CMainGameComponent::GetCurrentServerFrame 0x00416225, CTCTalk::StopCurrentSpeech(CTCSoundPlayer*) 0x006E28D0,
//   CGameDefinitionManager::Get 0x0044C6B0, std::__find<const CCharString*,CCharString> 0x004B8FF0 (same-TU noinline body below),
//   CBasicString<char>::Compare 0x00411570 (same-TU noinline body below, called from __find), CCharString::operator= 0x0099EFB0,
//   CTCSoundPlayer::PlayAttachedSample(CASoundBank&,...) 0x0066B300, CTCBase::GetWorld 0x00686A70, NSpeechGainManager::OnDialogueStart 0x006E31E0,
//   CDefClassBase::GetSubDef<CEntitySoundDef> 0x0065F604, CTCSoundPlayer::SetSpecificSamplePitch 0x0066AFA0, CTCTalk::PlayLipMovement 0x006E29A0,
//   CTCTalk::GetSpeechDurationInFrames 0x006E2940. Data: g_InvalidSoundHandle 0x0129D25C.
// Header overlay: hdr/ (FABLE_HDR_OVERLAY), see hdr/README.md.
// Leaf: ?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@AAVCASoundBank@@J_NABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@11@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_CONVERSATION
#define FABLE_RETAIL_CTCTALK_PLAYSPEECH
#define FABLE_RETAIL_CTCTALK_PLAYSPEECH_INLINES
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_BANKSAMPLE
#define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE

#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SERVERFRAME_OUTOFLINE
#define FABLE_RETAIL_NSPEECHGAINMANAGER_ONDIALOGUESTART
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CDefPointer.h"
#include "engine/CEntitySoundDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGlobalDef.h"
#include "engine/CSoundPair.h"
#include "engine/CSoundInit.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/NSpeechGainManager.h"
#include "engine/CTCTalk.h"

// CBasicString<T>::Compare (retail 0x00411570, the landed body), out of line (noinline) so VC7.1 knows it writes no memory.
#pragma auto_inline(off)
template <class T> long __fastcall CBasicString<T>::Compare(const T* a, const T* b)   // the landed 0x00411570 body (?Compare@?$CBasicString@D@@KIJPBD0@Z)
{
    for (;;) {
        T ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        T cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

// Same-TU body of STLport 4.6 __find (retail 0x004B8FF0, the landed std::__find<const CCharString*,CCharString> source), out of line
// (noinline) so VC7.1 sees it never touches the empty category tag.
namespace std {
#pragma auto_inline(off)
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                                         const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)
}

extern unsigned long g_InvalidSoundHandle;      // 0x0129D25C

CTCTalk::ESpeechPlaybackResult CTCTalk::PlaySpeech(CASoundBank& bank, long sample, bool b, const CCharString& narrator,
                                                   NSpeechGainManager::EDialogueLayer layer, bool in2D, bool c)
{
    CTCSoundPlayer* player = NULL;
    if (!ParentThing->GetTC(TCI_SOUND_PLAYER, &player))
        return SPEECH_PLAYBACK_FAILED_FINAL;
    if (IsTalking())
        StopCurrentSpeech(player);
    SetAsUsingNarrator(narrator);
    LastNarrator = narrator;
    CSoundInit init;
    init.GainFaderBits = 0x8040;
    SampleID = player->PlayAttachedSample(bank, CSoundPair(sample), init, true, b, in2D);
    if (SampleID != g_InvalidSoundHandle)
    {
        CThing& thing = *ParentThing;
        NSpeechGainManager::OnDialogueStart(GetWorld(), thing, SampleID, layer);
    }
    CDefPointer<const CEntitySoundDef> sound_def;
    if (ParentThing->GetPBaseDef()->GetSubDef(&sound_def) && sound_def->VoicePitchOverride != 1.0f)
        player->SetSpecificSamplePitch(SampleID, sound_def->VoicePitchOverride);
    UseLipSynching = c;
    PlayLipMovement(bank, sample);
    SampleLoadedTime = -1.0f;
    SampleStartedFrame = CMainGameComponent::Get()->GetCurrentServerFrame();
    SampleDurationFrames = GetSpeechDurationInFrames(sample);
    return SPEECH_PLAYBACK_STARTED;
}