// Behaviour test for NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams (0x008E18D0): the beams start at the caster's
// WEAPON_FOCUS_01 dummy (the caster's position when the dummy is missing); main beam n points along (0,1,0) turned about Z by
// n / num_beams plus the fractional part of StormRotation (truncated toward zero), StormRadius long, at the caster's height; each
// live main beam gets LIGHTNING_START / LIGHTNING_END vector parameters (CRC keys, sorted by key); each live end beam is moved to its
// end point, with the main beams' angle step; a live sky beam runs from the start to 15 above it. Dead or missing emitters are skipped.
// Self-contained test doubles (no engine classes): plain C++03, no exceptions.
#include <stdio.h>
#include <string.h>
#include <math.h>

static int errors;
#define CHECK(condition) do { if (!(condition)) { \
    if (errors < 20) printf("line %d: %s\n", __LINE__, #condition); \
    ++errors; \
} } while (0)

static unsigned long g_crc_table[256];
static void BuildCrcTable()
{
    for (unsigned long n = 0; n < 256; ++n) {
        unsigned long c = n;
        for (int k = 0; k < 8; ++k)
            c = (c & 1) ? 0xEDB88320UL ^ (c >> 1) : c >> 1;
        g_crc_table[n] = c;
    }
}
// The seeded table CRC of engine/CCRC.h (FABLE_RETAIL_CCRC_PASS22_BODY): no final xor, an empty string returns the seed.
static unsigned long CrcOf(unsigned long seed, const char* text)
{
    unsigned long length = (unsigned long)strlen(text);
    if (length == 0)
        return seed;
    for (unsigned long i = 0; i < length; ++i)
        seed = g_crc_table[((const unsigned char*)text)[i] ^ (seed & 0xff)] ^ (seed >> 8);
    return seed;
}

struct Vec3 { float x, y, z; };
static Vec3 MakeVec(float x, float y, float z) { Vec3 v; v.x = x; v.y = y; v.z = z; return v; }
static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }
static bool NearVec(const Vec3& a, const Vec3& b) { return Near(a.x, b.x) && Near(a.y, b.y) && Near(a.z, b.z); }

// (0,1,0) turned about Z by `turns` of a full turn (the horizontal case of the vector-math RotateXY 0x00A14040).
static Vec3 TurnedUnitY(float turns)
{
    if (turns < 0.0f || turns >= 1.0f) {
        turns = (float)fmod((double)turns, 1.0);
        if (turns < 0.0f)
            turns += 1.0f;
    }
    return MakeVec((float)sin(turns * 6.2831855), (float)cos(turns * 6.2831855), 0.0f);
}

struct ParticleEmitterDouble {
    bool present;                     // the intelligent pointer is non-null
    bool dead;
    int parameter_updates;            // SetEmitterUpdateParameters calls
    unsigned long keys[2];            // the vector parameters after Update (sorted by key)
    Vec3 values[2];
    int position_updates;             // SetEmitterPosition calls
    Vec3 position;
};
static void ResetEmitter(ParticleEmitterDouble& e, bool present, bool dead)
{
    memset(&e, 0, sizeof(e));
    e.present = present;
    e.dead = dead;
}
static bool IsAlive(const ParticleEmitterDouble& e) { return e.present && !e.dead; }

struct StormDouble {
    Vec3 parent_pos;
    bool has_weapon_focus;            // the appearance has a WEAPON_FOCUS_01 dummy
    Vec3 weapon_focus;
    float storm_rotation;
    float storm_radius;
    long num_main, num_end;
    ParticleEmitterDouble main_beams[4];
    ParticleEmitterDouble end_beams[4];
    ParticleEmitterDouble sky_beam;
};

static void SetBeamParameters(ParticleEmitterDouble& e, const Vec3& start, const Vec3& end)
{
    unsigned long start_key = CrcOf(0, "LIGHTNING_START"), end_key = CrcOf(0, "LIGHTNING_END");
    // Insert START, Insert END, then Update sorts the vector map by key.
    if (start_key < end_key) {
        e.keys[0] = start_key; e.values[0] = start; e.keys[1] = end_key; e.values[1] = end;
    } else {
        e.keys[0] = end_key; e.values[0] = end; e.keys[1] = start_key; e.values[1] = start;
    }
    ++e.parameter_updates;
}

