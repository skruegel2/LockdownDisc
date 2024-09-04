
/** This is an autogenerated file. DO NOT EDIT **/

/** Enabled memory drivers. */
#define SOC_RAM_DRV_ENABLED 0
#define SOC_FLASH_DRV_ENABLED 1
#define EXT_FLASH_DRV_ENABLED 0
#define EXT_MAPPED_MEM_DRV_ENABLED 0

/** Unique names and IDs for devices as supplied by the OEM. */
#define DEVICE_ID_INTERNAL_FLASH 0x0000

/** Memory subregion table initialiser. */
#define MEMORY_SUBREGIONS_INIT \
{ \
    {0x8000000u, 0x800ffffu, 0x4000u, 0x20u, 0xffu}, \
    {0x8010000u, 0x801ffffu, 0x10000u, 0x20u, 0xffu}, \
    {0x8020000u, 0x80fffffu, 0x20000u, 0x20u, 0xffu}, \
    {0x8100000u, 0x810ffffu, 0x4000u, 0x20u, 0xffu}, \
    {0x8110000u, 0x811ffffu, 0x10000u, 0x20u, 0xffu}, \
    {0x8120000u, 0x81fffffu, 0x20000u, 0x20u, 0xffu} \
}

/** Memory device table initialiser. */
#define MEMORY_DEVICES_INIT \
{ \
    {DEVICE_ID_INTERNAL_FLASH, "INTERNAL_FLASH", SOC_FLASH_DRV, 0, 5, false} \
}

/** Unique IDs for SWUP slots supplied by the OEM. */
#define SLOT_ID_SOFTWARE_UPDATE_AREA_SLOT 0x0

/** Total number of update slots. */
#define NUM_UPDATE_SLOTS 1

/** Unique IDs for other SBM slots that cannot be modified. */
#define SLOT_ID_SBM_SLOT 0x50
#define SLOT_ID_APP_STATUS_SLOT 0x51
#define SLOT_ID_APP_SLOT 0x52


/** Memory slot table initialisers. */
#define SBM_MEMORY_SLOT_INIT \
    {SLOT_ID_SBM_SLOT, "SBM_SLOT", SBM_SLOT_TYPE, 0, 0x8000000u, 0x10000u, true}

#define APP_STATUS_MEMORY_SLOT_INIT \
    {SLOT_ID_APP_STATUS_SLOT, "APP_STATUS_SLOT", ASS_SLOT_TYPE, 0, 0x8010000u, 0x10000u, false}

#define EXEC_MEMORY_SLOT_INIT \
    {SLOT_ID_APP_SLOT, "APP_SLOT", EXEC_SLOT_TYPE, 0, 0x8020000u, 0xe0000u, false}

#define UPDATE_MEMORY_SLOTS_INIT \
    {SLOT_ID_SOFTWARE_UPDATE_AREA_SLOT, "SOFTWARE_UPDATE_AREA_SLOT", UPDATE_SLOT_TYPE, 0, 0x8120000u, 0xe0000u, false}
