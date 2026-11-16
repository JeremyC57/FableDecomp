#ifndef FABLE_CDSOUNDACCELERATIONQUERY_H
#define FABLE_CDSOUNDACCELERATIONQUERY_H
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
struct _GUID;
// Descriptive existing name, not a recovered PDB name. Native C5DB40 obtains the
// device interface path; C5DAA0 copies at most259bytes then terminatesbyte259.
// C5DD70 reads DirectSound\Mixer Defaults/Acceleration and returns its level.
class CDSoundAccelerationQuery {
public: CDSoundAccelerationQuery();int GetAccelerationLevel(const _GUID*);
private: char m_deviceInterfacePath[260];
};
#endif
#endif
