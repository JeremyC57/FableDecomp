#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#ifdef _MSC_VER
typedef unsigned int PlacementSize;
#else
typedef __SIZE_TYPE__ PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
class CDateAndTime { public: long Year; unsigned long Month,Day,Hour,Minute,Second,Millisecond; CDateAndTime(); };
class CUnprovenRecord_0071f9f0 { public: unsigned long UnprovenValue; CDateAndTime UnprovenDate; CUnprovenRecord_0071f9f0(); };
CUnprovenRecord_0071f9f0::CUnprovenRecord_0071f9f0() : UnprovenValue(0) {}

static int calls=0; static void* lastThis;
CDateAndTime::CDateAndTime() { ++calls; lastThis=this; Year=0; Month=Day=Hour=Minute=Second=Millisecond=0; }
int main() { void* storage=malloc(sizeof(CUnprovenRecord_0071f9f0)); memset(storage,0xA5,sizeof(CUnprovenRecord_0071f9f0)); CUnprovenRecord_0071f9f0* object=new(storage) CUnprovenRecord_0071f9f0(); if(calls!=1 || object->UnprovenValue || lastThis!=&object->UnprovenDate || object->UnprovenDate.Year || object->UnprovenDate.Month || object->UnprovenDate.Day || object->UnprovenDate.Hour || object->UnprovenDate.Minute || object->UnprovenDate.Second || object->UnprovenDate.Millisecond) { puts("0071f9f0_TEST FAIL"); free(storage); return 1; } object->~CUnprovenRecord_0071f9f0(); free(storage); puts("0071f9f0_TEST PASS"); return 0; }