// Behaviour test for 0x00A1C700 CSteeringAgent::CSteeringAgent(const CNavigatorExternalInterface*, bool):
// a model of the constructor (circular STLport-style lists with a malloc'd sentinel, push_back = insert before the
// sentinel) checks the reset values and which behaviours a real / dummy / interface-less agent receives.
#include <stdio.h>
#include <stdlib.h>

struct Node { Node* next; Node* prev; void* data; };
struct List {
    Node* head;
    List() { head = (Node*)malloc(sizeof(Node)); head->next = head; head->prev = head; }
    void push_back(void* x)
    {
        Node* pos = head;
        Node* tmp = (Node*)malloc(sizeof(Node));
        tmp->data = x;
        Node* prev = pos->prev;
        tmp->next = pos;
        tmp->prev = prev;
        prev->next = tmp;
        pos->prev = tmp;
    }
    int size() const { int n = 0; for (Node* p = head->next; p != head; p = p->next) ++n; return n; }
    void* at(int i) const { Node* p = head->next; while (i--) p = p->next; return p->data; }
};

struct Obstacles { int count; };
struct Interface { Obstacles obstacles; const Obstacles& GetNavigatorObstacles() const { return obstacles; } };

enum Kind { KAgents = 1, KGoal, KIAgents, KCorridor, KCAgents };
struct Behaviour { int kind; const void* a; const void* b; };

struct V2 { float X, Y; };
struct Agent {
    const Interface* NavigatorInterface; bool DummyObstacle; float RepulsionMultiplier;
    List SteeringBehaviours, SteeringBehaviourInputs, SteeringBehaviourConstraints;
    float CurrentMaxSpeed, CurrentSpeed; V2 Goal, NextGoal; int TimeToReacquireGoal; float DistCutoff; V2 Steer;
    float BestDistLeft, BestDistRight, BestNextDistLeft, BestNextDistRight, ProximityForArrival; int TimeSinceBestApproach;
    float BestApproach; bool Wandering, MovingForwards, RecalcWidth, AddWaypoint, AvoidDynamicObstacles,
    AvoidDynamicObstaclesUntilNextWaypoint, HasWanderedDuringThisSection, FailPath;
    Agent(const Interface* iface, bool dummy)
    {
        NavigatorInterface = iface; DummyObstacle = dummy; RepulsionMultiplier = 1.0f;
        CurrentMaxSpeed = 0.0f; CurrentSpeed = 0.0f; Goal.X = 0.0f; Goal.Y = 0.0f; TimeToReacquireGoal = 0;
        DistCutoff = 5.0f; Wandering = false; Steer.X = 0.0f; Steer.Y = 0.0f; MovingForwards = true;
        AvoidDynamicObstacles = true; AvoidDynamicObstaclesUntilNextWaypoint = true; BestDistLeft = 0.0f;
        BestDistRight = 0.0f; BestNextDistLeft = 0.0f; BestNextDistRight = 0.0f; TimeSinceBestApproach = 0;
        BestApproach = 0.0f; RecalcWidth = false; AddWaypoint = false; HasWanderedDuringThisSection = false;
        FailPath = false; ProximityForArrival = 0.0f;
        if (NavigatorInterface && !DummyObstacle) {
            Behaviour* b;
            b = new Behaviour; b->kind = KAgents; b->a = &NavigatorInterface->GetNavigatorObstacles(); b->b = this;
            SteeringBehaviours.push_back(b);
            b = new Behaviour; b->kind = KGoal; b->a = &Goal; b->b = &BestDistLeft;
            SteeringBehaviours.push_back(b);
            b = new Behaviour; b->kind = KIAgents; b->a = &NavigatorInterface->GetNavigatorObstacles(); b->b = 0;
            SteeringBehaviourInputs.push_back(b);
            b = new Behaviour; b->kind = KCorridor; b->a = 0; b->b = 0;
            SteeringBehaviourConstraints.push_back(b);
            b = new Behaviour; b->kind = KCAgents; b->a = &NavigatorInterface->GetNavigatorObstacles(); b->b = 0;
            SteeringBehaviourConstraints.push_back(b);
        }
    }
};

static int fail(int line) { printf("00a1c700_TEST FAIL (line %d)\n", line); return 1; }

int main()
{
    Interface iface; iface.obstacles.count = 3;
    Agent real(&iface, false);
    if (real.RepulsionMultiplier != 1.0f || real.DistCutoff != 5.0f || !real.MovingForwards) return fail(__LINE__);
    if (!real.AvoidDynamicObstacles || !real.AvoidDynamicObstaclesUntilNextWaypoint || real.Wandering) return fail(__LINE__);
    if (real.Goal.X != 0.0f || real.Steer.Y != 0.0f || real.ProximityForArrival != 0.0f) return fail(__LINE__);
    if (real.SteeringBehaviours.size() != 2 || real.SteeringBehaviourInputs.size() != 1) return fail(__LINE__);
    if (real.SteeringBehaviourConstraints.size() != 2) return fail(__LINE__);
    if (((Behaviour*)real.SteeringBehaviours.at(0))->kind != KAgents) return fail(__LINE__);
    if (((Behaviour*)real.SteeringBehaviours.at(0))->b != &real) return fail(__LINE__);
    if (((Behaviour*)real.SteeringBehaviours.at(1))->a != &real.Goal) return fail(__LINE__);
    if (((Behaviour*)real.SteeringBehaviourInputs.at(0))->a != &iface.obstacles) return fail(__LINE__);
    if (((Behaviour*)real.SteeringBehaviourConstraints.at(0))->kind != KCorridor) return fail(__LINE__);
    if (((Behaviour*)real.SteeringBehaviourConstraints.at(1))->kind != KCAgents) return fail(__LINE__);
    Agent dummy(&iface, true);
    if (dummy.SteeringBehaviours.size() != 0 || dummy.SteeringBehaviourConstraints.size() != 0) return fail(__LINE__);
    Agent none(0, false);
    if (none.SteeringBehaviourInputs.size() != 0 || none.DistCutoff != 5.0f) return fail(__LINE__);
    printf("00a1c700_TEST PASS\n");
    return 0;
}