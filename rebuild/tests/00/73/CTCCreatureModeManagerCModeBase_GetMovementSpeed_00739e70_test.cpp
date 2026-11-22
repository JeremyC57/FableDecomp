// Test for 0x00739E70 CTCCreatureModeManager::CModeBase::GetMovementSpeed on engine/CTCCreatureModeManager.h (NF240): links the row
// object; a qualified (non-virtual) call on a raw buffer, so no vtable is needed. Cases: 0.0f for two movement types and for
// different direction / facing vectors (the arguments are not read).
#include <stdio.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"

int main()
{
    static float buf[0x40], dirA[4] = { 1.0f, 2.0f, 3.0f, 0.0f }, dirB[4] = { -5.0f, 0.5f, 9.0f, 0.0f };
    const CTCCreatureModeManager::CModeBase* mode = reinterpret_cast<const CTCCreatureModeManager::CModeBase*>(buf);
    const C3DVector& a = *reinterpret_cast<const C3DVector*>(dirA);
    const C3DVector& b = *reinterpret_cast<const C3DVector*>(dirB);
    if (mode->CTCCreatureModeManager::CModeBase::GetMovementSpeed(a, b, (EMovementType)0) != 0.0f) { printf("00739e70_TEST FAIL 1\n"); return 1; }
    if (mode->CTCCreatureModeManager::CModeBase::GetMovementSpeed(b, a, (EMovementType)2) != 0.0f) { printf("00739e70_TEST FAIL 2\n"); return 1; }
    printf("00739e70_TEST PASS\n");
    return 0;
}
