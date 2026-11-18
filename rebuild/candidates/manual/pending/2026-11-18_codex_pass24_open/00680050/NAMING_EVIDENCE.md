# Naming evidence

- Ego_r 0x006B0EC0 and FableWin 0x01FD7470 give the exact leaf `?Validate@CThingObjectDef@@UBEXABVCValidateDefinitionInitBase@@ABVCCharString@@@Z`.
- The validation calls native mesh metadata, the appearance DEFAULT animation and the weapon action-point checks. Ego_d's object_name and weapon strings are debug-scope values.
- Native 0x00680191 onward returns to the same CDefPointer data and cleanup state. CValidateDefinitionInit is the Ego_d specialization used by this check.

## Behavioral fixture

The fixture compiles and calls the selected Validate body. It exercises diagnostics, resource presence, CRC registration, and null/resource type conditions against a synthetic world and graphics bank.
