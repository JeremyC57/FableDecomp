// Behaviour test for CEAXManager::ApplyVoiceBufferProperties (0x00C4EA30): levels clamp to [-10000, 0], LF ratios to [0, 1],
// room-rolloff / air-absorption to [0, 10]; the clamped block is kept on the voice; a 3D object gets direct, direct HF and the
// block, a 2D object direct and room; 0 without an object, for a property id with no data, or for a version other than 2.
#include <cstdio>
#include <cstring>
#define EAX_CLAMP(v, lo, hi) (((v) <= (hi) && (v) >= (lo)) ? (v) : (((v) > (hi)) ? (hi) : (lo)))
#define EAX_CLAMP_TO(applied, parameter, v, lo, hi) \
    if ((v) <= (hi) && (v) >= (lo)) { applied = (v); parameter = (v); } \
    else { applied = ((v) > (hi)) ? (hi) : (lo); parameter = ((v) > (hi)) ? (hi) : (lo); }
struct Params { int lRoom, lRoomHF, lObstruction; float flObstructionLFRatio; int lOcclusion; float flOcclusionLFRatio;
                int lExclusion; float flExclusionLFRatio, flRoomRolloffFactor, flAirAbsorptionFactor; };
struct Eax3 { int lDirect, lDirectHF, lRoom, lRoomHF, lObstruction; float flObstructionLFRatio; int lOcclusion;
              float flOcclusionLFRatio, flOcclusionRoomRatio, flOcclusionDirectRatio; int lExclusion; float flExclusionLFRatio;
              int lOutsideVolumeHF; float flDopplerFactor, flRolloffFactor, flRoomRolloffFactor, flAirAbsorptionFactor; };
struct Object { bool is3D; int direct, directHF, room; Params block; Object() : is3D(false), direct(1), directHF(1), room(1) {} };
struct Voice { Eax3 eax; Params applied; Object* owner; };
static int Apply(Voice* voice, unsigned version, unsigned id, void* data)
{
    int result = 0;
    Object* owner = voice->owner;
    if (!owner) return 0;
    if (!id || data) {
        Params parameters; std::memset(&parameters, 0, sizeof parameters);
        if (version == 2) {
            const Eax3& e = voice->eax;
            int direct = EAX_CLAMP(e.lDirect, -10000, 0);
            int directHF = EAX_CLAMP(e.lDirectHF, -10000, 0);
            EAX_CLAMP_TO(voice->applied.lRoom, parameters.lRoom, e.lRoom, -10000, 0)
            EAX_CLAMP_TO(voice->applied.lRoomHF, parameters.lRoomHF, e.lRoomHF, -10000, 0)
            EAX_CLAMP_TO(voice->applied.lObstruction, parameters.lObstruction, e.lObstruction, -10000, 0)
            EAX_CLAMP_TO(voice->applied.flObstructionLFRatio, parameters.flObstructionLFRatio, e.flObstructionLFRatio, 0.0f, 1.0f)
            EAX_CLAMP_TO(voice->applied.lOcclusion, parameters.lOcclusion, e.lOcclusion, -10000, 0)
            EAX_CLAMP_TO(voice->applied.flOcclusionLFRatio, parameters.flOcclusionLFRatio, e.flOcclusionLFRatio, 0.0f, 1.0f)
            EAX_CLAMP_TO(voice->applied.flRoomRolloffFactor, parameters.flRoomRolloffFactor, e.flRoomRolloffFactor, 0.0f, 10.0f)
            EAX_CLAMP_TO(voice->applied.flAirAbsorptionFactor, parameters.flAirAbsorptionFactor, e.flAirAbsorptionFactor, 0.0f, 10.0f)
            EAX_CLAMP_TO(voice->applied.lExclusion, parameters.lExclusion, e.lExclusion, -10000, 0)
            EAX_CLAMP_TO(voice->applied.flExclusionLFRatio, parameters.flExclusionLFRatio, e.flExclusionLFRatio, 0.0f, 1.0f)
            result = 1;
            if (owner->is3D) { owner->direct = direct; owner->directHF = directHF; owner->block = parameters; }
            else { owner->direct = direct; owner->room = parameters.lRoom; }
        }
    }
    return result;
}
int main()
{
    Voice v; std::memset(&v, 0, sizeof v);
    v.eax.lDirect = 500; v.eax.lDirectHF = -20000; v.eax.lRoom = -300; v.eax.lRoomHF = 10; v.eax.lObstruction = -10001;
    v.eax.flObstructionLFRatio = 1.5f; v.eax.lOcclusion = -42; v.eax.flOcclusionLFRatio = -0.25f; v.eax.lExclusion = 1;
    v.eax.flExclusionLFRatio = 0.5f; v.eax.flRoomRolloffFactor = 12.0f; v.eax.flAirAbsorptionFactor = 3.0f;
    int dummy = 0;
    bool ok = Apply(&v, 2, 0, 0) == 0;                      // no object
    Object o3; o3.is3D = true; v.owner = &o3;
    ok = ok && Apply(&v, 2, 7, 0) == 0 && o3.direct == 1;   // a property id without data
    ok = ok && Apply(&v, 4, 7, &dummy) == 0 && o3.direct == 1;   // EAX 4: nothing applied
    ok = ok && Apply(&v, 2, 7, &dummy) == 1 && o3.direct == 0 && o3.directHF == -10000 && o3.block.lRoom == -300
            && o3.block.lRoomHF == 0 && o3.block.lObstruction == -10000 && o3.block.flObstructionLFRatio == 1.0f
            && o3.block.lOcclusion == -42 && o3.block.flOcclusionLFRatio == 0.0f && o3.block.lExclusion == 0
            && o3.block.flExclusionLFRatio == 0.5f && o3.block.flRoomRolloffFactor == 10.0f && o3.block.flAirAbsorptionFactor == 3.0f
            && v.applied.lRoom == -300 && v.applied.flRoomRolloffFactor == 10.0f && o3.room == 1;
    Object o2; v.owner = &o2;
    ok = ok && Apply(&v, 2, 0, 0) == 1 && o2.direct == 0 && o2.room == -300 && o2.directHF == 1;
    if (!ok) { std::printf("00c4ea30_TEST FAIL\n"); return 1; }
    std::printf("00c4ea30_TEST PASS\n");
    return 0;
}