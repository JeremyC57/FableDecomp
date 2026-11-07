// Behaviour test for NScript::CV_BodyGuardScript::CBodyGuard::Main (0x00E5FD40): models the hired henchman's anger warnings,
// health-state classification, the health comment choice and the walk-to-hero pause.
#include <stdio.h>
#include <string.h>
static int failures = 0;
static void check(bool ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); ++failures; } }
// anger: < 20 low, < 45 medium, < 70 high (quits), else nothing; a made comment adds 30 s
static const char* AngerComment(long anger) {
    if (anger < 20) return "WARNING_LOW";
    if (anger < 45) return "WARNING_MEDIUM";
    if (anger < 70) return "WARNING_HIGH";
    return 0;
}
static long HealthState(float health, long critical, long low) {
    long h = (long)health;
    if (h < critical) return 2;
    if (h < low) return 1;
    return 0;
}
static const char* HealthComment(long current, long previous) {
    if (current < previous) return "HEALED";
    switch (current) { case 0: return "HEALTH_FINE"; case 1: return "HEALTH_LOW"; case 2: return "HEALTH_VERY_LOW"; }
    return "";
}
static float PauseTime(float distance) { float t = distance * 0.5f; if (t > 3.0f) t = 3.0f; return t; }
int main() {
    check(strcmp(AngerComment(0), "WARNING_LOW") == 0, "low");
    check(strcmp(AngerComment(44), "WARNING_MEDIUM") == 0, "medium");
    check(strcmp(AngerComment(69), "WARNING_HIGH") == 0, "high");
    check(AngerComment(70) == 0, "none");
    check(HealthState(9.9f, 10, 30) == 2 && HealthState(10.0f, 10, 30) == 1 && HealthState(30.5f, 10, 30) == 0, "health state");
    check(strcmp(HealthComment(0, 2), "HEALED") == 0, "healed");
    check(strcmp(HealthComment(2, 0), "HEALTH_VERY_LOW") == 0 && strcmp(HealthComment(1, 0), "HEALTH_LOW") == 0, "worse");
    check(PauseTime(4.0f) == 2.0f && PauseTime(10.0f) == 3.0f, "pause clamp");
    if (failures == 0) printf("00e5fd40_TEST PASS\n");
    return failures;
}
