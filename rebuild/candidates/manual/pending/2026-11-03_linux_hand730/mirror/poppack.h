/* poppack.h -- the Windows SDK's packing restore, paired with pshpack1.h (see there for the retail evidence). */
#if ! (defined(lint) || defined(RC_INVOKED))
#if ( _MSC_VER >= 800 && !defined(_M_I86)) || defined(_PUSHPOP_SUPPORTED)
#pragma warning(disable:4103)
#pragma pack(pop)
#else
#pragma pack()
#endif
#endif
