#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CDeletion.h"
using namespace NUISystem;
void f1(CList* l, CCountedPointer<CComponent>& c) { l->AddScrollingChildAndSetOffsets(c); }
void f2(CList* l, CCountedPointer<CComponent>& c) { l->AddScrollingChild(c); }
void f3(CList* l, CCountedPointer<CComponent>& c) { l->AddNonScrollingChild(c); }
void f4(CList* l) { l->InitialiseOffsets(1); }
void f5(CList* l) { l->SetDefaultChild(1L); }
void f6(CList* l) { l->SetSelectedChild(1L); }
