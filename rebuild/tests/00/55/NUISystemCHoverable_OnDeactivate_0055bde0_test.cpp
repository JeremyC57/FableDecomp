// NUISystem::CDraggableInto::EndDrag() -- clear dragging flag and unregister from the drag list.
// Retail0055bde0 is15 bytes, tail jump ends55bdee; int3 at55bdef; following int3 separates unrelated bodies.
// RTTI0124b2d4 slot141; slot133=0055bbf0 removes this from global013b8ad4 list.
// No shared target header exists. Donor CDraggableInto has only opaque padding here.
namespace NUISystem { struct CHoverable {
virtual ~CHoverable(); //retail slot0=00559040
virtual void slot1();
virtual void slot2();
virtual void slot3();
virtual void slot4();
virtual void slot5();
virtual void slot6();
virtual void slot7();
virtual void slot8();
virtual void slot9();
virtual void slot10();
virtual void slot11();
virtual void slot12();
virtual void slot13();
virtual void slot14();
virtual void slot15();
virtual void slot16();
virtual void slot17();
virtual void slot18();
virtual void slot19();
virtual void slot20();
virtual void slot21();
virtual void slot22();
virtual void slot23();
virtual void slot24();
virtual void slot25();
virtual void slot26();
virtual void slot27();
virtual void slot28();
virtual void slot29();
virtual void slot30();
virtual void slot31();
virtual void slot32();
virtual void slot33();
virtual void slot34();
virtual void slot35();
virtual void slot36();
virtual void slot37();
virtual void slot38();
virtual void slot39();
virtual void slot40();
virtual void slot41();
virtual void slot42();
virtual void slot43();
virtual void slot44();
virtual void slot45();
virtual void slot46();
virtual void slot47();
virtual void slot48();
virtual void slot49();
virtual void slot50();
virtual void slot51();
virtual void slot52();
virtual void slot53();
virtual void slot54();
virtual void slot55();
virtual void slot56();
virtual void slot57();
virtual void slot58();
virtual void slot59();
virtual void slot60();
virtual void slot61();
virtual void slot62();
virtual void slot63();
virtual void slot64();
virtual void slot65();
virtual void slot66();
virtual void slot67();
virtual void slot68();
virtual void slot69();
virtual void slot70();
virtual void slot71();
virtual void slot72();
virtual void slot73();
virtual void slot74();
virtual void slot75();
virtual void slot76();
virtual void slot77();
virtual void slot78();
virtual void slot79();
virtual void slot80();
virtual void slot81();
virtual void slot82();
virtual void slot83();
virtual void slot84();
virtual void slot85();
virtual void slot86();
virtual void slot87();
virtual void slot88();
virtual void slot89();
virtual void slot90();
virtual void slot91();
virtual void slot92();
virtual void slot93();
virtual void slot94();
virtual void slot95();
virtual void slot96();
virtual void slot97();
virtual void slot98();
virtual void slot99();
virtual void slot100();
virtual void slot101();
virtual void slot102();
virtual void slot103();
virtual void slot104();
virtual void slot105();
virtual void slot106();
virtual void slot107();
virtual void slot108();
virtual void slot109();
virtual void slot110();
virtual void slot111();
virtual void slot112();
virtual void slot113();
virtual void slot114();
virtual void slot115();
virtual void slot116();
virtual void slot117();
virtual void slot118();
virtual void slot119();
virtual void slot120();
virtual void slot121();
virtual void slot122();
virtual void slot123();
virtual void slot124();
virtual void slot125();
virtual void slot126();
virtual void slot127();
virtual void slot128();
virtual void slot129();
virtual void slot130();
virtual void slot131();
virtual void slot132();
virtual void UnregisterDrag(); //slot133
char nativeBase[0x15c];bool Dragging;
void EndDrag();
};
void CHoverable::EndDrag(){Dragging=false;UnregisterDrag();}
}

