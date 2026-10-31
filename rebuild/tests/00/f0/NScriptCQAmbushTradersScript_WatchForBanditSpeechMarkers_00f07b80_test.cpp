// Behaviour test for CQ_AmbushTradersScript::WatchForBanditSpeechMarkers (0x00F07B80): while speech is enabled and the
// bandit is near a marker, say the next of the first five lines in order (back/there by spy contact), then random ones,
// arming the long timer; away from a marker arm the short timer; stop when the thread terminates.
#include <stdio.h>
#include <string.h>
static int frames, termAt, count, timerSet, near_, spy, randv;
static bool enable;
static const char* said[16]; static int nsaid;
static const char* NB[5] = {"nb0","nb1","nb2","nb3","nb4"}; static const char* NT[5] = {"nt0","nt1","nt2","nt3","nt4"};
static const char* RB[5] = {"rb0","rb1","rb2","rb3","rb4"}; static const char* RT[5] = {"rt0","rt1","rt2","rt3","rt4"};
static bool Term() { return frames >= termAt; }
static void Frame() { ++frames; enable = true; }
static void Watch()
{
    const char* speech = 0;
    for (;;) {
        if (Term()) break;
        while (!enable) { Frame(); if (Term()) return; }
        if (Term()) break;
        if (near_) {
            if (count < 5) { speech = spy ? NB[count] : NT[count]; said[nsaid++] = speech; ++count; timerSet = 30; }
            else { int line = randv % 5; speech = spy ? RB[line] : RT[line]; said[nsaid++] = speech; timerSet = 30; }
        } else timerSet = 5;
        enable = false;
        Frame();
    }
}
int main()
{
    enable = true; near_ = 1; termAt = 7; randv = 7;
    Watch();
    if (nsaid != 7 || strcmp(said[0], "nt0") || strcmp(said[4], "nt4") || strcmp(said[5], "rt2") || count != 5 || timerSet != 30) { printf("00f07b80_TEST FAIL\n"); return 1; }
    frames = 0; nsaid = 0; near_ = 0; termAt = 2; Watch();
    if (nsaid != 0 || timerSet != 5) { printf("00f07b80_TEST FAIL\n"); return 1; }
    frames = 0; spy = 1; near_ = 1; count = 0; termAt = 1; Watch();
    if (nsaid != 1 || strcmp(said[0], "nb0")) { printf("00f07b80_TEST FAIL\n"); return 1; }
    printf("00f07b80_TEST PASS\n"); return 0;
}