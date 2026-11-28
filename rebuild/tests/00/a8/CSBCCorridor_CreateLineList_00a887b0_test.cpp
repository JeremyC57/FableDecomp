#define FABLE_RETAIL_SB_CORRIDOR_PASS29
#define FABLE_RETAIL_CSB
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CSBCCorridor.h"

void CSBCCorridor::CreateLineList(std::vector<C3DVector>& line_list)
{
    for (std::list<CWall>::iterator wall = Walls.begin(); wall != Walls.end(); ++wall) {
        C3DVector start;
        C3DVector end;
        start.X = wall->Point1.X;
        start.Y = wall->Point1.Y;
        start.Z = 0.0f;
        end.X = wall->Point2.X;
        end.Y = wall->Point2.Y;
        end.Z = 0.0f;
        line_list.push_back(start);
        line_list.push_back(end);
    }
}

#include <cstdio>
#include <cstdlib>

static int initialWalls;
CSB::~CSB() {}
void CSB::CreateLineList(std::vector<C3DVector>&) { std::abort(); }
void CSB::Reset() { std::abort(); }
bool CSBCCorridor::Process(const CSteeringAgent*) { std::abort(); }
CSB::ESB_TYPE CSBCCorridor::GetType() { return ESBC_CORRIDOR; }
void CSBCCorridor::Reset() {
    // Test-only construction of the wall list using its engine element and container.
    Walls.clear();
    for (int i = 0; i < initialWalls; ++i) {
        C2DVector a, b, normal;
        a.X = float(i+1); a.Y = float(-2-i);
        b.X = float(3+i*2); b.Y = float(5-i*3);
        normal.X = 90.0f; normal.Y = -80.0f;
        Walls.push_back(CWall(a,b,normal));
    }
}
static void check(bool ok) { if (!ok) std::abort(); }
int main() {
    for (int walls = 0; walls <= 4; ++walls) {
        for (int prefix = 0; prefix <= 3; ++prefix) {
            for (int spare = 0; spare <= 1; ++spare) {
                CSBCCorridor corridor;
                initialWalls = walls;
                corridor.Reset();
                std::vector<C3DVector> lines;
                if (spare) {
                    for (int i = 0; i < 16; ++i) lines.push_back(C3DVector(0,0,0));
                    lines.clear();
                }
                for (int p = 0; p < prefix; ++p) lines.push_back(C3DVector(float(p),71,-3));
                corridor.CreateLineList(lines);
                check(lines.size() == unsigned(prefix + walls*2));
                for (int p = 0; p < prefix; ++p)
                    check(lines[p].X == float(p) && lines[p].Y == 71 && lines[p].Z == -3);
                for (int i = 0; i < walls; ++i) {
                    const C3DVector& a = lines[prefix + 2*i];
                    const C3DVector& b = lines[prefix + 2*i + 1];
                    check(a.X == i+1 && a.Y == -2-i && a.Z == 0);
                    check(b.X == 3+i*2 && b.Y == 5-i*3 && b.Z == 0);
                }
            }
        }
    }
    std::puts("00a887b0_TEST PASS");
    return 0;
}