static void UpdateLightningBeams(StormDouble& s)
{
    long num_beams = s.num_main;
    float angle_between_beams = 1.0f / num_beams;
    float storm_rotation = s.storm_rotation;
    long storm_rotation_int = (long)storm_rotation;
    storm_rotation -= storm_rotation_int;
    float beam_length = s.storm_radius;
    Vec3 weapon_pos = s.parent_pos;
    if (s.has_weapon_focus)
        weapon_pos = s.weapon_focus;
    Vec3 start_pos = weapon_pos;
    long n;
    for (n = 0; n < num_beams; ++n) {
        if (IsAlive(s.main_beams[n])) {
            Vec3 d = TurnedUnitY(n * angle_between_beams + storm_rotation);
            Vec3 end_pos = MakeVec(d.x * beam_length + s.parent_pos.x, d.y * beam_length + s.parent_pos.y, d.z * beam_length + s.parent_pos.z);
            end_pos.z = s.parent_pos.z;
            SetBeamParameters(s.main_beams[n], start_pos, end_pos);
        }
    }
    long num_beams_ends = s.num_end;
    for (n = 0; n < num_beams_ends; ++n) {
        if (IsAlive(s.end_beams[n])) {
            Vec3 d = TurnedUnitY(n * angle_between_beams + storm_rotation);
            Vec3 end_pos = MakeVec(d.x * beam_length + s.parent_pos.x, d.y * beam_length + s.parent_pos.y, d.z * beam_length + s.parent_pos.z);
            end_pos.z = s.parent_pos.z;
            s.end_beams[n].position = end_pos;
            ++s.end_beams[n].position_updates;
        }
    }
    if (IsAlive(s.sky_beam)) {
        Vec3 end_pos = MakeVec(start_pos.x, start_pos.y, start_pos.z + 15.0f);
        SetBeamParameters(s.sky_beam, start_pos, end_pos);
    }
}

static void ResetStorm(StormDouble& s)
{
    memset(&s, 0, sizeof(s));
    s.parent_pos = MakeVec(10.0f, 20.0f, 3.0f);
    s.has_weapon_focus = true;
    s.weapon_focus = MakeVec(10.5f, 20.0f, 5.0f);
    s.storm_rotation = 2.0f;
    s.storm_radius = 4.0f;
    for (int i = 0; i < 4; ++i) {
        ResetEmitter(s.main_beams[i], true, false);
        ResetEmitter(s.end_beams[i], true, false);
    }
    ResetEmitter(s.sky_beam, true, false);
}

static const Vec3* ValueOf(const ParticleEmitterDouble& e, const char* name)
{
    unsigned long key = CrcOf(0, name);
    for (int i = 0; i < 2; ++i)
        if (e.keys[i] == key)
            return &e.values[i];
    return 0;
}

