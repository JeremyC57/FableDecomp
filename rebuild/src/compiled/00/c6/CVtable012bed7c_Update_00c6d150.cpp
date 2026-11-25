// CSalRenderPanDelay::SetPosition(float x, float y, float blend, float volume, const float* gains, bool crossfade, bool force) -- vtable
//   slot 4 of the GameCODA HRTF-delay panner: look up the direction's pair of filter records (0x00C400D0), take their delays as the
//   per-channel interaural delays m_itd, compute the per-channel pan gains m_fGainPan from the azimuth (four channels when the platform
//   mixes four or more outputs, else two), take the volume as m_fGain, and rebuild the filters when anything changed (or when forced)
// Leaf: ?SetPosition@CSalRenderPanDelay@@UAEXMMMMPBM_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6D150, 511 bytes, thiscall, 7 stack args (`ret 1Ch`); `blend` and `gains` are not read here. Slot 4
//   of the vtables 0x012BED7C, 0x012B8A64 and 0x012B8AA0 (the SIMD classes inherit it).
// Name (NF447, two sources): the GameCODA 2.10 SDK CodeView declares the virtual CSalRenderPanDelay::SetPosition(float, float, float, float,
//   const float*, bool, bool), slot 4 of CSalRender (engine/CSalRender.h), and the SDK Release ?SetPosition@CSalRenderPanDelay is 150
//   instructions, this body's count (NF441 CLASS_TABLE); retail pops 28 argument bytes = that signature (NF445 sigcheck_19.tsv). Was
//   ?Update@CVtable_012bed7c@@UAEXMMMMPBM_N1@Z (descriptive stand-in).
// Callees: 0x00C400D0 (cdecl, 5 args: a table selector 4 here, the azimuth, the elevation, two out pointers to 0x34-byte direction-filter
//   records whose word +0x32 is the delay; landed spelling LookupDirectionFilterPair), and the private CSalRenderPanDelay::UpdateFilters(const
//   bool) 0x00C6CF40 (the SDK Release SetPosition relocates to it at +0x1F2; retail `call 0x00C6CF40` at 0x00C6D341; ported in the same batch).
// Shape: a function-scope float gain[6] (the frame is 0x20: two record pointers + six floats; the stereo arm reuses gain[0..1]),
//   sin()/cos() intrinsics on the inline `x * 0.017453292f` (a float `angle` local is rounded through memory and spills), the
//   compiler homes cos*0.4, 0.6-sin*0.4 and cos*0.4+0.6 in the dead x / y / force parameter slots.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderPanDelay.h"   // CSalRenderPanDelay (vtable 0x012BED7C; m_fGainPan +0x1E4, m_itd +0x1FC) : CSalRender (m_fGain +0x20)
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(sin, cos)
#endif
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
struct SDirectionFilter { short m_coefficients[25]; unsigned short m_delay; };   // 0x34-byte record of the direction table (delay word at +0x32)
void LookupDirectionFilterPair(long table, float azimuth, float elevation, const SDirectionFilter** first, const SDirectionFilter** second);   // 0x00C400D0 (landed callers call it Animation_ComputeFrameOffset: a propagated wrong label)

void CSalRenderPanDelay::SetPosition(float x, float y, float, float volume, const float*, bool crossfade, bool force)
{
    const SDirectionFilter* first;
    const SDirectionFilter* second;
    bool changed = false;
    float gain[6];
    LookupDirectionFilterPair(4, x, y, &first, &second);
    if (force || m_itd[0] != first->m_delay || m_itd[1] != second->m_delay) {
        m_itd[0] = first->m_delay;
        m_itd[2] = first->m_delay;
        m_itd[1] = second->m_delay;
        m_itd[3] = second->m_delay;
        changed = true;
    }
    if (g_pAudioMaster->m_outputChannels >= 4) {
        float s = (float)sin(x * 0.017453292f) * 0.4f;
        float c = (float)cos(x * 0.017453292f) * 0.4f;
        gain[0] = (c + 0.6f) * (0.6f - s);
        gain[1] = (c + 0.6f) * (s + 0.6f);
        gain[2] = (0.6f - s) * (0.6f - c);
        gain[3] = (s + 0.6f) * (0.6f - c);
        if (force || m_fGainPan[0] != gain[0] || m_fGainPan[1] != gain[1] || m_fGainPan[2] != gain[2] || m_fGainPan[3] != gain[3]) {
            m_fGainPan[0] = gain[0];
            m_fGainPan[1] = gain[1];
            m_fGainPan[2] = gain[2];
            m_fGainPan[3] = gain[3];
            changed = true;
        }
    } else {
        float s = (float)sin(x * 0.017453292f) * 0.4f;
        gain[0] = 0.6f - s;
        gain[1] = s + 0.6f;
        if (force || m_fGainPan[0] != gain[0] || m_fGainPan[1] != gain[1]) {
            m_fGainPan[0] = gain[0];
            m_fGainPan[1] = gain[1];
            changed = true;
        }
    }
    if (force || m_fGain != volume) {
        m_fGain = volume;
        changed = true;
    }
    if (changed)
        UpdateFilters(crossfade);
}