#include <cstdio>
static int errors,calls,dtors;
namespace NUISystem {
CHoverable::~CHoverable(){++dtors;}
void CHoverable::slot1(){}
void CHoverable::slot2(){}
void CHoverable::slot3(){}
void CHoverable::slot4(){}
void CHoverable::slot5(){}
void CHoverable::slot6(){}
void CHoverable::slot7(){}
void CHoverable::slot8(){}
void CHoverable::slot9(){}
void CHoverable::slot10(){}
void CHoverable::slot11(){}
void CHoverable::slot12(){}
void CHoverable::slot13(){}
void CHoverable::slot14(){}
void CHoverable::slot15(){}
void CHoverable::slot16(){}
void CHoverable::slot17(){}
void CHoverable::slot18(){}
void CHoverable::slot19(){}
void CHoverable::slot20(){}
void CHoverable::slot21(){}
void CHoverable::slot22(){}
void CHoverable::slot23(){}
void CHoverable::slot24(){}
void CHoverable::slot25(){}
void CHoverable::slot26(){}
void CHoverable::slot27(){}
void CHoverable::slot28(){}
void CHoverable::slot29(){}
void CHoverable::slot30(){}
void CHoverable::slot31(){}
void CHoverable::slot32(){}
void CHoverable::slot33(){}
void CHoverable::slot34(){}
void CHoverable::slot35(){}
void CHoverable::slot36(){}
void CHoverable::slot37(){}
void CHoverable::slot38(){}
void CHoverable::slot39(){}
void CHoverable::slot40(){}
void CHoverable::slot41(){}
void CHoverable::slot42(){}
void CHoverable::slot43(){}
void CHoverable::slot44(){}
void CHoverable::slot45(){}
void CHoverable::slot46(){}
void CHoverable::slot47(){}
void CHoverable::slot48(){}
void CHoverable::slot49(){}
void CHoverable::slot50(){}
void CHoverable::slot51(){}
void CHoverable::slot52(){}
void CHoverable::slot53(){}
void CHoverable::slot54(){}
void CHoverable::slot55(){}
void CHoverable::slot56(){}
void CHoverable::slot57(){}
void CHoverable::slot58(){}
void CHoverable::slot59(){}
void CHoverable::slot60(){}
void CHoverable::slot61(){}
void CHoverable::slot62(){}
void CHoverable::slot63(){}
void CHoverable::slot64(){}
void CHoverable::slot65(){}
void CHoverable::slot66(){}
void CHoverable::slot67(){}
void CHoverable::slot68(){}
void CHoverable::slot69(){}
void CHoverable::slot70(){}
void CHoverable::slot71(){}
void CHoverable::slot72(){}
void CHoverable::slot73(){}
void CHoverable::slot74(){}
void CHoverable::slot75(){}
void CHoverable::slot76(){}
void CHoverable::slot77(){}
void CHoverable::slot78(){}
void CHoverable::slot79(){}
void CHoverable::slot80(){}
void CHoverable::slot81(){}
void CHoverable::slot82(){}
void CHoverable::slot83(){}
void CHoverable::slot84(){}
void CHoverable::slot85(){}
void CHoverable::slot86(){}
void CHoverable::slot87(){}
void CHoverable::slot88(){}
void CHoverable::slot89(){}
void CHoverable::slot90(){}
void CHoverable::slot91(){}
void CHoverable::slot92(){}
void CHoverable::slot93(){}
void CHoverable::slot94(){}
void CHoverable::slot95(){}
void CHoverable::slot96(){}
void CHoverable::slot97(){}
void CHoverable::slot98(){}
void CHoverable::slot99(){}
void CHoverable::slot100(){}
void CHoverable::slot101(){}
void CHoverable::slot102(){}
void CHoverable::slot103(){}
void CHoverable::slot104(){}
void CHoverable::slot105(){}
void CHoverable::slot106(){}
void CHoverable::slot107(){}
void CHoverable::slot108(){}
void CHoverable::slot109(){}
void CHoverable::slot110(){}
void CHoverable::slot111(){}
void CHoverable::slot112(){}
void CHoverable::slot113(){}
void CHoverable::slot114(){}
void CHoverable::slot115(){}
void CHoverable::slot116(){}
void CHoverable::slot117(){}
void CHoverable::slot118(){}
void CHoverable::slot119(){}
void CHoverable::slot120(){}
void CHoverable::slot121(){}
void CHoverable::slot122(){}
void CHoverable::slot123(){}
void CHoverable::slot124(){}
void CHoverable::slot125(){}
void CHoverable::slot126(){}
void CHoverable::slot127(){}
void CHoverable::slot128(){}
void CHoverable::slot129(){}
void CHoverable::slot130(){}
void CHoverable::slot131(){}
void CHoverable::slot132(){}
void CHoverable::UnregisterDrag(){++errors;}
}
struct TestDrag:NUISystem::CHoverable{virtual void UnregisterDrag(){if(Dragging)++errors;++calls;Dragging=true;}};
int main(){{TestDrag d;d.Dragging=true;d.EndDrag();if(calls!=1||!d.Dragging)++errors;d.Dragging=false;d.EndDrag();if(calls!=2||!d.Dragging)++errors;}if(dtors!=1)++errors;
if(errors){std::printf("0055bde0_TEST FAIL\n");return 1;}std::printf("0055bde0_TEST PASS\n");return 0;}