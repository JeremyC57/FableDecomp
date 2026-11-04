// Behaviour test for CEngineSimulationManager::FrameStart (0x00B81820, parked DIFFER): a model of the row -- wind time = speed x time, zoom clamped to >= 1, the simulation frame swapped in and restored, each pending step run.
#include <stdio.h>
#include <math.h>
struct M { float windTime, windOff, zoom; int full, always, paused, runs; };
static long g_frame = 3;
static void FrameStart(M& m, float speed, double time, float camZoom, long worldFrame)
{ m.windTime = (float)(speed * time); m.windOff = sinf(m.windTime * 6.2831855f); m.zoom = 1.0f > camZoom ? 1.0f : camZoom;
  long saved = g_frame; g_frame = worldFrame; while (m.full) { ++m.runs; --m.full; } while (m.always) { ++m.runs; --m.always; }
  while (m.paused) { ++m.runs; --m.paused; } g_frame = saved; }
int main() { M m = { 0, 0, 0, 2, 1, 0, 0 }; FrameStart(m, 2, 0.25, 0.5f, 9);
  bool ok = m.windTime == 0.5f && m.zoom == 1.0f && m.runs == 3 && g_frame == 3 && m.full == 0;
  printf(ok ? "00b81820_TEST PASS\n" : "00b81820_TEST FAIL\n");
  return ok ? 0 : 1;
}