int main()
{
    BuildCrcTable();
    CHECK(CrcOf(0xFFFFFFFFUL, "123456789") == 0x340BC6D9UL);       // the reflected CRC-32 without the final xor
    CHECK(CrcOf(0, "LIGHTNING_START") != CrcOf(0, "LIGHTNING_END"));

    // 1. Four main beams (one dead, one missing), two end beams (one dead), rotation 2.0 (fraction 0): beams every quarter turn.
    StormDouble s;
    ResetStorm(s);
    s.num_main = 4; s.num_end = 2;
    s.main_beams[1].dead = true;
    ResetEmitter(s.main_beams[3], false, false);
    s.end_beams[0].dead = true;
    UpdateLightningBeams(s);
    CHECK(s.main_beams[0].parameter_updates == 1 && s.main_beams[1].parameter_updates == 0);
    CHECK(s.main_beams[2].parameter_updates == 1 && s.main_beams[3].parameter_updates == 0);
    CHECK(s.main_beams[0].keys[0] < s.main_beams[0].keys[1]);       // Update sorted the map by key
    const Vec3* start = ValueOf(s.main_beams[0], "LIGHTNING_START");
    const Vec3* end = ValueOf(s.main_beams[0], "LIGHTNING_END");
    CHECK(start && end);
    if (start && end) {
        CHECK(NearVec(*start, s.weapon_focus));                     // the start is the weapon focus dummy
        CHECK(NearVec(*end, MakeVec(10.0f, 24.0f, 3.0f)));          // beam 0: +Y, radius 4, the caster's height (not the dummy's)
    }
    end = ValueOf(s.main_beams[2], "LIGHTNING_END");
    CHECK(end && NearVec(*end, MakeVec(10.0f, 16.0f, 3.0f)));       // beam 2: half a turn
    CHECK(s.end_beams[0].position_updates == 0);                    // dead end beam skipped
    CHECK(s.end_beams[1].position_updates == 1);
    CHECK(NearVec(s.end_beams[1].position, MakeVec(14.0f, 20.0f, 3.0f)));   // end beam 1: a quarter turn (1 / num_beams, not 1 / 2)
    CHECK(s.sky_beam.parameter_updates == 1);
    start = ValueOf(s.sky_beam, "LIGHTNING_START");
    end = ValueOf(s.sky_beam, "LIGHTNING_END");
    CHECK(start && end && NearVec(*start, s.weapon_focus) && NearVec(*end, MakeVec(10.5f, 20.0f, 20.0f)));   // sky: +15 above the start

    // 2. Positive fractional rotation 3.25: only the fraction 0.25 turns the beams.
    ResetStorm(s);
    s.num_main = 2; s.num_end = 0;
    s.storm_rotation = 3.25f;
    UpdateLightningBeams(s);
    end = ValueOf(s.main_beams[0], "LIGHTNING_END");
    CHECK(end && NearVec(*end, MakeVec(14.0f, 20.0f, 3.0f)));
    end = ValueOf(s.main_beams[1], "LIGHTNING_END");
    CHECK(end && NearVec(*end, MakeVec(6.0f, 20.0f, 3.0f)));        // 0.5 + 0.25 turns
    CHECK(s.end_beams[0].position_updates == 0);                     // no end beams

    // 3. Negative rotation -1.25: (long) truncates toward zero, so the fraction is -0.25 (three quarters of a turn).
    ResetStorm(s);
    s.num_main = 1; s.num_end = 1;
    s.storm_rotation = -1.25f;
    UpdateLightningBeams(s);
    end = ValueOf(s.main_beams[0], "LIGHTNING_END");
    CHECK(end && NearVec(*end, MakeVec(6.0f, 20.0f, 3.0f)));
    CHECK(NearVec(s.end_beams[0].position, MakeVec(6.0f, 20.0f, 3.0f)));

    // 4. No weapon focus dummy: the start stays at the caster's position. Dead sky beam: untouched.
    ResetStorm(s);
    s.num_main = 1; s.num_end = 0;
    s.has_weapon_focus = false;
    s.sky_beam.dead = true;
    UpdateLightningBeams(s);
    start = ValueOf(s.main_beams[0], "LIGHTNING_START");
    CHECK(start && NearVec(*start, s.parent_pos));
    CHECK(s.sky_beam.parameter_updates == 0);

    // 5. Missing sky beam, and no main or end beams at all: nothing is touched (the 1/0 angle step is never used).
    ResetStorm(s);
    s.num_main = 0; s.num_end = 0;
    ResetEmitter(s.sky_beam, false, false);
    UpdateLightningBeams(s);
    CHECK(s.main_beams[0].parameter_updates == 0 && s.end_beams[0].position_updates == 0 && s.sky_beam.parameter_updates == 0);

    // 6. A live sky beam with no main beams still gets its parameters.
    ResetStorm(s);
    s.num_main = 0; s.num_end = 0;
    UpdateLightningBeams(s);
    CHECK(s.sky_beam.parameter_updates == 1);

    if (errors) { printf("008e18d0_TEST FAIL (%d)\n", errors); return 1; }
    printf("008e18d0_TEST PASS\n");
    return 0;
}